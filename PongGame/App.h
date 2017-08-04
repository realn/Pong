#pragma once

#include <memory>

#include <CBStr/Defines.h>
#include <CBSDL/Fwd.h>
#include <CBGL/Fwd.h>

#include <glm/vec2.hpp>

namespace pong {
  class CGame;

  class CApp {
  private:
    bool mRun;
    glm::uvec2 mScreenSize;
    glm::vec2 mGameScreenSize;
    std::unique_ptr<cb::sdl::CSystem> mSDLSystem;
    std::unique_ptr<cb::sdl::CWindow> mWindow;
    std::unique_ptr<cb::sdl::CPerfTimer> mTimer;
    std::unique_ptr<cb::sdl::CGLContext> mGLContext;
    std::unique_ptr<cb::gl::CProgram> mGLProgram;
    std::unique_ptr<CGame> mGame;

  public:
    CApp(cb::strvector const& cmdLineArgs);
    CApp(CApp&& other);
    ~CApp();

    int Execute();

  private:
    void PollEvents();

    void Update(float const timeDelta);
    void UpdateRender();
    void Render();

    static cb::gl::CShader LoadShader(cb::gl::ShaderType const type, cb::string const& filepath);
  };
}
