#include "stdafx.h"
#include "Assets.h"

namespace pong {
  CAssets::CAssets(cb::string const & assetsDir) 
    : Fonts(assetsDir), Textures(assetsDir), Shaders(assetsDir)
  {}

  CAssets::~CAssets() {}
}
