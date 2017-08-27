#include "stdafx.h"
#include "Font.h"
#include "Vertex.h"

#include <CBSDL/Surface.h>
#include <CBXml/Document.h>
#include <CBXml/Serialize.h>
#include <CBIO/File.h>
#include <CBGL/Texture.h>

namespace cb {
  string toStr(glm::vec2 const& value) {
    return format(L"{0},{1}", value.x, value.y);
  }
  bool fromStr(string const& text, glm::vec2& outValue) {
    static const auto splitComma = L","s;
    auto list = split(text, splitComma);
    if(list.size() < 2)
      return false;
    return
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }
}

struct CFontChar {
  wchar_t mCode = 0;
  glm::vec2 mPos;
  glm::vec2 mSize;
  glm::vec2 mTexMin;
  glm::vec2 mTexMax;
  glm::vec2 mAdv;
};

static const auto XML_FONTCHAR_CODE = L"Code"s;
static const auto XML_FONTCHAR_POS = L"Pos"s;
static const auto XML_FONTCHAR_SIZE = L"Size"s;
static const auto XML_FONTCHAR_TEXMIN = L"TexMin"s;
static const auto XML_FONTCHAR_TEXMAX = L"TexMax"s;
static const auto XML_FONTCHAR_ADV = L"Adv"s;

CB_DEFINEXMLRW(CFontChar) {
  return
    RWAttribute(XML_FONTCHAR_CODE, mObject.mCode) &&
    RWAttribute(XML_FONTCHAR_POS, mObject.mPos) &&
    RWAttribute(XML_FONTCHAR_SIZE, mObject.mSize) &&
    RWAttribute(XML_FONTCHAR_TEXMIN, mObject.mTexMin) &&
    RWAttribute(XML_FONTCHAR_TEXMAX, mObject.mTexMax) &&
    RWAttribute(XML_FONTCHAR_ADV, mObject.mAdv);
}

struct CFont {
  cb::string mName;
  cb::string mTexture;
  std::vector<CFontChar> mChars;
};

static const auto XML_FONT_NAME = L"Name"s;
static const auto XML_FONT_TEXTURE = L"Texture"s;
static const auto XML_FONT_CHARS_ELEM = L"Char"s;

CB_DEFINEXMLRW(CFont) {
  return
    RWAttribute(XML_FONT_NAME, mObject.mName) &&
    RWAttribute(XML_FONT_TEXTURE, mObject.mTexture) &&
    RWNodeList(mObject.mChars, XML_FONT_CHARS_ELEM);
}

namespace pong {
  CText::CText() {}

  CText::CText(CText && other) {
    std::swap(mText, other.mText);
    std::swap(mBuffer, other.mBuffer);
    std::swap(mTexture, other.mTexture);
    std::swap(mIndices, other.mIndices);
  }

  CText::~CText() {}

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

  CFont CFont::Load(cb::string const & filepath) {
    auto source = cb::readtextfileutf8(filepath);
    cb::CXmlDocument xmlDoc(source);
    if(!xmlDoc.IsValid()) {
      throw std::exception("Failed to load font.");
    }

    ::CFont dataFont;
    if(!cb::ReadXmlObject(xmlDoc.RootNode, dataFont)) {
      throw std::exception("Failed to parse font xml.");
    }

    auto surf = cb::sdl::CSurface::Load(dataFont.mTexture);
    surf.Convert(cb::sdl::PixelFormat::RGBA32);

    auto tex = std::make_shared<cb::gl::CTexture>(surf.GetSize(), cb::gl::TextureFormat::RGBA8);
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

  CText Print(CFont const & font, cb::string const & text) {
    auto verts = std::vector<CVertex>();
    auto indices = std::vector<cb::u16>();

    auto col = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    auto idx = {0, 1, 2, 0, 2, 3};
    auto pos = glm::vec2();
    for(auto& item : text) {
      auto& glyph = font.GetChar(item);

      auto i = static_cast<cb::u16>(verts.size());

      verts.push_back({pos + glyph.getVPos({0, 0}), glyph.getVTex({0, 0}), col});
      verts.push_back({pos + glyph.getVPos({1, 0}), glyph.getVTex({1, 0}), col});
      verts.push_back({pos + glyph.getVPos({1, 1}), glyph.getVTex({1, 1}), col});
      verts.push_back({pos + glyph.getVPos({0, 1}), glyph.getVTex({0, 1}), col});

      std::transform(std::begin(idx), std::end(idx), std::back_inserter(indices), 
      [i](auto& item) -> auto{
        return static_cast<cb::u16>(i + item);
      });

      pos += glyph.mAdv;
    }

    auto buffer = std::make_unique<cb::gl::CBuffer>();
    buffer->SetData(verts);

    auto result = CText();
    result.SetBuffers(std::move(buffer), indices);
    result.SetText(text);
    result.SetTexture(font.GetTexture());

    return result;
  }

  glm::vec2 CFont::CChar::getVPos(glm::ivec2 const& xy) const {
    return mPos + mSize * glm::vec2(xy);
  }

  glm::vec2 CFont::CChar::getVTex(glm::ivec2 const& xy) const {
    auto nxy = glm::vec2(1.0f) - glm::vec2(xy);
    return mTexMin * nxy + mTexMax * glm::vec2(xy);
  }
}
