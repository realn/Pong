#pragma once

#include "GameObject.h"
#include <CBGL/Buffer.h>

namespace pong {
  class CGameBall
    : public CGameObject {
  private:
    std::shared_ptr<cb::gl::CBuffer> mBuffer;
    glm::vec2 mVec;

  public:
    CGameBall(glm::vec2 const& size, float const speed);
    virtual ~CGameBall();

    virtual void Update(CGame & game, float const timeDelta) override;

    virtual void UpdateRender() override;

    virtual void Render(cb::gl::CProgram & glProgram, glm::mat4 const & transform) const override;

  };
}