#include "stdafx.h"
#include "GUIPanel.h"

namespace gui {
  CWidget * CPanel::FindWidgetById(cb::string const & id) {
    if(mContent) {
      auto result = mContent->FindWidgetById(id);
      if(result) {
        return result;
      }
    }
    return CWidget::FindWidgetById(id);
  }

  const CWidget * CPanel::FindWidgetById(cb::string const & id) const {
    if(mContent) {
      auto result = mContent->FindWidgetById(id);
      if(result) {
        return result;
      }
    }
    return CWidget::FindWidgetById(id);
  }

  void CPanel::Update(float const timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CPanel::UpdateWidget(CRenderContext const & ctx, glm::vec2 const & spaceSize) {
    CRect::UpdateWidget(ctx, spaceSize);

    if(mContent) {
      auto marginSize = GetMarginSize(spaceSize, mContentMargin);
      mContent->UpdateWidget(ctx, marginSize);
      mContentPos = GetMarginPos(mContentMargin) + 
        GetAlignedPos(mContent->GetSize(), marginSize, mContentAlign);
    }
  }

  void CPanel::UpdateRender(CRenderContext & ctx, glm::vec2 const & pos) const {
    CRect::UpdateRender(ctx, pos);

    if(mContent) {
      mContent->UpdateRender(ctx, pos + mContentPos);
    }
  }
}
