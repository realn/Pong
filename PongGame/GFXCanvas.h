#pragma once

#include <memory>

#include <CBGL/Fwd.h>

#include "Consts.h"
#include "GFXTextureAtlas.h"

namespace gfx {
  struct CVertex;
  class CFont;
  class CBRect;

  class CCanvas {
  private:
    std::vector<CVertex> mVertices;
    std::vector<cb::u16> mIndices;
    CTextureAtlas mTextureAtlas;

  public:
    CCanvas(CTextureAtlas const& textureAtlas);
    ~CCanvas();

    void SetTextureAtlas(CTextureAtlas const& atlas) { mTextureAtlas = atlas; }

    std::vector<CVertex> const& GetVertices() const { return mVertices; }
    std::vector<cb::u16> const& GetIndices() const { return mIndices; }
    CTextureAtlas const& GetTextureAtlas() const { return mTextureAtlas; }

    void DrawPoint(glm::vec2 const& pos,
                   float const size = 0.01f,
                   glm::vec4 const& color = glm::vec4(1.0f));
    void DrawLine(glm::vec2 const& posA,
                  glm::vec2 const& posB,
                  float const width = 0.01f,
                  glm::vec4 const& color = glm::vec4(1.0f));
    void DrawTriangle(glm::vec2 const& posA,
                      glm::vec2 const& posB,
                      glm::vec2 const& posC,
                      glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(glm::vec2 const& pos,
                  glm::vec2 const& size,
                  glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(glm::vec2 const& pos,
                  glm::vec2 const& size,
                  cb::string const& imgName,
                  glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(CBRect const& rect, glm::vec4 const& color = glm::vec4(1.0f));
    void DrawRect(CBRect const& rect, CBRect const& texRect, glm::vec4 const& color = glm::vec4(1.0f));
    void Print(glm::vec2 const& tpos,
               CFont const& font, 
               cb::string const& text, 
               glm::vec4 const& color,
               glm::vec2 const& scale = glm::vec2(1.0f));

    void Clear();

  private:
    void internalDrawRect(CBRect const& prect, CBRect const& trect, glm::vec4 const& color);
    void AddVertex(glm::vec2 const& pos,
                   glm::vec2 const& tex,
                   glm::vec4 const& color);
  };
}
