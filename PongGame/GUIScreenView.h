#pragma once

#include <glm/mat4x4.hpp>
#include <CBGL/Fwd.h>

#include "GFXTextureAtlas.h"

namespace gfx {
  class CFont;
  class CCanvas;
  class CCanvasView;
  class CTextureAtlas;
}

namespace gui {
  class CScreen;

  class CScreenView {
  private:
    std::shared_ptr<gfx::CFont> mFont;
    std::unique_ptr<gfx::CCanvasView> mCanvasView;
    gfx::CTextureAtlas mTextureAtlas;
    glm::mat4 mTransform;

  public:
    CScreenView(std::shared_ptr<gfx::CFont> font,
                std::shared_ptr<cb::gl::CProgram> guiProgram,
                gfx::CTextureAtlas const& textureAtlas);
    ~CScreenView();

    gfx::CCanvas CreateCanvas() const;

    gfx::CFont const& GetFont() const { return *mFont; }
    gfx::CTextureAtlas const& GetTextureAtlas() const { return mTextureAtlas; }

    void UpdateRender(CScreen const& screen, gfx::CCanvas const& canvas);
    void Render() const;

  private:
    static std::shared_ptr<cb::gl::CTexture> LoadTexture(cb::string const& filepath);
  };
}
