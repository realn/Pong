#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include <CBSDL/Defines.h>
#include <CBGL/Fwd.h>

#include <CoreBRect.h>

#include "GameObject.h"

namespace pong {
  class CGameField 
    : public CGameObject
  {
  private:

  public:
    CGameField();
    virtual ~CGameField();

    virtual void Update(float const timeDelta) override;

    virtual void UpdateRender(gfx::CCanvas& canvas) override;
  };
}
