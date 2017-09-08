#include "stdafx.h"
#include "GUIScreen.h"
#include "GUIWidget.h"
#include "GUIRenderContext.h"

namespace gui {
  CScreen::CScreen(glm::vec2 const & size, glm::vec4 const& contentMargin, Align const contentAlign) 
    : CWidgetContainer(contentMargin, contentAlign), mSize(size) {}

  CScreen::~CScreen() {}

  void CScreen::Update(float timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CScreen::UpdateRender(gfx::CFont const& font, gfx::CCanvas& canvas) {
    if(mContent) {
      auto ctx = CRenderContext{canvas, font};

      mContent->UpdateWidget(ctx, mSize);
      mContent->UpdateRender(ctx, {0.0f, 0.0f});
    }
  }
}
