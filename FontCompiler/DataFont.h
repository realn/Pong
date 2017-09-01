#pragma once

#include <glm/glm.hpp>
#include <CBStr/StringEx.h>
#include <CBXml/Serialize.h>
#include <CBXml/Document.h>

using namespace std::literals;

namespace cb {
  static string toStr(glm::vec2 const& value) {
    return format(L"{0},{1}", value.x, value.y);
  }
  static bool fromStr(string const& text, glm::vec2& outValue) {
    static const auto splitComma = L","s;
    auto list = split(text, splitComma);
    if(list.size() < 2)
      return false;
    return
      fromStr(list[0], outValue.x) &&
      fromStr(list[1], outValue.y);
  }
}

namespace data {
  struct CFontChar {
    wchar_t mCode = 0;
    glm::vec2 mPos;
    glm::vec2 mSize;
    glm::vec2 mTexMin;
    glm::vec2 mTexMax;
    glm::vec2 mAdv;
  };

  struct CFont {
    cb::string mName;
    cb::string mTexture;
    std::vector<CFontChar> mChars;
  };

  static const auto XML_FONTCHAR_CODE = L"Code"s;
  static const auto XML_FONTCHAR_POS = L"Pos"s;
  static const auto XML_FONTCHAR_SIZE = L"Size"s;
  static const auto XML_FONTCHAR_TEXMIN = L"TexMin"s;
  static const auto XML_FONTCHAR_TEXMAX = L"TexMax"s;
  static const auto XML_FONTCHAR_ADV = L"Adv"s;

  static const auto XML_FONT_NAME = L"Name"s;
  static const auto XML_FONT_TEXTURE = L"Texture"s;
  static const auto XML_FONT_CHARS_ELEM = L"Char"s;

}

CB_DEFINEXMLRW(data::CFontChar) {
  return
    RWAttribute(data::XML_FONTCHAR_CODE, mObject.mCode) &&
    RWAttribute(data::XML_FONTCHAR_POS, mObject.mPos) &&
    RWAttribute(data::XML_FONTCHAR_SIZE, mObject.mSize) &&
    RWAttribute(data::XML_FONTCHAR_TEXMIN, mObject.mTexMin) &&
    RWAttribute(data::XML_FONTCHAR_TEXMAX, mObject.mTexMax) &&
    RWAttribute(data::XML_FONTCHAR_ADV, mObject.mAdv);
}

CB_DEFINEXMLRW(data::CFont) {
  return
    RWAttribute(data::XML_FONT_NAME, mObject.mName) &&
    RWAttribute(data::XML_FONT_TEXTURE, mObject.mTexture) &&
    RWNodeList(mObject.mChars, data::XML_FONT_CHARS_ELEM);
}

