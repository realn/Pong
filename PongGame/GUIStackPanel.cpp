#include "stdafx.h"
#include "GUIStackPanel.h"
#include "GUIRenderContext.h"

namespace gui {
  void CStackPanel::AddWidget(std::unique_ptr<CWidget> widget, Align const align) {
    mItems.push_back({glm::vec2(), std::move(widget), align});
  }

  void CStackPanel::Update(float const timeDelta) {
    for(auto& item : mItems) {
      item.Widget->Update(timeDelta);
    }
  }

  void CStackPanel::UpdateRender(CRenderContext const & ctx, glm::vec2 const & spaceSize) {
    auto ipos = glm::vec2();
    auto spaceLeft = spaceSize;
    auto ori = (mOrientation == Orientation::Horizontal) ? glm::vec2(1.0f, 0.0f) : glm::vec2(0.0f, 1.0f);
    auto nori = glm::vec2(1.0f) - ori;

    for(auto& item : mItems) {
      item.Widget->UpdateRender(ctx, spaceLeft);

      auto itemSpaceSize = item.Widget->GetSize();
      spaceLeft -= itemSpaceSize * ori;
      itemSpaceSize = spaceLeft * nori + itemSpaceSize * ori;
      
      item.Pos = ipos + GetAlignedPos(item.Widget->GetSize(), itemSpaceSize, item.Align);
      ipos += itemSpaceSize * ori;
    }
  }

  void CStackPanel::Render(CRenderContext & ctx, glm::vec2 const & pos) const {
    for(auto& item : mItems) {
      item.Widget->Render(ctx, pos + item.Pos);
    }
  }

  CWidget * CStackPanel::FindWidgetById(cb::string const & id) {
    for(auto& item : mItems) {
      auto result = item.Widget->FindWidgetById(id);
      if(result) {
        return result;
      }
    }
    return CWidget::FindWidgetById(id);
  }

  const CWidget * CStackPanel::FindWidgetById(cb::string const & id) const {
    for(auto& item : mItems) {
      auto result = item.Widget->FindWidgetById(id);
      if(result) {
        return result;
      }
    }
    return CWidget::FindWidgetById(id);
  }
}
