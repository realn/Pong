#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include <CoreEvents.h>
#include <CoreInputObserver.h>

#include "GameConsts.h"
#include "GameSceneProcess.h"

namespace pong {
  class CGame;
  class CGamePaddle;

  class CGamePaddleLocalControllerConfig {
  public:
    GameInput::EventIdT mLeftId;
    GameInput::EventIdT mRightId;
    GameInput::EventIdT mUpId;
    GameInput::EventIdT mDownId;
  };

  class CGamePaddleLocalController
    : public CGameSceneProcess
    , public core::IEventTarget<core::IInputObserver>
  {
  private:
    std::shared_ptr<CGamePaddle> mPaddle;
    CGamePaddleLocalControllerConfig mConfig;
    bool mMoveUp;
    bool mMoveDown;

  public:
    CGamePaddleLocalController(std::shared_ptr<CGamePaddle> paddle,
                               const CGamePaddleLocalControllerConfig& config);
    virtual ~CGamePaddleLocalController();

    bool Init(CGame& game);

    void Update(const float timeDelta) override;

    virtual void OnInputState(core::InputEventId const id, bool value) override;
  };
}
