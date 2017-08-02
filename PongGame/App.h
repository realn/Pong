#pragma once

#include <memory>

#include <CBStr/Defines.h>
#include <CBSDL/Fwd.h>

namespace pong {
  class CApp {
  private:
    bool mRun;
    std::unique_ptr<cb::sdl::CSystem> mSDLSystem;
    std::unique_ptr<cb::sdl::CWindow> mWindow;
    std::unique_ptr<cb::sdl::CGLContext> mGLContext;

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
  };
}
