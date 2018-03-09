#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "GamePaddleController.h"
#include "ControlEventObservers.h"

namespace pong {
  class CGame;
  class CGamePaddle;

  class CGamePaddleControllerBase 
    : public IGameController
  {
  protected:
    std::shared_ptr<CGamePaddle> mPaddle;

  public:
    CGamePaddleControllerBase(std::shared_ptr<CGameObject> paddle);
    virtual ~CGamePaddleControllerBase();
  };

  class CGamePaddleMouseController 
    : public CGamePaddleControllerBase 
    , public IMouseEventObserver
    , public std::enable_shared_from_this<CGamePaddleMouseController>
  {
  private:
    glm::vec2 mMousePos;

  public:
    CGamePaddleMouseController(std::shared_ptr<CGameObject> paddle);
    virtual ~CGamePaddleMouseController();

    bool InitController(CGame& game) override;
    void Update(CGame& game, float const timeDelta) override;

    void EventMouseMove(glm::vec2 const& pos) override { mMousePos = pos; }
  };

  class CGamePaddleKeyboardController
    : public CGamePaddleControllerBase
    , public IKeyboardEventObserver 
    , public std::enable_shared_from_this<CGamePaddleKeyboardController>
  {
  private:
    bool mMoveUp;
    bool mMoveDown;

  public:
    CGamePaddleKeyboardController(std::shared_ptr<CGameObject> paddle);
    virtual ~CGamePaddleKeyboardController();

    bool InitController(CGame& game) override;
    void Update(CGame& game, float const timeDelta) override;

    void EventKeyPress(cb::sdl::ScanCode key, cb::sdl::KeyState state) override;
  };
}
