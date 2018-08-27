#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include <CoreEvents.h>
#include <CoreInputObserver.h>

#include "GameConsts.h"
#include "GameController.h"

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

  //class CGamePaddleMouseController 
  //  : public CGamePaddleControllerBase 
  //  //, public core::IEventTarget<IMouseEventObserver>
  //{
  //private:
  //  glm::vec2 mMousePos;

  //public:
  //  CGamePaddleMouseController(std::shared_ptr<CGameObject> paddle);
  //  virtual ~CGamePaddleMouseController();

  //  bool InitController(CGame& game) override;
  //  void Update(CGame& game, float const timeDelta) override;

  //  //void EventMouseMove(glm::vec2 const& pos) override { mMousePos = pos; }
  //};

  class CGamePaddleLocalControllerConfig {
  public:
    GameInput::EventIdT mLeftId;
    GameInput::EventIdT mRightId;
    GameInput::EventIdT mUpId;
    GameInput::EventIdT mDownId;
  };

  class CGamePaddleLocalController
    : public CGamePaddleControllerBase
    , public core::IEventTarget<core::IInputObserver>
  {
  private:
    CGamePaddleLocalControllerConfig mConfig;
    bool mMoveUp;
    bool mMoveDown;

  public:
    CGamePaddleLocalController(std::shared_ptr<CGameObject> paddle,
                               const CGamePaddleLocalControllerConfig& config);
    virtual ~CGamePaddleLocalController();

    bool InitController(CGame& game) override;
    void Update(CGame& game, float const timeDelta) override;

    virtual void OnInputState(core::InputEventId const id, bool value) override;
    //void EventKeyPress(cb::sdl::ScanCode key, cb::sdl::KeyState state) override;
  };
}
