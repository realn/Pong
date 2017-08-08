#include "stdafx.h"
#include "Game.h"
#include "Vertex.h"
#include "Consts.h"
#include "GamePaddle.h"
#include "GamePaddleController.h"
#include "GameBall.h"
#include "BRect.h"

const glm::vec2 FIELD_PROPS{5.0f, 3.0f};
const float FIELD_ASP_RATIO = FIELD_PROPS.x / FIELD_PROPS.y;

namespace pong {
  CGame::CGame(glm::vec2 const & screenSize) {
    auto screenAspRatio = screenSize.x / screenSize.y;
    if(screenAspRatio >= FIELD_ASP_RATIO) {
      mFieldSize = {FIELD_ASP_RATIO * screenSize.y, screenSize.y};
    }
    else {
      mFieldSize = {screenSize.x, screenSize.x / FIELD_ASP_RATIO};
    }
    mFieldPos = (screenSize - mFieldSize) / 2.0f;

    auto fieldMin = mFieldPos;
    auto fieldMax = mFieldPos + mFieldSize;
    auto color = glm::vec4{0.5f, 0.5f, 0.5f, 1.0f};
    mBuffer = std::make_unique<cb::gl::CBuffer>();
    mBuffer->SetData(std::vector<CVertex>{
      {fieldMin, 0.0f, 0.0f, color},
      {fieldMax.x, fieldMin.y, 1.0f, 0.0f, color},
      {fieldMax, 1.0f, 1.0f, color},
      {fieldMin.x, fieldMax.y, 0.0f, 1.0f, color},
    });

    {
      auto paddle = std::make_shared<CGamePaddle>(mFieldSize / glm::vec2(16.0f, 3.0f), 0.5f, 1.0f);
      paddle->SetPosition({
        mFieldSize.x / 16.0f,
        (mFieldSize.y - paddle->GetSize().y) / 2.0f
      });
      mPaddles.push_back(paddle);

      auto controller = std::make_shared<CGamePaddleMouseController>(paddle);
      mControllers.push_back(controller);
    }

    {
      auto paddle = std::make_shared<CGamePaddle>(mFieldSize / glm::vec2(16.0f, 3.0f), 0.5f, 1.0f);
      paddle->SetPosition({
        mFieldSize.x - (mFieldSize.x / 16.0f) - paddle->GetSize().x,
        (mFieldSize.y - paddle->GetSize().y) / 2.0f
      });
      mPaddles.push_back(paddle);

      auto controller = std::make_shared<CGamePaddleMouseController>(paddle);
      mControllers.push_back(controller);
    }

    mBall = std::make_unique<CGameBall>(glm::vec2{0.3f, 0.3f}, 1.0f);
    mBall->SetPosition((mFieldSize - mBall->GetSize()) / 2.0f);
  }

  CGame::~CGame() {}

  void CGame::Update(float const timeDelta) {
    for(auto& controller : mControllers) {
      controller->Update(*this, timeDelta);
    }
    for(auto& paddle : mPaddles) {
      paddle->Update(*this, timeDelta);
    }
    mBall->Update(*this, timeDelta);
  }

  void CGame::UpdateRender() {
    for(auto& paddle : mPaddles) {
      paddle->UpdateRender();
    }
    mBall->UpdateRender();
  }

  void CGame::Render(cb::gl::CProgram & glProgram, glm::mat4 const & transform) {
    glProgram.SetUniform(render::UNI_TRANSFORM, transform);

    auto gbuf = cb::gl::bind(*mBuffer);
    auto gver = cb::gl::bind(CVertex::Def);

    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, std::vector<glm::u16>{0, 1, 2, 0, 2, 3});

    for(auto& paddle : mPaddles) {
      paddle->Render(glProgram, transform);
    }
    mBall->Render(glProgram, transform);
  }

  void CGame::EventMousePos(glm::vec2 const & pos) {
    for(auto& controller : mControllers) {
      controller->EventMouseMove(pos / mFieldSize);
    }
  }
}
