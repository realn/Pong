#pragma once

#include "Canvas.h"
#include "Font.h"

namespace gui {
  struct CRenderContext {
    gfx::CCanvas& Canvas;
    gfx::CFont& Font;

    CRenderContext(gfx::CCanvas& canvas, gfx::CFont& font) : Canvas(canvas), Font(font) {}
  };
}