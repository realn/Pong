#pragma once

#include <CBGL/Fwd.h>
#include <glm/vec2.hpp>

#include <CoreFwd.h>

namespace pong {
  class CGame;

  class CGameObject {
  protected:
    glm::vec2 mSize;
    glm::vec2 mPos;
    glm::vec2 mVec;
    float mSpeed;

  public:
    CGameObject(glm::vec2 const& size, float const speed)
      : mSize(size), mSpeed(speed) {}
    virtual ~CGameObject() {}

    void SetPosition(glm::vec2 const& value) { mPos = value; }
    void SetVector(glm::vec2 const& value) { mVec = value; }
    void SetSpeed(float const speed) { mSpeed = speed; }

    glm::vec2 GetPosition() const { return mPos; }
    glm::vec2 GetVector() const { return mVec; }
    glm::vec2 GetSize() const { return mSize; }
    glm::vec2 GetCenterPos() const { return mPos + (mSize / 2.0f); }
    float GetSpeed() const { return mSpeed; }

    core::CBRect GetBRect() const;

    virtual void Update(CGame& game, float const timeDelta) = 0;
    virtual void UpdateRender() = 0;
    virtual void Render(cb::gl::CProgram& glProgram, glm::mat4 const& transform) const = 0;
  };
}