#include "stdafx.h"
#include "GFXCanvas.h"
#include "Vertex.h"
#include "Font.h"
#include "BRect.h"

namespace gfx {
  CCanvas::CCanvas(CTextureAtlas const& textureAtlas)
    : mTextureAtlas(textureAtlas)
  {}

  CCanvas::~CCanvas() {}

  void CCanvas::DrawPoint(glm::vec2 const & pos, float const size, glm::vec4 const & color) {
    auto half = glm::vec2(size) / 2.0f;
    auto tex = glm::vec2();
    internalDrawRect(CBRect(pos - half, glm::vec2(size)), CBRect(), glm::clamp(color, 0.0f, 1.0f));
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
    internalDrawRect(CBRect(pos, size), CBRect(), glm::clamp(color, 0.0f, 1.0f));
  }

  void CCanvas::DrawRect(glm::vec2 const & pos, glm::vec2 const & size, cb::string const & imgName, glm::vec4 const & color) {
    auto tex = mTextureAtlas[imgName];
    auto prect = CBRect(pos, size);
    auto trect = CBRect(tex.TexMin, tex.TexMax - tex.TexMin);

    internalDrawRect(CBRect(pos, size),
                     CBRect(tex.TexMin, tex.TexMax - tex.TexMin),
                     glm::clamp(color, 0.0f, 1.0f) + glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }

  void CCanvas::DrawRect(CBRect const & rect, glm::vec4 const & color) {
    internalDrawRect(rect, CBRect(), color);
  }

  void CCanvas::DrawRect(CBRect const & rect, CBRect const & texRect, glm::vec4 const & color) {
    internalDrawRect(rect, texRect, glm::clamp(color, 0.0f, 1.0f) + glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }

  void CCanvas::Print(glm::vec2 const& tpos, CFont const & font, cb::string const & text, glm::vec4 const& color) {
    auto idx = {0, 1, 2, 0, 2, 3};
    auto pos = tpos;
    auto col = glm::vec4(color.r, color.g, color.b, color.a + 2.0f);
    for(auto& item : text) {
      auto& glyph = font.GetChar(item);

      auto i = static_cast<cb::u16>(mVertices.size());

      mVertices.push_back({pos + glyph.getVPos({0, 0}), glyph.getVTex({0, 0}), col});
      mVertices.push_back({pos + glyph.getVPos({1, 0}), glyph.getVTex({1, 0}), col});
      mVertices.push_back({pos + glyph.getVPos({1, 1}), glyph.getVTex({1, 1}), col});
      mVertices.push_back({pos + glyph.getVPos({0, 1}), glyph.getVTex({0, 1}), col});

      std::transform(std::begin(idx), std::end(idx), std::back_inserter(mIndices),
                     [i](auto& item) -> auto{
        return static_cast<cb::u16>(i + item);
      });

      pos += glyph.mAdv;
    }
  }

  void CCanvas::Clear() {
    mVertices.clear();
    mIndices.clear();
  }

  void CCanvas::internalDrawRect(CBRect const & prect, CBRect const & trect, glm::vec4 const & color) {
    AddVertex(prect.GetUCrd(0, 0), trect.GetUCrd(0, 0), color);
    AddVertex(prect.GetUCrd(1, 0), trect.GetUCrd(1, 0), color);
    AddVertex(prect.GetUCrd(1, 1), trect.GetUCrd(1, 1), color);

    AddVertex(prect.GetUCrd(0, 0), trect.GetUCrd(0, 0), color);
    AddVertex(prect.GetUCrd(1, 1), trect.GetUCrd(1, 1), color);
    AddVertex(prect.GetUCrd(0, 1), trect.GetUCrd(0, 1), color);
  }

  void CCanvas::AddVertex(glm::vec2 const & pos, glm::vec2 const & tex, glm::vec4 const & color) {
    auto vert = CVertex(pos, tex, color);
    auto it = std::find(mVertices.begin(), mVertices.end(), vert);
    if(it != mVertices.end()) {
      mIndices.push_back(static_cast<cb::u16>(it - mVertices.begin()));
    }
    else {
      auto index = mVertices.size();
      mVertices.push_back(vert);
      mIndices.push_back(static_cast<cb::u16>(index));
    }
  }
}