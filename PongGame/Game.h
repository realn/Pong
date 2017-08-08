#pragma once

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <CBGL/Fwd.h>

namespace pong {
  class CGamePaddle;
  class CGamePaddleMouseController;
  class CGameBall;

  class CGame {
  public:
    using PaddlePtrT = std::shared_ptr<CGamePaddle>;
    using PaddlePtrVecT = std::vector<PaddlePtrT>;
    using PaddleControllerPtrT = std::shared_ptr<CGamePaddleMouseController>;
    using PaddleControllerPtrVecT = std::vector<PaddleControllerPtrT>;

  private:
    glm::vec2 mFieldPos;
    glm::vec2 mFieldSize;
    std::unique_ptr<cb::gl::CBuffer> mBuffer;
    PaddlePtrVecT mPaddles;
    PaddleControllerPtrVecT mControllers;
    std::unique_ptr<CGameBall> mBall;

  public:
    CGame(glm::vec2 const& screenSize);
    ~CGame();

    glm::vec2 GetFieldSize() const { return mFieldSize; }

    const PaddlePtrVecT& GetPaddles() const { return mPaddles; }

    void Update(float const timeDelta);
    void UpdateRender();
    void Render(cb::gl::CProgram& glProgram, glm::mat4 const& transform);

    void EventMousePos(glm::vec2 const& pos);
  };
}