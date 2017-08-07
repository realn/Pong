#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include <CBGL/Fwd.h>

namespace pong {
  class CGamePaddle;
  class CGamePaddleMouseController;
  class CGameBall;

  class CGame {
  private:
    glm::vec2 mFieldPos;
    glm::vec2 mFieldSize;
    std::unique_ptr<cb::gl::CBuffer> mBuffer;
    std::shared_ptr<CGamePaddle> mPaddle;
    std::unique_ptr<CGamePaddleMouseController> mController;
    std::unique_ptr<CGameBall> mBall;

  public:
    CGame(glm::vec2 const& screenSize);
    ~CGame();

    glm::vec2 GetFieldSize() const { return mFieldSize; }

    const CGamePaddle& GetPaddle() const { return *mPaddle; }

    void Update(float const timeDelta);
    void UpdateRender();
    void Render(cb::gl::CProgram& glProgram, glm::mat4 const& transform);

    void EventMousePos(glm::vec2 const& pos);
  };
}