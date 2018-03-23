#pragma once

#include <CoreFwd.h>
#include <CoreFontRepository.h>
#include <GFXTextureRepository.h>
#include <GFXShaderRepository.h>

namespace pong {
  class CAssets {
  public:
    CAssets(cb::string const& assetsDir);
    ~CAssets();

    core::CFontRepository Fonts;
    gfx::CTextureRepository Textures;
    gfx::CShaderRepository Shaders;
  };
}
