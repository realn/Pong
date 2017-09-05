#include "stdafx.h"
#include "Font.h"
#include "Vertex.h"
#include "Consts.h"
#include "../FontCompiler/DataFont.h"

#include <CBSDL/Surface.h>
#include <CBIO/File.h>
#include <CBGL/Texture.h>

namespace gfx {
  CFont::CFont(std::shared_ptr<cb::gl::CTexture> texture)
    : mTexture(texture) {}

  CFont::~CFont() {}

  void CFont::AddChar(wchar_t code, CFont::CChar const & fontChar) {
    mChars[code] = fontChar;
  }

  const CFont::CChar & CFont::GetChar(wchar_t code) const {
    auto it = mChars.find(code);
    if(it != mChars.end())
      return it->second;
    it = mChars.find('?');
    if(it != mChars.end())
      return it->second;
    return mChars.begin()->second;
  }

  glm::vec2 CFont::GetTextSize(cb::string const & text, bool const charHeight) const {
    if(text.empty()) {
      return glm::vec2();
    }

    auto result = glm::vec2();
    for(auto i = 0u; i < text.length()-1; i++) {
      auto data = GetChar(text[i]);
      result.x += data.mAdv.x;
      result.y = glm::max(result.y, data.mSize.y);
    }

    auto data = GetChar(*text.rbegin());
    result.x += data.mSize.x;

    if(!charHeight) {
      result.y = glm::max(result.y, 1.0f);
    }

    return result;
  }

  CFont CFont::Load(cb::string const & filepath) {
    auto source = cb::readtextfileutf8(filepath);
    cb::CXmlDocument xmlDoc(source);
    if(!xmlDoc.IsValid()) {
      throw std::exception("Failed to load font.");
    }

    auto dataFont = data::CFont();
    if(!cb::ReadXmlObject(xmlDoc.RootNode, dataFont)) {
      throw std::exception("Failed to parse font xml.");
    }

    auto surf = cb::sdl::CSurface::Load(dataFont.mTexture);
    surf.Convert(cb::sdl::PixelFormat::RGBA32);

    auto tex = std::make_shared<cb::gl::CTexture>(surf.GetSize(), cb::gl::TextureFormat::RGBA8);
    tex->SetData(cb::gl::InputFormat::RGBA, surf.GetPixels());

    auto font = CFont(tex);

    for(auto& dataChar : dataFont.mChars) {
      font.AddChar(dataChar.mCode, {
        dataChar.mPos,
        dataChar.mSize,
        dataChar.mTexMin,
        dataChar.mTexMax,
        dataChar.mAdv
      });
    }

    return font;
  }

  glm::vec2 CFont::CChar::getVPos(glm::ivec2 const& xy) const {
    return mPos + mSize * glm::vec2(xy);
  }

  glm::vec2 CFont::CChar::getVTex(glm::ivec2 const& xy) const {
    auto nxy = glm::vec2(1.0f) - glm::vec2(xy);
    return mTexMin * nxy + mTexMax * glm::vec2(xy);
  }
}
