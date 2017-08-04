#include "stdafx.h"
#include "Game.h"

namespace pong {
  CGame::CGame(glm::vec2 const & fieldSize) 
    : mFieldSize(fieldSize)
    , mPlayer1({fieldSize.x / 8.0f, fieldSize.y / 3.0f}, {0.0f, 0.0f}, 0.5f, 0.0f)
  {}

  CGame::~CGame() {}

  void CGame::Update(float const timeDelta) {
    mPlayer1.Update(*this, timeDelta);
  }

}
