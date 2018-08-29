#pragma once

#include "GameObject.h"

namespace pong {
  class CGameMovingObject
    : public CGameObject
  {
  protected:
    glm::vec2 mVec;
    float mSpeed = 0.0f;

  public:
    CGameMovingObject() {}
    ~CGameMovingObject() {}

    void SetVector(glm::vec2 const& value) { mVec = value; }
    void SetSpeed(float const speed) { mSpeed = speed; }

    glm::vec2 GetVector() const { return mVec; }
    float GetSpeed() const { return mSpeed; }
  };
}