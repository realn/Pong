#include "stdafx.h"
#include "GUIScreenView.h"
#include "GUIScreen.h"
#include "GUIRenderContext.h"

namespace gui {
  void CScreenView::UpdateRender(CScreen & screen) {
    auto ctx = CRenderContext{ mCanvas, *mFont };
    screen.UpdateRender(ctx);
    screen.Render(ctx);
    mCanvasView.UpdateRender(mCanvas);
  }

  void gui::CScreenView::Render(glm::mat4 const & tranform) const {
    mCanvasView.Render(tranform);
  }
}
