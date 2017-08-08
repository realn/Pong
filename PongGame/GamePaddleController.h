#pragma once

namespace pong {
  enum class PaddleControllerType {
    Mouse, Keyboard, AI,
  };

  class CGame;

  class IGamePaddleController {
  public:
    virtual ~IGamePaddleController() {}

    virtual void Update(CGame& game, float const timeDelta) = 0;
  };
}