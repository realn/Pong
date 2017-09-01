#include "FontCompiler.h"
#include "DataFont.h"

#include <glm/glm.hpp>
#include <CBSDL/Font.h>
#include <CBSDL/Surface.h>
#include <CBIO/File.h>
#include <CBIO/Path.h>

struct CCompileContext {
  glm::uvec2 TexPos;
  glm::ivec2 Descent;
  glm::ivec2 Ascent;
  unsigned LineHeight = 0u;
  unsigned RowHeight = 0u;
};

bool CompileFont(CFontParams const & params) {
  auto font = cb::sdl::CFont(params.FontPath, params.FontSize);
  auto finalSurface = cb::sdl::CSurface(glm::uvec2(params.TexSize), 32, cb::sdl::PixelFormat::RGBA32);
  finalSurface.Fill(glm::vec4(0.0f));

  auto ctx = CCompileContext();
  ctx.LineHeight = font.GetHeight();
  ctx.Ascent = glm::ivec2(0, font.GetAscent());
  ctx.Descent = glm::ivec2(0, font.GetDescent());
  ctx.TexPos = glm::uvec2(params.TexCharBorder);

  auto outFont = data::CFont();
  outFont.mName = font.GetName();
  outFont.mTexture = cb::makefilename(params.OutputName, L"png"s);

  for(auto& item : params.FontChars) {
    auto glyphSurface = font.RenderGlyphBlended(item, params.CharColor);
    auto glyphSize = glyphSurface.GetSize();
    auto metrics = font.GetGlyphMetrics(item);

    auto fontChar = data::CFontChar{item};

    ctx.RowHeight = std::max(ctx.RowHeight, glyphSize.y);
    if(ctx.TexPos.x + glyphSize.x + 2 * params.TexCharBorder > params.TexSize) {
      ctx.TexPos.x = params.TexCharBorder;
      ctx.TexPos.y += ctx.RowHeight + 2 * params.TexCharBorder;
      if(ctx.TexPos.y > params.TexSize) {
        break;
      }
      ctx.RowHeight = 0;
    }

    finalSurface.Paste(ctx.TexPos, glyphSurface);
    fontChar.mTexMin = glm::vec2(ctx.TexPos) / glm::vec2(static_cast<float>(params.TexSize));
    fontChar.mTexMax = glm::vec2(ctx.TexPos + glyphSize) / glm::vec2(static_cast<float>(params.TexSize));
    ctx.TexPos.x += glyphSize.x + 2 * params.TexCharBorder;

    fontChar.mPos = glm::vec2(metrics.min) / glm::vec2(static_cast<float>(ctx.LineHeight));
    fontChar.mSize = glm::vec2(metrics.max - metrics.min) / glm::vec2(static_cast<float>(ctx.LineHeight));
    fontChar.mAdv = glm::vec2(metrics.advance, 0.0f) / glm::vec2(static_cast<float>(ctx.LineHeight));

    outFont.mChars.push_back(fontChar);
  }

  finalSurface.SavePNG(cb::makepath(params.OutputPath, outFont.mTexture));

  auto xmlDoc = cb::CXmlDocument();
  cb::WriteXmlObject(xmlDoc.RootNode, outFont);
  xmlDoc.RootNode.SetName(L"Font");

  auto outfilename = cb::makefilename(params.OutputName, L"xml"s);
  auto result = xmlDoc.ToString();
  cb::writetextfileutf8(cb::makepath(params.OutputPath, outfilename), result);
  return true;
}
