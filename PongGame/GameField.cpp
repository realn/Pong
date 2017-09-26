#include "stdafx.h"
#include "GameField.h"
#include "Consts.h"

#include <CBGL/Buffer.h>
#include <CBGL/Program.h>
#include <GFXVertex.h>
#include <GFXConsts.h>

namespace pong {
  CGameField::CGameField(glm::vec2 const & pos, glm::vec2 const & size, glm::vec4 const & color) 
    : CGameObject(size, 0.0f)
  {
    mPos = pos;

    auto fieldMin = mPos;
    auto fieldMax = mPos + mSize;
    mBuffer = std::make_unique<cb::gl::CBuffer>();
    mBuffer->SetData(std::vector<gfx::CVertex>{
      { fieldMin, 0.0f, 0.0f, color },
      {fieldMax.x, fieldMin.y, 1.0f, 0.0f, color},
      {fieldMax, 1.0f, 1.0f, color},
      {fieldMin.x, fieldMax.y, 0.0f, 1.0f, color},
    });
  }

  CGameField::~CGameField() {}

  void CGameField::Update(CGame & game, float const timeDelta) {}

  void CGameField::UpdateRender() {}

  void CGameField::Render(cb::gl::CProgram & glProgram, glm::mat4 const & transform) const {
    glProgram.SetUniform(gfx::UNI_TRANSFORM, transform);

    auto gbuf = cb::gl::bind(*mBuffer);
    auto gver = cb::gl::bind(gfx::CVertex::Def);

    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, std::vector<glm::u16>{0, 1, 2, 0, 2, 3});
  }
}
