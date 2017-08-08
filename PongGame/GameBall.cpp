#include "stdafx.h"
#include "GameBall.h"
#include "GamePaddle.h"
#include "Game.h"
#include "Vertex.h"
#include "Consts.h"
#include "BRect.h"

namespace pong {
  CGameBall::CGameBall(glm::vec2 const & size, float const speed) 
    : CGameObject(size, speed)
  {
    glm::vec4 color = {0.5f, 1.0f, 0.0f, 1.0f};
    auto verts = std::vector<CVertex>{
      {0.0f, 0.0f, 0.0f, 0.0f, color},
      {size.x, 0.0f, 1.0f, 0.0f, color},
      {size.x, size.y, 1.0f, 1.0f, color},
      {0.0f, size.y, 0.0f, 1.0f, color},
    };

    mBuffer = std::make_unique<cb::gl::CBuffer>();
    mBuffer->SetData(verts);

    mVec = glm::normalize(glm::vec2{2.0f, 1.0f});
  }

  CGameBall::~CGameBall() {}

  void CGameBall::Update(CGame & game, float const timeDelta) {
    mVec = glm::normalize(mVec);
    mPos += mVec * mSpeed * timeDelta;

    auto ballRect = GetBRect();
    auto fieldRect = CBRect({0.0f, 0.0f}, game.GetFieldSize());

    if(!fieldRect.Contains(ballRect)) {
      auto edge = fieldRect.ContainsEdgeOf(ballRect);
      if(edge != RectEdge::None) {
        auto norm = CBRect::GetNormal(edge);
        mVec = (glm::vec2(1.0f) - glm::abs(norm)) * mVec + norm * glm::abs(mVec);
      }
      else {
        auto corner = fieldRect.ContainsCornerOf(ballRect);
        if(corner != RectCorner::None) {
          mVec = CBRect::GetNormal(corner) * glm::abs(mVec);
        }
      }
    }

    auto& paddles = game.GetPaddles();
    for(auto& paddle : paddles) {
      auto paddleRect = paddle->GetBRect();

      if(paddleRect.Contains(ballRect)) {
        mVec = glm::normalize(fieldRect.GetCenter() - GetPosition());
      }
      else {
        auto edge = paddleRect.ContainsEdgeOf(ballRect);
        if(edge != RectEdge::None) {
          auto norm = CBRect::GetNormal(edge);
          mVec = (glm::vec2(1.0f) - glm::abs(norm)) * mVec + -(norm * glm::abs(mVec));
        }
        else {
          auto corner = paddleRect.ContainsCornerOf(ballRect);
          if(corner != RectCorner::None) {
            mVec = -CBRect::GetNormal(corner) * glm::abs(mVec);
          }
        }
      }
    }
  }

  void CGameBall::UpdateRender() {}

  void CGameBall::Render(cb::gl::CProgram & glProgram, glm::mat4 const & transform) const {
    glProgram.SetUniform(render::UNI_TRANSFORM,
                         transform * glm::translate(glm::mat4(1.0f), {mPos, 0.0f}));

    auto gbuf = cb::gl::bind(*mBuffer);
    auto gver = cb::gl::bind(CVertex::Def);

    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, std::vector<glm::u16>{0, 1, 2, 0, 2, 3});
  }
}
