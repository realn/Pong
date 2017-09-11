#include "stdafx.h"
#include "GUIScreen.h"
#include "GUIWidget.h"
#include "GUIContext.h"
#include "GFXCanvas.h"

namespace gui {
  CScreen::CScreen(glm::vec2 const & size, glm::vec4 const& contentMargin, Align const contentAlign) 
    : CWidgetContainer(contentMargin, contentAlign), mSize(size) {}

  CScreen::~CScreen() {}

  void CScreen::Update(float timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CScreen::UpdateRender(gfx::CCanvas& canvas, gfx::CFont const& font) {
    if(mContent) {
      {
        auto ctx = CUpdateContext{font};
        mContent->UpdateWidget(ctx, mSize);
      }
      {
        auto ctx = CRenderContext{font, canvas};
        mContent->UpdateRender(ctx, {0.0f, 0.0f});
      }
    }
  }
}
