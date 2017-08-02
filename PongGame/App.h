#pragma once

#include <memory>

#include <CBStr/Defines.h>
#include <CBSDL/Fwd.h>
#include <CBGL/Fwd.h>

namespace pong {
  class CApp {
  private:
    bool mRun;
    std::unique_ptr<cb::sdl::CSystem> mSDLSystem;
    std::unique_ptr<cb::sdl::CWindow> mWindow;
    std::unique_ptr<cb::sdl::CGLContext> mGLContext;
    std::unique_ptr<cb::gl::CProgram> mGLProgram;
    std::unique_ptr<cb::gl::CBuffer> mGLBuffer;

  public:
    CApp(cb::strvector const& cmdLineArgs);
    CApp(CApp&& other);
    ~CApp();

    int Execute();

  private:
    void PollEvents();

    void Update();
    void UpdateRender();
    void Render();

    static cb::gl::CShader LoadShader(cb::gl::ShaderType const type, cb::string const& filepath);
  };
}
