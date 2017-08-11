#pragma once

#include <glm/vec4.hpp>

#include <CBSDL/Defines.h>

namespace pong {
  class CGamePlayer {
  private:
    cb::u32 mPoints;
    cb::u32 mDeaths;
    glm::vec4 mColor;

  public:
    CGamePlayer(glm::vec4 const& color) : mPoints(0), mDeaths(0), mColor(color) {}

    void AddPoint() { mPoints++; }
    void AddDeath() { mDeaths++; }

    cb::u32 GetPoints() const { return mPoints; }
    cb::u32 GetDeaths() const { return mDeaths; }
    glm::vec4 GetColor() const { return mColor; }
  };
}