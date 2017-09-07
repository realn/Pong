#pragma once

#include "GFXCanvas.h"
#include "Font.h"

namespace gui {
  struct CRenderContext {
    gfx::CCanvas& Canvas;
    gfx::CFont const& Font;

    CRenderContext(gfx::CCanvas& canvas, gfx::CFont const& font) : Canvas(canvas), Font(font) {}
  };
}