#pragma once

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <CBGL/Fwd.h>

#include <CoreEvents.h>
#include <CoreInputEvents.h>
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
    : public core::IEventSource<IMouseEventObserver>
    , public core::IEventSource<IKeyboardEventObserver>
    , public core::IEventTarget<core::IInputKeyEvents>
    , public core::IEventTarget<core::IInputMouseEvents>
  {
  public:
    using PaddlePtrT = std::shared_ptr<CGamePaddle>;
    using PaddlePtrVecT = std::vector<PaddlePtrT>;
    using PaddleControllerPtrT = std::shared_ptr<IGameController>;
    using PaddleControllerPtrVecT = std::vector<PaddleControllerPtrT>;

  private:
    PaddlePtrVecT mPaddles;
    PaddleControllerPtrVecT mControllers;
    std::unique_ptr<CGameField> mField;
    std::unique_ptr<CGameBall> mBall;
    std::unique_ptr<gfx::CCanvas> mCanvas;
    std::unique_ptr<gfx::CCanvasView> mCanvasView;

  public:
    CGame(glm::vec2 const& screenSize, CAssets& assets);
    ~CGame();

    const PaddlePtrVecT& GetPaddles() const { return mPaddles; }
    const CGameField& GetField() const { return *mField; }

    void AddPlayer(GameControllerType controllerType);

    void Update(float const timeDelta);
    void UpdateRender();
    void Render(glm::mat4 const& transform);

    // Inherited via IEventTarget
    virtual void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) override;

    // Inherited via IEventTarget
    virtual void OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) override;
    virtual void OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) override;

  private:
    PaddleSide GetPlayerPaddleSide(cb::u32 const index) const;
    glm::vec2 GetPaddleSize(PaddleSide const side) const;
    glm::vec2 GetPaddleStartPos(CGamePaddle const& paddle, PaddleSide const side) const;
    void AddController(std::shared_ptr<CGameObject> target, GameControllerType const type);

  };
}
