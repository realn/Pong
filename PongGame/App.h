#pragma once

#include <memory>

#include <CBStr/Defines.h>
#include <CBSDL/Fwd.h>

namespace pong {
  class CApp {
  private:
    std::unique_ptr<cb::sdl::CSystem> mSDLSystem;
    std::unique_ptr<cb::sdl::CWindow> mWindow;

  public:
    CApp(cb::strvector const& cmdLineArgs);
    CApp(CApp&&) = delete;
    CApp(CApp const&) = delete;
    ~CApp();

    int Execute();
  };
}
