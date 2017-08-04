#pragma once

#include <memory>
#include <CBGL/Fwd.h>

#include "GameObject.h"

namespace pong {
  class CGamePaddle
    : public CGameObject {
  private:
    std::unique_ptr<cb::gl::CBuffer> mBuffer;
    glm::vec2 mTargetPos;

  public:
    CGamePaddle(glm::vec2 const& size, glm::vec2 const& pos, float const speed, float const accel);
    virtual ~CGamePaddle() {}

    void SetTargetPos(glm::vec2 const& value) { mTargetPos = value; }

    glm::vec2 GetTargetPos() const { return mTargetPos; }

    void Update(CGame& game, float const timeDelta) override;
    void UpdateRender() override;
    void Render(cb::gl::CProgram& glProgram, glm::mat4 const& transform) const override;
  };
}