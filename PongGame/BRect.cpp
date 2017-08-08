#include "stdafx.h"
#include "BRect.h"

namespace pong {
  CBRect CBRect::GetEdge(RectEdge side) const {
    switch(side) {
    case pong::RectEdge::XMin: return CBRect(GetMin(), {0.0f, mSize.y});
    case pong::RectEdge::XMax: return CBRect(GetMaxMin(), {0.0f, mSize.y});
    case pong::RectEdge::YMin: return CBRect(GetMin(), {mSize.x, 0.0f});
    case pong::RectEdge::YMax: return CBRect(GetMinMax(), {mSize.x, 0.0f});
    default:
      return *this;
    }
  }

  CBRect CBRect::GetCorner(RectCorner corner) const {
    switch(corner) {
    case pong::RectCorner::Min:     return CBRect(GetMin(), {0.0f, 0.0f});
    case pong::RectCorner::MinMax:  return CBRect(GetMinMax(), {0.0f, 0.0f});
    case pong::RectCorner::Max:     return CBRect(GetMax(), {0.0f, 0.0f});
    case pong::RectCorner::MaxMin:  return CBRect(GetMaxMin(), {0.0f, 0.0f});
    default:
      return *this;
    }
  }

  inline bool CBRect::Contains(glm::vec2 const & pos) const {
    return 
      glm::all(glm::greaterThan(pos, GetMin())) &&
      glm::all(glm::lessThan(pos, GetMax()));
  }

  bool CBRect::Contains(CBRect const & rect) const {
    return Contains(rect.GetMin()) && Contains(rect.GetMax());
  }

  RectEdge CBRect::ContainsEdgeOf(CBRect const & rect) const {
    for(auto side : {RectEdge::XMin, RectEdge::XMax, RectEdge::YMin, RectEdge::YMax}) {
      if(Contains(rect.GetEdge(side))) {
        return side;
      }
    }
    return RectEdge::None;
  }

  RectCorner CBRect::ContainsCornerOf(CBRect const & rect) const {
    for(auto corner : {RectCorner::Min, RectCorner::Max, RectCorner::MinMax, RectCorner::MaxMin}) {
      if(Contains(rect.GetCorner(corner))) {
        return corner;
      }
    }
    return RectCorner::None;
  }

  glm::vec2 CBRect::GetNormal(RectEdge edge) {
    switch(edge) {
    case pong::RectEdge::XMin:  return {-1.0f, 0.0f};
    case pong::RectEdge::XMax:  return {1.0f, 0.0f};
    case pong::RectEdge::YMin:  return {0.0f, -1.0f};
    case pong::RectEdge::YMax:  return {0.0f, 1.0f};
    default:
    case pong::RectEdge::None:  return {0.0f, 0.0f};
    }
  }

  glm::vec2 CBRect::GetNormal(RectCorner corner) {
    switch(corner) {
    case pong::RectCorner::Min:     return glm::normalize(glm::vec2{-1.0f, -1.0f});
    case pong::RectCorner::MinMax:  return glm::normalize(glm::vec2{-1.0f, 1.0f});
    case pong::RectCorner::Max:     return glm::normalize(glm::vec2{1.0f, 1.0f});
    case pong::RectCorner::MaxMin:  return glm::normalize(glm::vec2{1.0f, -1.0f});
    default:
    case pong::RectCorner::None:    return glm::vec2();
    }
  }
}
