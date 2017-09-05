#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include <CBSDL/Defines.h>
#include <CBGL/Fwd.h>

#include "GameObject.h"

namespace pong {
  class CGameField 
    : public CGameObject
  {
  private:
    std::unique_ptr<cb::gl::CBuffer> mBuffer;

  public:
    CGameField(glm::vec2 const& pos, glm::vec2 const& size, glm::vec4 const& color);
    virtual ~CGameField();

    virtual void Update(CGame & game, float const timeDelta) override;

    virtual void UpdateRender() override;

    virtual void Render(cb::gl::CProgram & glProgram, glm::mat4 const & transform) const override;
  };
}
