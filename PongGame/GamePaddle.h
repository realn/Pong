#pragma once

#include <memory>
#include <CBGL/Fwd.h>

#include <CoreBRect.h>

#include "GameObject.h"

namespace pong {
  enum class PaddleSide {
    Left, Right, Top, Bottom,
  };
  enum class PaddleMoveDir {
    None, Up, Down, Left, Right,
  };

  class CGamePaddle
    : public CGameObject {
  private:
    glm::vec4 mColor;
    PaddleSide mSide;
    PaddleMoveDir mMoveDir;
    float mAccel;

  public:
    CGamePaddle(PaddleSide const side, glm::vec2 const& size, float const speed, float const accel);
    virtual ~CGamePaddle() {}

    void SetMoveDir(PaddleMoveDir const value) { mMoveDir = value; }

    PaddleMoveDir GetMoveDir() const { return mMoveDir; }

    void Update(CGame& game, float const timeDelta) override;
    void UpdateRender(gfx::CCanvas& canvas) override;
  };
}