#include "stdafx.h"
#include "GUILabel.h"
#include "GUIRenderContext.h"

namespace gui {
  void CLabel::UpdateRender(CRenderContext const & ctx, glm::vec2 const & spaceSize) {
    mSize = ctx.Font.GetTextSize(mText, true);
    mTextAlignPos = GetAlignedPos(mSize, spaceSize, mTextAlign);
  }

  void CLabel::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    ctx.Canvas.Print(pos + mTextAlignPos, ctx.Font, mText, mTextColor);
  }
}
