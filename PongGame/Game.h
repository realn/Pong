#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <CBGL/Fwd.h>
#include "GamePaddle.h"

namespace pong {
  class CGame {
  private:
    glm::vec2 mFieldSize;
    CGamePaddle mPlayer1;

  public:
    CGame(glm::vec2 const& fieldSize);
    ~CGame();

    glm::vec2 GetFieldSize() const { return mFieldSize; }

    void Update(float const timeDelta);
    void UpdateRender();
  };
}