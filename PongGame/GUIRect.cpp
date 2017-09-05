#include "stdafx.h"
#include "GUIRect.h"
#include "GUIRenderContext.h"

namespace gui {
  void CRect::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    ctx.Canvas.DrawRect(pos, mSize, mBackColor);
  }
}
