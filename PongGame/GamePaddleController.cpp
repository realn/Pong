#include "stdafx.h"

#include <glm/gtc/epsilon.hpp>

#include <CoreInput.h>

#include "Game.h"
#include "GameField.h"
#include "GamePaddle.h"

#include "GamePaddleController.h"

namespace pong {
  CGamePaddleControllerBase::CGamePaddleControllerBase(std::shared_ptr<CGameObject> paddle) 
    : mPaddle(std::dynamic_pointer_cast<CGamePaddle>(paddle))
  {}

  CGamePaddleControllerBase::~CGamePaddleControllerBase() {}

  //CGamePaddleMouseController::CGamePaddleMouseController(std::shared_ptr<CGameObject> paddle) 
  //  : CGamePaddleControllerBase(paddle)
  //{}

  //CGamePaddleMouseController::~CGamePaddleMouseController() {}

  //bool CGamePaddleMouseController::InitController(CGame& game) {
  //  //core::bind<IMouseEventObserver>(game, *this);
  //  return true;
  //}

  //void CGamePaddleMouseController::Update(CGame & game, float const timeDelta) {
  //  glm::vec2 paddlePos = mPaddle->GetCenterPos() / game.GetField().GetSize();
  //  if(glm::epsilonEqual(paddlePos.y, mMousePos.y, 0.01f)) {
  //    mPaddle->SetMoveDir(PaddleMoveDir::None);
  //  }
  //  else if(paddlePos.y < mMousePos.y) {
  //    mPaddle->SetMoveDir(PaddleMoveDir::Up);
  //  }
  //  else if(paddlePos.y > mMousePos.y) {
  //    mPaddle->SetMoveDir(PaddleMoveDir::Down);
  //  }
  //}

  CGamePaddleLocalController::CGamePaddleLocalController(std::shared_ptr<CGameObject> paddle,
                                                         const CGamePaddleLocalControllerConfig& config) 
    : CGamePaddleControllerBase(paddle)
    , mConfig(config)
    , mMoveUp(false), mMoveDown(false)
  {}

  CGamePaddleLocalController::~CGamePaddleLocalController() {}

  bool CGamePaddleLocalController::InitController(CGame& game) {
    core::bind<core::IInputObserver>(game.GetInput(), *this);
    return true;
  }

  void CGamePaddleLocalController::Update(CGame & game, float const timeDelta) {
    //if(mMoveUp) {
    //  mPaddle->SetMoveDir(PaddleMoveDir::Up);
    //}
    //else if(mMoveDown) {
    //  mPaddle->SetMoveDir(PaddleMoveDir::Down);
    //}
    //else {
    //  mPaddle->SetMoveDir(PaddleMoveDir::None);
    //}
  }

  void CGamePaddleLocalController::OnInputState(core::InputEventId const id, bool value) {
    if(id == mConfig.mUpId) {
      mMoveUp = value;
    }
    if(id == mConfig.mDownId) {
      mMoveDown = value;
    }
  }
}
