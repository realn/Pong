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
#include "GameScene.h"

#include "Game.h"

const glm::vec2 FIELD_PROPS {5.0f, 3.0f};
const float FIELD_ASP_RATIO = FIELD_PROPS.x / FIELD_PROPS.y;

namespace pong {
  CGame::CGame(cb::strvector const& args)
    : core::CApp(args)
  {}

  CGame::~CGame() {}

  bool CGame::OnAdjustConfig(core::CAppConfig& config) {
    config.WindowTitle = L"Pong Game"s;

    return true;
  }

  bool CGame::OnInit() {
    auto aspectRatio = GetConfig().GetAspectRatio();
    mScreenSize = glm::vec2(2.0f) * glm::vec2(aspectRatio, 1.0f);

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

    GetInput().AddBinding(L"Keyboard", L"Escape", L"GAME_QUIT");

    mAssets = std::make_unique<CAssets>(L"Assets"s);

    return true;
  }

  std::unique_ptr<core::CScene> CGame::OnCreateScene() {
    auto scene = std::make_unique<CGameScene>(mScreenSize, *mAssets);
    
    {
      auto field = std::make_shared<CGameField>();
      field->SetPosition(mScreenSize * 0.1f);
      field->SetSize(mScreenSize * 0.8f);
      field->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
      scene->AddObject(field);
    }

    return scene;
  }

  void CGame::OnAppClose() {
    Quit();
  }

  void CGame::OnInputAction(core::InputEventId const id) {
    if(id == GameInput::GAME_QUIT) {
      Quit();
    }
  }

  void CGame::OnInputState(core::InputEventId const id, bool value) {
  }

  void CGame::OnInputRange(core::InputEventId const id, float const value, float const prevValue) {
  }
}
