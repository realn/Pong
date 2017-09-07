#pragma once

#include "GFXCanvas.h"
#include "GFXCanvasView.h"
#include "GFXTextureAtlas.h"

namespace gui {
  class CScreen;

  class CScreenView {
  private:
    std::shared_ptr<gfx::CFont> mFont;
    gfx::CCanvas mCanvas;
    gfx::CCanvasView mCanvasView;
    gfx::CTextureAtlas mTextureAtlas;

  public:


    void UpdateRender(CScreen& screen);
    void Render(glm::mat4 const& tranform) const;
  };
}