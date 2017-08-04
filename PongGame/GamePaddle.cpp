#include "stdafx.h"
#include "GamePaddle.h"
#include "Game.h"
#include "Vertex.h"
#include "Consts.h"

namespace pong {
  CGamePaddle::CGamePaddle(glm::vec2 const & size, 
                           glm::vec2 const & pos, 
                           float const speed, 
                           float const accel) :
    CGameObject(size, pos, speed) {
    
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
    if(mTargetPos != mPos) {
      mVec = glm::normalize(mTargetPos - mPos);
    }
    else {
      mVec = glm::vec2(0.0f);
    }

    mPos += mVec * mSpeed * timeDelta;
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
