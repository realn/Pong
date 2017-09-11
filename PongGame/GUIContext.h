#pragma once

namespace gfx {
  class CFont;
  class CCanvas;
}

namespace gui {
  struct CUpdateContext {
    gfx::CFont const& Font;

    CUpdateContext(gfx::CFont const& font) : Font(font) {}
  };

  struct CRenderContext {
    gfx::CFont const& Font;
    gfx::CCanvas& Canvas;

    CRenderContext(gfx::CFont const& font, gfx::CCanvas& canvas) : Font(font), Canvas(canvas) {}
  };
}