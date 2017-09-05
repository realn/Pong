#include "stdafx.h"
#include "GUIWidget.h"

namespace gui {
  cb::string CWidget::NoId = cb::string();

  glm::vec2 CWidget::GetAlignedPos(glm::vec2 const & size, glm::vec2 const & spaceSize, Align const align) {
    auto pos = glm::vec2();

    if(static_cast<bool>(align & Align::Center)) { pos.x = (spaceSize.x - size.x) / 2.0f; }
    else if(static_cast<bool>(align & Align::Right)) { pos.x = spaceSize.x - size.x; }
    else { pos.x = 0.0f; }

    if(static_cast<bool>(align & Align::Middle)) { pos.y = (spaceSize.y - size.y) / 2.0f; }
    else if(static_cast<bool>(align & Align::Bottom)) { pos.y = spaceSize.y - size.y; }
    else { pos.y = 0.0f; }

    return pos;
  }

  glm::vec2 CWidget::GetMarginPos(glm::vec4 const & margin) {
    return glm::vec2(margin.x, margin.y);
  }

  glm::vec2 CWidget::GetMarginSize(glm::vec2 const& size, glm::vec4 const & margin) {
    return size - glm::vec2(margin.x + margin.z, margin.y + margin.w);
  }
}
