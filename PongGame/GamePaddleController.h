#pragma once

#include <memory>

namespace pong {
  class CGame;
  class CGamePaddle;

  class CGamePaddleControllerBase{
  protected:
    std::shared_ptr<CGamePaddle> mPaddle;

  public:
    CGamePaddleControllerBase(std::shared_ptr<CGamePaddle> paddle);
    virtual ~CGamePaddleControllerBase();

    virtual void Update(CGame& game, float const timeDelta) = 0;
  };

  class CGamePaddleMouseController :
    public CGamePaddleControllerBase {
  private:
    glm::vec2 mMousePos;

  public:
    CGamePaddleMouseController(std::shared_ptr<CGamePaddle> paddle);
    virtual ~CGamePaddleMouseController();

    void Update(CGame& game, float const timeDelta) override;

    void EventMouseMove(glm::vec2 const& pos) { mMousePos = pos; }
  };
}