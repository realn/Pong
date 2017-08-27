#pragma once

#include <memory>
#include <vector>
#include <map>

#include <CBStr/StringEx.h>
#include <CBGL/Fwd.h>
#include <glm/vec2.hpp>

namespace pong {
  class CText {
  private:
    cb::string mText;
    std::unique_ptr<cb::gl::CBuffer> mBuffer;
    std::shared_ptr<cb::gl::CTexture> mTexture;
    std::vector<cb::u16> mIndices;

  public:
    CText();
    CText(CText && other);
    ~CText();

    void SetText(cb::string const& value) { mText = value; }
    void SetBuffers(std::unique_ptr<cb::gl::CBuffer> buffer, std::vector<cb::u16>& indices) {
      mBuffer = std::move(buffer); mIndices = indices;
    }
    void SetTexture(std::shared_ptr<cb::gl::CTexture> texture) { mTexture = texture; }
  };

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

  public:
    CFont(std::shared_ptr<cb::gl::CTexture> texture);
    ~CFont();

    void AddChar(wchar_t code, CChar const& fontChar);

    const CChar&  GetChar(wchar_t code) const;
    std::shared_ptr<cb::gl::CTexture> GetTexture() const { return mTexture; }

    static CFont Load(cb::string const& filepath);
  };

  extern CText Print(CFont const& font, cb::string const& text);
}
