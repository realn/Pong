#include <iostream>
#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <CBStr/Defines.h>
#include <CBStr/Convert.h>

#include <CBSDL/System.h>
#include <CBSDL/Font.h>
#include <CBSDL/Surface.h>

using namespace std::literals;

cb::strvector convMainArgs(int argc, char* argv[]) {
  auto result = cb::strvector();
  std::transform(argv, argv + argc, std::back_inserter(result), [](auto& item) -> auto{
    return cb::fromUtf8(item);
  });
  return result;
}

struct fontChar {
  wchar_t mCode = 0;
  glm::vec2 mSize;
  glm::vec2 mTexMin;
  glm::vec2 mTexMax;
  glm::vec2 mAdv;
};

int main(int argc, char* argv[]) {
  auto args = convMainArgs(argc, argv);

  auto fontPath = L"Instruction.otf"s;
  auto texSize = 512u;
  auto fontSize = 36u;
  auto fontChars = L"abcdefghijklmnopqrstuvwxyz0123456789`~!@#$^&*()-_=+[]{};:'\",./<>?|\\"s;

  auto sdlSys = cb::sdl::CSystem(cb::sdl::System::VIDEO);
  auto font = cb::sdl::CFont(fontPath, fontSize);

  auto finalSurface = cb::sdl::CSurface(glm::uvec2(texSize), 32, cb::sdl::PixelFormat::RGBA32);
  finalSurface.Fill(glm::vec4(0.0f));

  auto color = glm::vec4(1.0f);
  auto tpos = glm::uvec2(0);
  auto nrowh = 0u;
  for(auto& item : fontChars) {
    auto charSurf = font.RenderGlyphBlended(item, color);
    auto charSize = charSurf.GetSize();
    nrowh = std::max(nrowh, charSize.y);
    if(tpos.x + charSize.x > texSize) {
      tpos.x = 0;
      tpos.y += nrowh;
      if(tpos.y > texSize) {
        break;
      }
      nrowh = 0;
      finalSurface.Paste(tpos, charSurf);
    }
    else {
      finalSurface.Paste(tpos, charSurf);
    }
    tpos.x += charSize.x;
  }

  finalSurface.SavePNG(L"texture.png");

  std::cin.get();

  return 0;
}