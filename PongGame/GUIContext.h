#pragma once

namespace gfx {
  class CFont;
  class CCanvas;
}

namespace gui {
  struct CUpdateContext {
    gfx::CFont const& Font;
    glm::vec2 const TextScale;

    CUpdateContext(gfx::CFont const& font, 
                   glm::vec2 const textScale) : Font(font), TextScale(textScale) {}
  };

  struct CRenderContext {
    gfx::CFont const& Font;
    gfx::CCanvas& Canvas;
    glm::vec2 const TextScale;

    CRenderContext(gfx::CFont const& font, 
                   gfx::CCanvas& canvas,
                   glm::vec2 const textScale) : Font(font), Canvas(canvas), TextScale(textScale) {}
  };
}