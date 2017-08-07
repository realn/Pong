#include "stdafx.h"
#include "GameBall.h"
#include "GamePaddle.h"
#include "Game.h"
#include "Vertex.h"
#include "Consts.h"

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
    mPos += mVec * mSpeed * timeDelta;

    if(mPos.y < 0.0f || mPos.y + mSize.y > game.GetFieldSize().y) {
      mVec.y = -mVec.y;
    }
    if(mPos.x < 0.0f || mPos.x + mSize.x > game.GetFieldSize().x) {
      mVec.x = -mVec.x;
    }

    if(game.GetPaddle().DoesBRectContains(GetBRectMin()) ||
       game.GetPaddle().DoesBRectContains(GetBRectMinMax())) {
      mVec.x = -mVec.x;
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
