#include "stdafx.h"
#include "Canvas.h"
#include "Vertex.h"
#include "Font.h"

namespace pong {
  CCanvas::CCanvas(std::shared_ptr<cb::gl::CTexture> baseTexture, 
                   std::shared_ptr<cb::gl::CTexture> fontTexture) 
    : mBaseTexture(baseTexture)
    , mFontTexture(fontTexture)
  {}

  CCanvas::~CCanvas() {}

  void CCanvas::DrawPoint(glm::vec2 const & pos, float const size, glm::vec4 const & color) {
    auto half = glm::vec2(size) / 2.0f;
    auto tex = glm::vec2();

    AddVertex(pos - half, tex, color);
    AddVertex(pos + glm::vec2{half.x, -half.y}, tex, color);
    AddVertex(pos + half, tex, color);
    
    AddVertex(pos - half, tex, color);
    AddVertex(pos + half, tex, color);
    AddVertex(pos + glm::vec2{-half.x, half.y}, tex, color);
  }

  void CCanvas::DrawLine(glm::vec2 const & posA, glm::vec2 const & posB, float const width, glm::vec4 const & color) {
    // TODO
  }

  void CCanvas::DrawTriangle(glm::vec2 const & posA, glm::vec2 const & posB, glm::vec2 const & posC, glm::vec4 const & color) {
    auto tex = glm::vec2();
    AddVertex(posA, tex, color);
    AddVertex(posB, tex, color);
    AddVertex(posC, tex, color);
  }

  void CCanvas::DrawRect(glm::vec2 const & pos, glm::vec2 const & size, glm::vec4 const & color) {
    auto tex = glm::vec2();
    AddVertex(pos, tex, color);
    AddVertex(pos + glm::vec2{size.x, 0.0f}, tex, color);
    AddVertex(pos + size, tex, color);

    AddVertex(pos, tex, color);
    AddVertex(pos + size, tex, color);
    AddVertex(pos + glm::vec2{0.0f, size.y}, tex, color);
  }

  void CCanvas::Print(glm::vec2 const& tpos, CFont const & font, cb::string const & text, glm::vec4 const& color) {
    auto idx = {0, 1, 2, 0, 2, 3};
    auto pos = tpos;
    auto col = glm::vec4(color.r, color.g, color.b, color.a + 2.0f);
    for(auto& item : text) {
      auto& glyph = font.GetChar(item);

      auto i = static_cast<cb::u16>(mVerts.size());

      mVerts.push_back({pos + glyph.getVPos({0, 0}), glyph.getVTex({0, 0}), col});
      mVerts.push_back({pos + glyph.getVPos({1, 0}), glyph.getVTex({1, 0}), col});
      mVerts.push_back({pos + glyph.getVPos({1, 1}), glyph.getVTex({1, 1}), col});
      mVerts.push_back({pos + glyph.getVPos({0, 1}), glyph.getVTex({0, 1}), col});

      std::transform(std::begin(idx), std::end(idx), std::back_inserter(mIndices),
                     [i](auto& item) -> auto{
        return static_cast<cb::u16>(i + item);
      });

      pos += glyph.mAdv;
    }
  }

  void CCanvas::Clear() {
    mVerts.clear();
    mIndices.clear();
  }

  cb::gl::CBuffer CCanvas::CreateVertexBuffer() const {
    auto buf = cb::gl::CBuffer();
    buf.SetData(mVerts);
    return buf;
  }

  void CCanvas::AddVertex(glm::vec2 const & pos, glm::vec2 const & tex, glm::vec4 const & color) {
    auto vert = CVertex(pos, tex, color);
    auto it = std::find(mVerts.begin(), mVerts.end(), vert);
    if(it != mVerts.end()) {
      mIndices.push_back(static_cast<cb::u16>(it - mVerts.begin()));
    }
    else {
      auto index = mVerts.size();
      mVerts.push_back(vert);
      mIndices.push_back(static_cast<cb::u16>(index));
    }
  }
}