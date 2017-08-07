#pragma once

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace pong {
  enum class RectSide {
    None, Left, Top, Right, Bottom
  };

  class CBRect {
  private:
    glm::vec2 mPos;
    glm::vec2 mSize;

  public:
    CBRect() = default;
    CBRect(glm::vec2 const& pos, glm::vec2 const& size)
      : mPos(pos), mSize(size) {}

    glm::vec2 GetPosition() const { return mPos; }
    glm::vec2 GetSize() const { return mSize; }

    glm::vec2 GetBRectMin() const { return mPos; }
    glm::vec2 GetBRectMax() const { return mPos + mSize; }
    glm::vec2 GetBRectMinMax() const { return {mPos.x, mPos.y + mSize.y}; }
    glm::vec2 GetBRectMaxMin() const { return {mPos.x + mSize.x, mPos.y}; }

    bool DoesBRectContains(glm::vec2 const& pos) const;

    RectSide GetCollisionSide(CBRect const& rect) const;
  };
}