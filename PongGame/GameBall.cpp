#include "stdafx.h"

#include <CoreBRect.h>
#include <GFXConsts.h>
#include <GFXCanvas.h>

#include "GamePaddle.h"
#include "Game.h"
#include "Consts.h"
#include "GameField.h"
#include "GameBall.h"

namespace pong {
  CGameBall::CGameBall(glm::vec2 const & size, float const speed) 
    : CGameObject(size, speed)
    , mColor(0.5f, 1.0f, 0.0f, 1.0f)
  {
    mVec = glm::normalize(glm::vec2{2.0f, 1.0f});
  }

  CGameBall::~CGameBall() {}

  void CGameBall::Update(CGame & game, float const timeDelta) {
    mVec = glm::normalize(mVec);
    mPos += mVec * mSpeed * timeDelta;

    auto ballRect = GetBRect();
    auto fieldRect = game.GetField().GetBRect();

    if(!fieldRect.Contains(ballRect)) {
      auto edge = fieldRect.ContainsEdgeOf(ballRect);
      if(edge != core::RectEdge::None) {
        auto norm = core::CBRect::GetNormal(edge);
        mVec = (glm::vec2(1.0f) - glm::abs(norm)) * mVec + norm * glm::abs(mVec);
      }
      else {
        auto corner = fieldRect.ContainsCornerOf(ballRect);
        if(corner != core::RectCorner::None) {
          mVec = core::CBRect::GetNormal(corner) * glm::abs(mVec);
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
        if(edge != core::RectEdge::None) {
          auto norm = core::CBRect::GetNormal(edge);
          mVec = (glm::vec2(1.0f) - glm::abs(norm)) * mVec + -(norm * glm::abs(mVec));
        }
        else {
          auto corner = paddleRect.ContainsCornerOf(ballRect);
          if(corner != core::RectCorner::None) {
            mVec = -core::CBRect::GetNormal(corner) * glm::abs(mVec);
          }
        }
      }
    }
  }

  void CGameBall::UpdateRender(gfx::CCanvas& canvas) {
    canvas.DrawRect(GetBRect(), mColor);
  }
}
