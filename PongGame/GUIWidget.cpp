#include "stdafx.h"
#include "GUIWidget.h"

namespace gui {
  cb::string CWidget::NoId = cb::string();

  void CWidget::UpdateWidget(CUpdateContext const & ctx, glm::vec2 const & spaceSize) {
    if(glm::all(glm::equal(mFixedSize, glm::vec2(0.0f)))) {
      mSize = spaceSize;
    }
    else {
      mSize = glm::clamp(mFixedSize, glm::vec2(0.0f), spaceSize);
    }
  }

  glm::vec2 CWidget::GetAlignedPos(glm::vec2 const & size, glm::vec2 const & spaceSize, Align const align) {
    auto pos = glm::vec2();

    if(isTrueAnd(align, Align::Center)) { 
      pos.x = (spaceSize.x - size.x) / 2.0f; 
    }
    else if(isTrueAnd(align, Align::Right)) { 
      pos.x = spaceSize.x - size.x; 
    }
    else { 
      pos.x = 0.0f; 
    }

    if(isTrueAnd(align, Align::Middle)) { 
      pos.y = (spaceSize.y - size.y) / 2.0f; 
    }
    else if(isTrueAnd(align, Align::Top)) { 
      pos.y = spaceSize.y - size.y; 
    }
    else { 
      pos.y = 0.0f; 
    }

    return pos;
  }

  glm::vec2 CWidget::GetMarginPos(glm::vec4 const & margin) {
    return glm::vec2(margin.x, margin.y);
  }

  glm::vec2 CWidget::GetMarginSize(glm::vec2 const& size, glm::vec4 const & margin) {
    return size - glm::vec2(margin.x + margin.z, margin.y + margin.w);
  }
}
