#include <iostream>
#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <CBStr/StringEx.h>

#include <CBSDL/System.h>
#include <CBSDL/Font.h>
#include <CBSDL/Surface.h>

#include <CBXml/Serialize.h>
#include <CBXml/Document.h>

#include <CBIO/File.h>

using namespace std::literals;

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

cb::strvector convMainArgs(int argc, char* argv[]) {
  auto result = cb::strvector();
  std::transform(argv, argv + argc, std::back_inserter(result), [](auto& item) -> auto{
    return cb::fromUtf8(item);
  });
  return result;
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

int main(int argc, char* argv[]) {
  auto args = convMainArgs(argc, argv);

  auto fontPath = L"Instruction.otf"s;
  auto texSize = 512u;
  auto fontSize = 36u;
  auto fontChars = L"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$^&*()-_=+[]{};:'\",./<>?|\\"s;

  auto sdlSys = cb::sdl::CSystem(cb::sdl::System::VIDEO);
  auto font = cb::sdl::CFont(fontPath, fontSize);

  auto finalSurface = cb::sdl::CSurface(glm::uvec2(texSize), 32, cb::sdl::PixelFormat::RGBA32);
  finalSurface.Fill(glm::vec4(0.0f));

  auto lineH = font.GetHeight();
  auto desc = glm::ivec2(0, font.GetDescent());

  auto color = glm::vec4(1.0f);
  auto tpos = glm::uvec2(0);
  auto nrowh = 0u;

  auto outFont = CFont();
  outFont.mName = font.GetName();
  outFont.mTexture = L"texture.png";

  for(auto& item : fontChars) {
    auto charSurf = font.RenderGlyphBlended(item, color);
    auto charSize = charSurf.GetSize();

    auto fontChar = CFontChar{item};

    nrowh = std::max(nrowh, charSize.y);
    if(tpos.x + charSize.x > texSize) {
      tpos.x = 0;
      tpos.y += nrowh;
      if(tpos.y > texSize) {
        break;
      }
      nrowh = 0;
    }
    finalSurface.Paste(tpos, charSurf);
    fontChar.mTexMin = glm::vec2(tpos) / glm::vec2(static_cast<float>(texSize));
    fontChar.mTexMax = glm::vec2(tpos + charSize) / glm::vec2(static_cast<float>(texSize));
    tpos.x += charSize.x;

    auto metrics = font.GetGlyphMetrics(item);

    fontChar.mPos = glm::vec2(metrics.min - desc) / glm::vec2(static_cast<float>(lineH));
    fontChar.mSize = glm::vec2(metrics.max - metrics.min) / glm::vec2(static_cast<float>(lineH));
    fontChar.mAdv = glm::vec2(metrics.advance, 0.0f) / glm::vec2(static_cast<float>(lineH));

    outFont.mChars.push_back(fontChar);
  }

  finalSurface.SavePNG(outFont.mTexture);

  auto xmlDoc = cb::CXmlDocument();
  cb::WriteXmlObject(xmlDoc.RootNode, outFont);
  xmlDoc.RootNode.SetName(L"Font");

  auto result = xmlDoc.ToString();
  cb::writetextfileutf8(L"font.xml", result);

  return 0;
}