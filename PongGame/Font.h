#pragma once

#include <memory>
#include <vector>
#include <map>

#include <CBStr/StringEx.h>
#include <CBGL/Fwd.h>
#include <glm/vec2.hpp>

namespace pong {
  class CFont {
  public:
    struct CChar {
      glm::vec2 mPos;
      glm::vec2 mSize;
      glm::vec2 mTexMin;
      glm::vec2 mTexMax;
      glm::vec2 mAdv;

      glm::vec2 getVPos(glm::ivec2 const& xy) const;
      glm::vec2 getVTex(glm::ivec2 const& xy) const;
    };
  private:
    std::map<wchar_t, CChar> mChars;
    std::shared_ptr<cb::gl::CTexture> mTexture;
    float mLineHeight;

  public:
    CFont(std::shared_ptr<cb::gl::CTexture> texture);
    ~CFont();

    void AddChar(wchar_t code, CChar const& fontChar);

    const CChar&  GetChar(wchar_t code) const;
    std::shared_ptr<cb::gl::CTexture> GetTexture() const { return mTexture; }
    float GetLineHeight() const { return 1.0f; }
    glm::vec2 GetTextSize(cb::string const& text, bool const charHeight = false) const;

    static CFont Load(cb::string const& filepath);
  };
}
