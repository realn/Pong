#include "stdafx.h"
#include "GamePaddle.h"
#include "GameField.h"
#include "Game.h"
#include "Vertex.h"
#include "Consts.h"

namespace pong {
  CGamePaddle::CGamePaddle(PaddleSide const side,
                           glm::vec2 const & size, 
                           float const speed, 
                           float const accel) 
    : CGameObject(size, speed) 
    , mMoveDir(PaddleMoveDir::None)
    , mSide(side)
    , mAccel(accel)
  {
    
    glm::vec4 color = {1.0f, 0.0f, 0.5f, 1.0f};
    auto verts = std::vector<CVertex>{
      {0.0f, 0.0f, 0.0f, 0.0f, color},
      {size.x, 0.0f, 1.0f, 0.0f, color},
      {size.x, size.y, 1.0f, 1.0f, color},
      {0.0f, size.y, 0.0f, 1.0f, color},
    };

    mBuffer = std::make_unique<cb::gl::CBuffer>();
    mBuffer->SetData(verts);
  }

  void CGamePaddle::Update(CGame& game, float const timeDelta) {
    auto dir = glm::vec2();
    switch(mMoveDir) {
    case PaddleMoveDir::Up:     dir = {0.0f, 1.0f}; break;
    case PaddleMoveDir::Down:   dir = {0.0f, -1.0f}; break;
    case PaddleMoveDir::Left:   dir = {-1.0f, 0.0f}; break;
    case PaddleMoveDir::Right:  dir = {1.0f, 0.0f}; break;
    default:  dir = -glm::sign(mVec); break;
    }

    mVec = glm::clamp(mVec + dir * mAccel * timeDelta, -1.0f, 1.0f);

    mPos += mVec * mSpeed * timeDelta;

    if(mPos.y < 0.0f || mPos.y + mSize.y > game.GetField().GetSize().y) {
      mPos.y = glm::clamp(mPos.y, 0.0f, game.GetField().GetSize().y - mSize.y);
      mVec = glm::vec2(0.0f);
    }
  }

  void CGamePaddle::UpdateRender() {}

  void CGamePaddle::Render(cb::gl::CProgram & glProgram, glm::mat4 const& transform) const {
    glProgram.SetUniform(render::UNI_TRANSFORM, 
                         transform * glm::translate(glm::mat4(1.0f), {mPos, 0.0f}));

    auto gbuf = cb::gl::bind(*mBuffer);
    auto gver = cb::gl::bind(CVertex::Def);

    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, std::vector<glm::u16>{0, 1, 2, 0, 2, 3});
  }
}
