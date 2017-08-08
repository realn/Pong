#include "stdafx.h"
#include "GameObject.h"
#include "BRect.h"

namespace pong {
  CBRect CGameObject::GetBRect() const {
    return CBRect(GetPosition(), GetSize());
  }
}