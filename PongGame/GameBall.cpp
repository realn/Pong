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
  CGameBall::CGameBall()
    : CGameMovingObject() {
  }

  CGameBall::~CGameBall() {}

  void CGameBall::SetField(std::shared_ptr<CGameField> field) {
    mField = field;
  }

  std::shared_ptr<CGameField> CGameBall::GetField() const {
    return mField;
  }

  void CGameBall::Update(float const timeDelta) {
    if(mVec != glm::zero<glm::vec2>()) {
      mVec = glm::normalize(mVec);
      mPos += mVec * mSpeed * timeDelta;
    }

    auto ballRect = GetBRect();
    auto fieldRect = mField->GetBRect();

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

    //auto& paddles = game.GetPaddles();
    //for(auto& paddle : paddles) {
    //  auto paddleRect = paddle->GetBRect();

    //  if(paddleRect.Contains(ballRect)) {
    //    mVec = glm::normalize(fieldRect.GetCenter() - GetPosition());
    //  }
    //  else {
    //    auto edge = paddleRect.ContainsEdgeOf(ballRect);
    //    if(edge != core::RectEdge::None) {
    //      auto norm = core::CBRect::GetNormal(edge);
    //      mVec = (glm::vec2(1.0f) - glm::abs(norm)) * mVec + -(norm * glm::abs(mVec));
    //    }
    //    else {
    //      auto corner = paddleRect.ContainsCornerOf(ballRect);
    //      if(corner != core::RectCorner::None) {
    //        mVec = -core::CBRect::GetNormal(corner) * glm::abs(mVec);
    //      }
    //    }
    //  }
    //}
  }

  void CGameBall::UpdateRender(gfx::CCanvas& canvas) {
    canvas.DrawRect(GetBRect(), mColor);
  }
}
