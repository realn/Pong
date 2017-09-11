#include "stdafx.h"
#include "GUIStackPanel.h"
#include "GUIContext.h"

namespace gui {
  void CStackPanel::AddWidget(std::unique_ptr<CWidget> widget, Align const align) {
    mItems.push_back({glm::vec2(), std::move(widget), align});
  }

  void CStackPanel::Update(float const timeDelta) {
    for(auto& item : mItems) {
      item.Widget->Update(timeDelta);
    }
  }

  void CStackPanel::UpdateWidget(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    auto ipos = glm::vec2();
    auto spaceLeft = spaceSize;
    auto ori = (mOrientation == Orientation::Horizontal) ? glm::vec2(1.0f, 0.0f) : glm::vec2(0.0f, 1.0f);
    auto nori = glm::vec2(1.0f) - ori;

    for(auto& item : mItems) {
      item.Widget->UpdateWidget(ctx, spaceLeft);

      auto itemSpaceSize = item.Widget->GetSize();
      spaceLeft -= itemSpaceSize * ori;
      itemSpaceSize = spaceLeft * nori + itemSpaceSize * ori;
      
      item.Pos = ipos + GetAlignedPos(item.Widget->GetSize(), itemSpaceSize, item.Align);
      ipos += itemSpaceSize * ori;
    }
  }

  void CStackPanel::UpdateRender(CRenderContext & ctx, glm::vec2 const & pos) const {
    for(auto& item : mItems) {
      item.Widget->UpdateRender(ctx, pos + item.Pos);
    }
  }

  CWidget * CStackPanel::FindWidgetById(cb::string const & id) {
    auto result = CWidgetListContainer::FindWidgetById(id);
    if(result) {
      return result;
    }
    return CWidget::FindWidgetById(id);
  }

  const CWidget * CStackPanel::FindWidgetById(cb::string const & id) const {
    auto result = CWidgetListContainer::FindWidgetById(id);
    if(result) {
      return result;
    }
    return CWidget::FindWidgetById(id);
  }
}
