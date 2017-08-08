#pragma once

#include <memory>
#include <CBGL/Fwd.h>

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
    std::unique_ptr<cb::gl::CBuffer> mBuffer;
    PaddleSide mSide;
    PaddleMoveDir mMoveDir;
    float mAccel;

  public:
    CGamePaddle(PaddleSide const side, glm::vec2 const& size, float const speed, float const accel);
    virtual ~CGamePaddle() {}

    void SetMoveDir(PaddleMoveDir const value) { mMoveDir = value; }

    PaddleMoveDir GetMoveDir() const { return mMoveDir; }

    void Update(CGame& game, float const timeDelta) override;
    void UpdateRender() override;
    void Render(cb::gl::CProgram& glProgram, glm::mat4 const& transform) const override;
  };
}