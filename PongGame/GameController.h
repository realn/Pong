#pragma once

#include <memory>

namespace pong {
  enum class GameControllerType {
    Mouse, Keyboard, AI,
  };

  class CGame;
  class CGameObject;

  class IGameController {
  public:

    virtual ~IGameController() {}

    virtual bool InitController(CGame& game) = 0;
    virtual void Update(CGame& game, float const timeDelta) = 0;

    static std::shared_ptr<IGameController> Create(GameControllerType const type, 
                                                   std::shared_ptr<CGameObject> target);
  };
}
