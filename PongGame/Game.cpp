#include "stdafx.h"

#include <GFXCanvas.h>
#include <GFXCanvasView.h>

#include "Assets.h"
#include "Consts.h"
#include "GamePaddle.h"
#include "GamePaddleControllerImpl.h"
#include "GameBall.h"
#include "GameField.h"

#include "Game.h"

const glm::vec2 FIELD_PROPS{5.0f, 3.0f};
const float FIELD_ASP_RATIO = FIELD_PROPS.x / FIELD_PROPS.y;

namespace pong {
  CGame::CGame(glm::vec2 const & screenSize, CAssets& assets) {
    {
      auto screenAspRatio = screenSize.x / screenSize.y;
      auto fieldPos = glm::vec2();
      auto fieldSize = glm::vec2();
      if(screenAspRatio >= FIELD_ASP_RATIO) {
        fieldSize = {FIELD_ASP_RATIO * screenSize.y, screenSize.y};
      }
      else {
        fieldSize = {screenSize.x, screenSize.x / FIELD_ASP_RATIO};
      }
      fieldPos = (screenSize - fieldSize) / 2.0f;
      mField = std::make_unique<CGameField>(fieldPos, fieldSize, glm::vec4(0.5f, 0.1f, 0.2f, 1.0f));
    }

    AddPlayer(PaddleControllerType::Mouse);
    AddPlayer(PaddleControllerType::Keyboard);

    mBall = std::make_unique<CGameBall>(glm::vec2{0.1f, 0.1f}, 1.5f);
    mBall->SetPosition((mField->GetSize() - mBall->GetSize()) / 2.0f);

    auto textureFileName = L"texture.png"s;
    auto textureSize = glm::uvec2(512);
    auto texture = assets.Textures.Get(textureFileName);
    auto shaderProg = assets.Shaders.Get({ L"font_vs"s, L"font_fs"s });

    shaderProg->SetInLocation(gfx::CCanvasVertex::Inputs);
    shaderProg->Link();

    mCanvas = std::make_unique<gfx::CCanvas>(gfx::CTextureAtlas(textureFileName, textureSize));

    mCanvasView = std::make_unique<gfx::CCanvasView>(shaderProg, texture, texture);
  }

  CGame::~CGame() {}

  void CGame::AddPlayer(PaddleControllerType controllerType) {
    auto playerIndex = cb::u32(mPaddles.size());
    auto side = GetPlayerPaddleSide(playerIndex);

    auto paddle = std::make_shared<CGamePaddle>(side, GetPaddleSize(side), 0.5f, 4.0f);
    paddle->SetPosition(GetPaddleStartPos(*paddle, side));
    mPaddles.push_back(paddle);

    AddController(paddle, controllerType);
  }

  void CGame::Update(float const timeDelta) {
    for(auto& controller : mControllers) {
      controller->Update(*this, timeDelta);
    }
    for(auto& paddle : mPaddles) {
      paddle->Update(*this, timeDelta);
    }
    mBall->Update(*this, timeDelta);
  }

  void CGame::UpdateRender() {
    mCanvas->Clear();
    for(auto& paddle : mPaddles) {
      paddle->UpdateRender(*mCanvas);
    }
    mBall->UpdateRender(*mCanvas);

    mCanvasView->UpdateRender(*mCanvas);
  }

  void CGame::Render(glm::mat4 const & transform) {
    mCanvasView->Render(transform);
  }

  void CGame::EventMousePos(glm::vec2 const & pos) {
    for(auto& observer : mMouseEventObservers) {
      observer->EventMouseMove(pos / mField->GetSize());
    }
  }

  void CGame::EventKeyPress(cb::sdl::ScanCode code, cb::sdl::KeyState state) {
    for(auto& observer : mKeyboardEventObservers) {
      observer->EventKeyPress(code, state);
    }
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
    case PaddleSide::Left:  return {padding, centerPos.y};
    case PaddleSide::Right: return {mField->GetSize().x - paddle.GetSize().x - padding, centerPos.y};
    case PaddleSide::Top:   return {centerPos.x, mField->GetSize().y - paddle.GetSize().y - padding};
    case PaddleSide::Bottom:return {centerPos.x, padding};
    default:
      return {};
    }
  }

  void CGame::AddController(std::shared_ptr<CGamePaddle> paddle, PaddleControllerType const type) {
    switch(type) {
    case PaddleControllerType::Mouse:
      {
        auto controller = std::make_shared<CGamePaddleMouseController>(paddle);
        mControllers.push_back(controller);
        mMouseEventObservers.push_back(controller);
        break;
      }

    case PaddleControllerType::Keyboard:
      {
        auto controller = std::make_shared<CGamePaddleKeyboardController>(paddle);
        mControllers.push_back(controller);
        mKeyboardEventObservers.push_back(controller);
        break;
      }

    default:
      break;
    }
  }
}
