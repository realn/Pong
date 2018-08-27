#pragma once

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <CBGL/Fwd.h>

#include <CoreEvents.h>
#include <CoreApp.h>
#include <CoreInputObserver.h>
#include <GFXFwd.h>

#include "ControlEventObservers.h"

namespace pong {
  enum class PaddleSide;
  enum class GameControllerType;

  class CAssets;
  class CGameField;
  class CGamePaddle;
  class CGameBall;
  class CGameObject;

  class IGameController;

  class CGame
    : public core::CApp
    , public core::IEventTarget<core::IInputObserver>
  {

  private:
    glm::vec2 mScreenSize;
    std::unique_ptr<CAssets> mAssets;

  public:
    CGame(cb::strvector const& args);
    CGame(CGame&&) = default;
    ~CGame();

    bool OnAdjustConfig(core::CAppConfig& config) override;
    bool OnInit() override;

    std::unique_ptr<core::CScene> OnCreateScene() override;

    virtual void OnAppClose() override;

    virtual void OnInputAction(core::InputEventId const id) override;;
    virtual void OnInputState(core::InputEventId const id, bool value) override;
    virtual void OnInputRange(core::InputEventId const id, float const value, float const prevValue) override;
  };
}
