#pragma once

#include <CBGL/Fwd.h>
#include <glm/vec2.hpp>

#include <CoreFwd.h>
#include <GFXFwd.h>

namespace pong {
  class CGameObject {
  protected:
    glm::vec2 mPos;
    glm::vec2 mSize;
    glm::vec4 mColor;

  public:
    CGameObject() {}
    virtual ~CGameObject() {}

    void SetPosition(glm::vec2 const& value) { mPos = value; }
    void SetSize(const glm::vec2& value) { mSize = value; }
    void SetColor(const glm::vec4& value) { mColor = value; }

    glm::vec2 GetPosition() const { return mPos; }
    glm::vec2 GetSize() const { return mSize; }
    glm::vec4 GetColor() const { return mColor; }

    glm::vec2 GetCenterPos() const { return mPos + (mSize / 2.0f); }
    core::CBRect GetBRect() const;

    virtual void Update(float const timeDelta) = 0;
    virtual void UpdateRender(gfx::CCanvas& canvas) = 0;
  };
}