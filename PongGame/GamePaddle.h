#pragma once

#include <memory>
#include <CBGL/Fwd.h>

#include <CoreBRect.h>

#include "GameMovingObject.h"

namespace pong {
  enum class PaddleSide {
    Left, Right, Top, Bottom,
  };
  enum class PaddleMoveDir {
    None, Up, Down, Left, Right,
  };

  class CGameField;

  class CGamePaddle
    : public CGameMovingObject {
  private:
    std::shared_ptr<CGameField> mField;
    float mAccel = 0.0f;
    PaddleSide mSide = PaddleSide::Left;
    PaddleMoveDir mMoveDir = PaddleMoveDir::None;

  public:
    CGamePaddle();
    ~CGamePaddle();

    void SetField(std::shared_ptr<CGameField> field);
    void SetAccel(const float accel) { mAccel = accel; }
    void SetPaddleSide(const PaddleSide side) { mSide = side; }
    void SetMoveDir(PaddleMoveDir const value) { mMoveDir = value; }

    PaddleMoveDir GetMoveDir() const { return mMoveDir; }

    void Update(float const timeDelta) override;
    void UpdateRender(gfx::CCanvas& canvas) override;
  };
}