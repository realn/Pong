#pragma once

#include <memory>

#include <CBGL/Fwd.h>

#include "Consts.h"

namespace pong {
  struct CVertex;
  class CFont;

  class CCanvas {
  private:
    std::shared_ptr<cb::gl::CTexture> mBaseTexture;
    std::shared_ptr<cb::gl::CTexture> mFontTexture;
    std::vector<CVertex> mVerts;
    std::vector<cb::u16> mIndices;

  public:
    CCanvas(std::shared_ptr<cb::gl::CTexture> baseTexture,
            std::shared_ptr<cb::gl::CTexture> fontTexture);
    ~CCanvas();

    std::shared_ptr<cb::gl::CTexture> GetBaseTexture() const { return mBaseTexture; }
    std::shared_ptr<cb::gl::CTexture> GetFontTexture() const { return mFontTexture; }
    const std::vector<cb::u16>& GetIndices() const { return mIndices; }

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
    void Print(glm::vec2 const& pos,
               CFont const& font, 
               cb::string const& text, 
               glm::vec4 const& color);

    void Clear();
    cb::gl::CBuffer CreateVertexBuffer() const;

  private:
    void AddVertex(glm::vec2 const& pos,
                   glm::vec2 const& tex,
                   glm::vec4 const& color);
  };
}