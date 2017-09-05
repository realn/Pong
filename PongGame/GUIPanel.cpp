#include "stdafx.h"
#include "GUIPanel.h"

namespace gui {
  void CPanel::UpdateRender(CRenderContext const & ctx, glm::vec2 const & spaceSize) {
    CRect::UpdateRender(ctx, spaceSize);

    if(mContent) {
      auto marginSize = GetMarginSize(spaceSize, mContentMargin);
      mContent->UpdateRender(ctx, marginSize);
      mContentPos = GetMarginPos(mContentMargin) + 
        GetAlignedPos(mContent->GetSize(), marginSize, mContentAlign);
    }
  }

  void CPanel::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    CRect::Render(ctx, pos);

    if(mContent) {
      mContent->Render(ctx, pos + mContentPos);
    }
  }
}
