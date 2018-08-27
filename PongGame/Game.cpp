#include "stdafx.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CoreApp.h>
#include <CoreInput.h>
#include <GFXCanvas.h>
#include <GFXCanvasView.h>

#include "Assets.h"
#include "Consts.h"
#include "GameConsts.h"
#include "GamePaddle.h"
#include "GameBall.h"
#include "GameField.h"
#include "GameController.h"
#include "GameSceneLevel.h"

#include "Game.h"

const glm::vec2 FIELD_PROPS {5.0f, 3.0f};
const float FIELD_ASP_RATIO = FIELD_PROPS.x / FIELD_PROPS.y;

namespace pong {
  CGame::CGame(cb::strvector const& args)
    : core::CApp(args)
  {}

  CGame::~CGame() {}

  bool CGame::AdjustConfig(core::CAppConfig& config) {
    config.WindowTitle = L"Pong Game"s;

    return true;
  }

  bool CGame::Init() {
    auto aspectRatio = GetConfig().GetAspectRatio();
    mScreenSize = glm::vec2(2.0f) * glm::vec2(aspectRatio, 1.0f);

    {
      auto fieldPos = glm::vec2();
      auto fieldSize = glm::vec2();
      if(aspectRatio >= FIELD_ASP_RATIO) {
        fieldSize = { FIELD_ASP_RATIO * mScreenSize.y, mScreenSize.y };
      }
      else {
        fieldSize = { mScreenSize.x, mScreenSize.x / FIELD_ASP_RATIO };
      }
      fieldPos = (mScreenSize - fieldSize) / 2.0f;
      mField = std::make_unique<CGameField>(fieldPos, fieldSize, glm::vec4(0.5f, 0.1f, 0.2f, 1.0f));
    }

    core::bind(GetInput(), *this);

    GetInput().RegisterEvent(L"PLAYER_ONE_UP", GameInput::PLAYER_ONE_UP);
    GetInput().RegisterEvent(L"PLAYER_ONE_DOWN", GameInput::PLAYER_ONE_DOWN);
    GetInput().RegisterEvent(L"PLAYER_ONE_LEFT", GameInput::PLAYER_ONE_LEFT);
    GetInput().RegisterEvent(L"PLAYER_ONE_RIGHT", GameInput::PLAYER_ONE_RIGHT);

    GetInput().RegisterEvent(L"PLAYER_TWO_UP", GameInput::PLAYER_TWO_UP);
    GetInput().RegisterEvent(L"PLAYER_TWO_DOWN", GameInput::PLAYER_TWO_DOWN);
    GetInput().RegisterEvent(L"PLAYER_TWO_LEFT", GameInput::PLAYER_TWO_LEFT);
    GetInput().RegisterEvent(L"PLAYER_TWO_RIGHT", GameInput::PLAYER_TWO_RIGHT);

    GetInput().RegisterEvent(L"GAME_QUIT", GameInput::GAME_QUIT);

    GetInput().RegisterEvent(L"MENU_ITEM_ENTER)", GameInput::MENU_ITEM_ENTER);
    GetInput().RegisterEvent(L"MENU_ITEM_RETURN)", GameInput::MENU_ITEM_RETURN);

    AddPlayer(GameControllerType::Mouse);
    AddPlayer(GameControllerType::Keyboard);

    GetInput().AddBinding(L"Keyboard", L"Escape", L"QUIT_APP");

    mBall = std::make_unique<CGameBall>(glm::vec2{ 0.1f, 0.1f }, 1.5f);
    mBall->SetPosition((mField->GetSize() - mBall->GetSize()) / 2.0f);

    mAssets = std::make_unique<CAssets>(L"Assets"s);

    auto textureFileName = L"texture.png"s;
    auto textureSize = glm::uvec2(512);
    auto texture = mAssets->Textures.Get(textureFileName);
    auto shaderProg = mAssets->Shaders.Get({ L"font_vs"s, L"font_fs"s });

    shaderProg->SetInLocation(gfx::CCanvasVertex::Inputs);
    shaderProg->Link();

    mCanvas = std::make_unique<gfx::CCanvas>(gfx::CTextureAtlas(textureFileName, textureSize));
    mCanvasView = std::make_unique<gfx::CCanvasView>(shaderProg, texture, texture);

    return true;
  }

  std::unique_ptr<core::CScene> CGame::CreateScene() {
	  return std::make_unique<CGameSceneMain>();
  }

  void CGame::AddPlayer(GameControllerType controllerType) {
    auto playerIndex = cb::u32(mPaddles.size());
    auto side = GetPlayerPaddleSide(playerIndex);

    auto paddle = std::make_shared<CGamePaddle>(side, GetPaddleSize(side), 0.5f, 4.0f);
    paddle->SetPosition(GetPaddleStartPos(*paddle, side));
    mPaddles.push_back(paddle);

    AddController(paddle, controllerType);
  }

  PaddleSide CGame::GetPlayerPaddleSide(cb::u32 const index) const {
    switch(index) {
    case 0: return PaddleSide::Left;
    case 1: return PaddleSide::Right;
    case 2: return PaddleSide::Top;
    case 3: return PaddleSide::Bottom;
    default:
      return PaddleSide::Left;
    }
  }

  glm::vec2 CGame::GetPaddleSize(PaddleSide const side) const {
    auto size = mField->GetSize() / glm::vec2(16.0f, 3.0f);
    switch(side) {
    case PaddleSide::Left:
    case PaddleSide::Right:
      return size;

    case PaddleSide::Top:
    case PaddleSide::Bottom:
      return {size.y, size.x};
    default:
      return {};
    }
  }

  glm::vec2 CGame::GetPaddleStartPos(CGamePaddle const & paddle, PaddleSide const side) const {
    auto padding = mField->GetSize().x / 16.0f;
    auto centerPos = (mField->GetSize() - paddle.GetSize()) / 2.0f;
    switch(side) {
    case PaddleSide::Left:   return {padding, centerPos.y};
    case PaddleSide::Right:  return {mField->GetSize().x - paddle.GetSize().x - padding, centerPos.y};
    case PaddleSide::Top:    return {centerPos.x, mField->GetSize().y - paddle.GetSize().y - padding};
    case PaddleSide::Bottom: return {centerPos.x, padding};
    default:
      return {};
    }
  }

  void CGame::AddController(std::shared_ptr<CGameObject> target, GameControllerType const type) {
    auto controller = IGameController::Create(type, target);
    if(!controller->InitController(*this)) {
      return;
    }
    mControllers.push_back(controller);
  }

  void CGame::OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) {
    //auto gamePos = glm::vec2{ pos.x, 1.0f - pos.y } * mScreenSize;

    //for(auto& observer : IEventSource<IMouseEventObserver>::GetObservers()) {
    //  observer->EventMouseMove(gamePos / mField->GetSize());
    //}
  }

  void CGame::OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) {
  }

  void CGame::OnAppClose() {
    Quit();
  }

  void CGame::OnInputAction(core::InputEventId const id) {
    if(id == 0) {
      Quit();
    }
  }

  void CGame::OnInputState(core::InputEventId const id, bool value) {
  }

  void CGame::OnInputRange(core::InputEventId const id, float const value, float const prevValue) {
  }
}
