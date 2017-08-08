#include "stdafx.h"
#include "Game.h"
#include "Vertex.h"
#include "Consts.h"
#include "GamePaddle.h"
#include "GamePaddleControllerImpl.h"
#include "GameBall.h"
#include "BRect.h"

const glm::vec2 FIELD_PROPS{5.0f, 3.0f};
const float FIELD_ASP_RATIO = FIELD_PROPS.x / FIELD_PROPS.y;

namespace pong {
  CGame::CGame(glm::vec2 const & screenSize) {
    auto screenAspRatio = screenSize.x / screenSize.y;
    if(screenAspRatio >= FIELD_ASP_RATIO) {
      mFieldSize = {FIELD_ASP_RATIO * screenSize.y, screenSize.y};
    }
    else {
      mFieldSize = {screenSize.x, screenSize.x / FIELD_ASP_RATIO};
    }
    mFieldPos = (screenSize - mFieldSize) / 2.0f;

    auto fieldMin = mFieldPos;
    auto fieldMax = mFieldPos + mFieldSize;
    auto color = glm::vec4{0.5f, 0.5f, 0.5f, 1.0f};
    mBuffer = std::make_unique<cb::gl::CBuffer>();
    mBuffer->SetData(std::vector<CVertex>{
      {fieldMin, 0.0f, 0.0f, color},
      {fieldMax.x, fieldMin.y, 1.0f, 0.0f, color},
      {fieldMax, 1.0f, 1.0f, color},
      {fieldMin.x, fieldMax.y, 0.0f, 1.0f, color},
    });

    AddPlayer(PaddleControllerType::Mouse);
    AddPlayer(PaddleControllerType::Keyboard);

    mBall = std::make_unique<CGameBall>(glm::vec2{0.1f, 0.1f}, 1.5f);
    mBall->SetPosition((mFieldSize - mBall->GetSize()) / 2.0f);
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
    for(auto& paddle : mPaddles) {
      paddle->UpdateRender();
    }
    mBall->UpdateRender();
  }

  void CGame::Render(cb::gl::CProgram & glProgram, glm::mat4 const & transform) {
    glProgram.SetUniform(render::UNI_TRANSFORM, transform);

    auto gbuf = cb::gl::bind(*mBuffer);
    auto gver = cb::gl::bind(CVertex::Def);

    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, std::vector<glm::u16>{0, 1, 2, 0, 2, 3});

    for(auto& paddle : mPaddles) {
      paddle->Render(glProgram, transform);
    }
    mBall->Render(glProgram, transform);
  }

  void CGame::EventMousePos(glm::vec2 const & pos) {
    for(auto& observer : mMouseEventObservers) {
      observer->EventMouseMove(pos / mFieldSize);
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
    auto size = mFieldSize / glm::vec2(16.0f, 3.0f);
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
    auto padding = mFieldSize.x / 16.0f;
    auto centerPos = (mFieldSize - paddle.GetSize()) / 2.0f;
    switch(side) {
    case PaddleSide::Left:  return {padding, centerPos.y};
    case PaddleSide::Right: return {mFieldSize.x - paddle.GetSize().x - padding, centerPos.y};
    case PaddleSide::Top:   return {centerPos.x, mFieldPos.y - paddle.GetSize().y - padding};
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
