#include "stdafx.h"
#include "GamePaddle.h"
#include "GameField.h"
#include "Game.h"
#include "Consts.h"

#include <GFXCanvas.h>
#include <GFXConsts.h>

namespace pong {
  CGamePaddle::CGamePaddle(PaddleSide const side,
                           glm::vec2 const & size,
                           float const speed,
                           float const accel,
                           const glm::vec2& fieldSize)
    : CGameObject(size, speed)
    , mColor(1.0f, 0.0f, 0.5f, 1.0f)
    , mMoveDir(PaddleMoveDir::None)
    , mSide(side)
    , mAccel(accel)
    , mFieldSize(fieldSize)
  {}

  void CGamePaddle::Update(float const timeDelta) {
    auto dir = glm::vec2();
    switch(mMoveDir) {
    case PaddleMoveDir::Up:     dir = { 0.0f, 1.0f }; break;
    case PaddleMoveDir::Down:   dir = { 0.0f, -1.0f }; break;
    case PaddleMoveDir::Left:   dir = { -1.0f, 0.0f }; break;
    case PaddleMoveDir::Right:  dir = { 1.0f, 0.0f }; break;
    default:  dir = -glm::sign(mVec); break;
    }

    mVec = glm::clamp(mVec + dir * mAccel * timeDelta, -1.0f, 1.0f);

    mPos += mVec * mSpeed * timeDelta;

    if(mPos.y < 0.0f || mPos.y + mSize.y > mFieldSize.y) {
      mPos.y = glm::clamp(mPos.y, 0.0f, mFieldSize.y - mSize.y);
      mVec = glm::vec2(0.0f);
    }
  }

  void CGamePaddle::UpdateRender(gfx::CCanvas& canvas) {
    canvas.DrawRect(GetBRect(), mColor);
  }
}
