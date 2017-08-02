#include "App.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/Events.h>

namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs) 
    : mSDLSystem(nullptr)
    , mWindow(nullptr)
  {
    mSDLSystem = std::make_unique<cb::sdl::CSystem>(cb::sdl::System::Video);
    mWindow = std::make_unique<cb::sdl::CWindow>(L"Pong", 
                                                 glm::ivec2{0, 0}, 
                                                 glm::uvec2{800u, 480u}, 
                                                 cb::sdl::WindowFlag::OPENGL);
  }

  CApp::~CApp() {}

  int CApp::Execute() {
    using namespace cb::sdl;

    mWindow->Show();

    auto run = true;
    auto event = CEvent();
    while(run) {
      while(CEvent::Poll(event)) {
        if(event.GetType() == EventType::QUIT ||
           (event.GetType() == EventType::WINDOWEVENT && event.Window().GetType() == WindowEventType::CLOSE)) {
          run = false;
        }
      }
    }

    mWindow->Hide();

    return 0;
  }
}
