#include "stdafx.h"
#include "GUIScreenView.h"
#include "GUIScreen.h"
#include "GFXCanvas.h"
#include "GFXCanvasView.h"
#include "GFXTextureAtlas.h"

#include <CoreFont.h>

#include <CBIO/File.h>
#include <CBSDL/Surface.h>
#include <CBGL/Texture.h>

namespace gui {
  CScreenView::CScreenView(std::shared_ptr<core::CFont> font,
                           std::shared_ptr<cb::gl::CProgram> guiProgram,
                           gfx::CTextureAtlas const& textureAtlas)
    : mFont(font), mTextureAtlas(textureAtlas), mTransform(1.0f)
  {
    auto fontTexture = LoadTexture(font->GetTextureFilePath());
    auto baseTexture = LoadTexture(textureAtlas.GetTextureFileName());

    mCanvasView = std::make_unique<gfx::CCanvasView>(guiProgram, baseTexture, fontTexture);
  }

  CScreenView::~CScreenView() {}

  gfx::CCanvas CScreenView::CreateCanvas() const {
    return gfx::CCanvas(mTextureAtlas);
  }

  void CScreenView::UpdateRender(CScreen const& screen, gfx::CCanvas const& canvas) {
    auto screenSize = screen.GetSize();
    mTransform = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y);
    mCanvasView->UpdateRender(canvas);
  }

  void gui::CScreenView::Render() const {
    mCanvasView->Render(mTransform);
  }

  std::shared_ptr<cb::gl::CTexture> CScreenView::LoadTexture(cb::string const & filepath) {
    auto surface = cb::sdl::CSurface::Load(filepath);
    surface = surface.Convert(cb::sdl::PixelFormat::RGBA32);

    auto texture = cb::gl::CTexture(surface.GetSize(), cb::gl::TextureFormat::RGBA8);
    texture.SetData(cb::gl::InputFormat::RGBA, surface.GetPixels());

    return std::make_shared<cb::gl::CTexture>(std::move(texture));
  }
}
