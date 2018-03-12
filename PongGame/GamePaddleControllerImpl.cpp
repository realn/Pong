#include "stdafx.h"

#include <glm/gtc/epsilon.hpp>

#include "Game.h"
#include "GameField.h"
#include "GamePaddle.h"

#include "GamePaddleControllerImpl.h"

namespace pong {
  CGamePaddleControllerBase::CGamePaddleControllerBase(std::shared_ptr<CGameObject> paddle) 
    : mPaddle(std::dynamic_pointer_cast<CGamePaddle>(paddle))
  {}

  CGamePaddleControllerBase::~CGamePaddleControllerBase() {}

  CGamePaddleMouseController::CGamePaddleMouseController(std::shared_ptr<CGameObject> paddle) 
    : CGamePaddleControllerBase(paddle)
  {}

  CGamePaddleMouseController::~CGamePaddleMouseController() {}

  bool CGamePaddleMouseController::InitController(CGame& game) {
    core::bind<IMouseEventObserver>(game, *this);
    return true;
  }

  void CGamePaddleMouseController::Update(CGame & game, float const timeDelta) {
    glm::vec2 paddlePos = mPaddle->GetCenterPos() / game.GetField().GetSize();
    if(glm::epsilonEqual(paddlePos.y, mMousePos.y, 0.01f)) {
      mPaddle->SetMoveDir(PaddleMoveDir::None);
    }
    else if(paddlePos.y < mMousePos.y) {
      mPaddle->SetMoveDir(PaddleMoveDir::Up);
    }
    else if(paddlePos.y > mMousePos.y) {
      mPaddle->SetMoveDir(PaddleMoveDir::Down);
    }
  }

  CGamePaddleKeyboardController::CGamePaddleKeyboardController(std::shared_ptr<CGameObject> paddle) 
    : CGamePaddleControllerBase(paddle)
    , mMoveUp(false), mMoveDown(false)
  {}

  CGamePaddleKeyboardController::~CGamePaddleKeyboardController() {}

  bool CGamePaddleKeyboardController::InitController(CGame& game) {
    core::bind<IKeyboardEventObserver>(game, *this);
    return true;
  }

  void CGamePaddleKeyboardController::Update(CGame & game, float const timeDelta) {
    if(mMoveUp) {
      mPaddle->SetMoveDir(PaddleMoveDir::Up);
    }
    else if(mMoveDown) {
      mPaddle->SetMoveDir(PaddleMoveDir::Down);
    }
    else {
      mPaddle->SetMoveDir(PaddleMoveDir::None);
    }
  }

  void CGamePaddleKeyboardController::EventKeyPress(cb::sdl::ScanCode key, cb::sdl::KeyState state) {
    if(key == cb::sdl::ScanCode::W) {
      mMoveUp = state == cb::sdl::KeyState::PRESSED;
    }
    if(key == cb::sdl::ScanCode::S) {
      mMoveDown = state == cb::sdl::KeyState::PRESSED;
    }
  }
}
