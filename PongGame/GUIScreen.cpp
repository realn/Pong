#include "stdafx.h"
#include "GUIScreen.h"
#include "GUIWidget.h"

namespace gui {
  CScreen::~CScreen() {}

  void CScreen::Update(float timeDelta) {
    if(mContent) {
      mContent->Update(timeDelta);
    }
  }

  void CScreen::UpdateRender(CRenderContext const & ctx) {
    if(mContent) {
      mContent->UpdateRender(ctx, mSize);
    }
  }

  void CScreen::Render(CRenderContext & ctx) const {
    if(mContent) {
      mContent->Render(ctx, {0.0f, 0.0f});
    }
  }

  CWidget * CScreen::FindWidgetById(cb::string const & id) {
    if(mContent) {
      return mContent->FindWidgetById(id);
    }
    return nullptr;
  }

  const CWidget * CScreen::FindWidgetById(cb::string const & id) const {
    if(mContent) {
      return mContent->FindWidgetById(id);
    }
    return nullptr;
  }
}
