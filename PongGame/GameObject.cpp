#include "stdafx.h"
#include "GameObject.h"

#include <CoreBRect.h>

namespace pong {
  core::CBRect CGameObject::GetBRect() const {
    return core::CBRect(GetPosition(), GetSize());
  }
}