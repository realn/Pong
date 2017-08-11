#pragma once

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <CBGL/Fwd.h>

namespace pong {
  enum class PaddleSide;
  enum class PaddleControllerType;

  class CGameField;
  class CGamePaddle;
  class CGameBall;

  class IGamePaddleController;
  class IMouseEventObserver;
  class IKeyboardEventObserver;

  class CGame {
  public:
    using PaddlePtrT = std::shared_ptr<CGamePaddle>;
    using PaddlePtrVecT = std::vector<PaddlePtrT>;
    using PaddleControllerPtrT = std::shared_ptr<IGamePaddleController>;
    using PaddleControllerPtrVecT = std::vector<PaddleControllerPtrT>;
    using MouseEventObserverPtrT = std::shared_ptr<IMouseEventObserver>;
    using MouseEventObserverPtrVecT = std::vector<MouseEventObserverPtrT>;
    using KeyboardEventObserverPtrT = std::shared_ptr<IKeyboardEventObserver>;
    using KeyboardEventObserverPtrVecT = std::vector<KeyboardEventObserverPtrT>;

  private:
    PaddlePtrVecT mPaddles;
    PaddleControllerPtrVecT mControllers;
    MouseEventObserverPtrVecT mMouseEventObservers;
    KeyboardEventObserverPtrVecT mKeyboardEventObservers;
    std::unique_ptr<CGameField> mField;
    std::unique_ptr<CGameBall> mBall;

  public:
    CGame(glm::vec2 const& screenSize);
    ~CGame();

    const PaddlePtrVecT& GetPaddles() const { return mPaddles; }
    const CGameField& GetField() const { return *mField; }

    void AddPlayer(PaddleControllerType controllerType);

    void Update(float const timeDelta);
    void UpdateRender();
    void Render(cb::gl::CProgram& glProgram, glm::mat4 const& transform);

    void EventMousePos(glm::vec2 const& pos);
    void EventKeyPress(cb::sdl::ScanCode code, cb::sdl::KeyState state);

  private:
    PaddleSide GetPlayerPaddleSide(cb::u32 const index) const;
    glm::vec2 GetPaddleSize(PaddleSide const side) const;
    glm::vec2 GetPaddleStartPos(CGamePaddle const& paddle, PaddleSide const side) const;
    void AddController(std::shared_ptr<CGamePaddle> paddle, PaddleControllerType const type);
  };
}