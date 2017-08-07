#include "stdafx.h"
#include "GamePaddleController.h"
#include "GamePaddle.h"
#include "Game.h"

namespace pong {
  CGamePaddleControllerBase::CGamePaddleControllerBase(std::shared_ptr<CGamePaddle> paddle) 
    : mPaddle(paddle)
  {}

  CGamePaddleControllerBase::~CGamePaddleControllerBase() {}

  CGamePaddleMouseController::CGamePaddleMouseController(std::shared_ptr<CGamePaddle> paddle) 
    : CGamePaddleControllerBase(paddle)
  {}

  CGamePaddleMouseController::~CGamePaddleMouseController() {}

  void CGamePaddleMouseController::Update(CGame & game, float const timeDelta) {
    glm::vec2 paddlePos = mPaddle->GetCenterPos() / game.GetFieldSize();
    if(paddlePos.y < mMousePos.y) {
      mPaddle->SetMoveDir(PaddleMoveDir::Up);
    }
    else if(paddlePos.y > mMousePos.y) {
      mPaddle->SetMoveDir(PaddleMoveDir::Down);
    }
    else {
      mPaddle->SetMoveDir(PaddleMoveDir::None);
    }
  }
}
