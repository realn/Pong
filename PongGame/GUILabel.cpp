#include "stdafx.h"
#include "GUILabel.h"
#include "GUIContext.h"
#include "GFXCanvas.h"
#include "Font.h"
#include "Consts.h"

namespace gui {
  void CLabel::UpdateWidget(CUpdateContext const& ctx, glm::vec2 const & spaceSize) {
    mSize = ctx.Font.GetTextSize(mText, true) * glm::vec2(TEXT_SCALE) * mTextScale;
    mTextAlignPos = GetAlignedPos(mSize, spaceSize, mTextAlign);
  }

  void CLabel::UpdateRender(CRenderContext & ctx, glm::vec2 const & pos) const {
    ctx.Canvas.Print(pos + mTextAlignPos, 
                     ctx.Font, mText, mTextColor, glm::vec2(TEXT_SCALE) * mTextScale);
  }
}
