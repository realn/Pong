#include "stdafx.h"
#include "BRect.h"

namespace pong {
  inline bool CBRect::DoesBRectContains(glm::vec2 const & pos) const {
    return 
      glm::all(glm::greaterThan(pos, GetBRectMin())) &&
      glm::all(glm::lessThan(pos, GetBRectMax()));
  }

  RectSide CBRect::GetCollisionSide(CBRect const & rect) const {
    return RectSide();
  }
}
