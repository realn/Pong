#include "stdafx.h"
#include "GUIRect.h"
#include "GUIRenderContext.h"

namespace gui {
  void CRect::UpdateRender(CRenderContext & ctx, glm::vec2 const & pos) const {
    if(mBackColor.a != 0.0f) {
      ctx.Canvas.DrawRect(pos, mSize, mBackColor);
    }
  }
}
