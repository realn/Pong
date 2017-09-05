#include "stdafx.h"
#include "GameObject.h"
#include "BRect.h"

namespace pong {
  gfx::CBRect CGameObject::GetBRect() const {
    return gfx::CBRect(GetPosition(), GetSize());
  }
}