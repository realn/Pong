#include "App.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/Events.h>
#include <CBSDL/GLContext.h>
#include <CBGL/Rendering.h>

namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs)
    : mRun(true)
  {
    mSDLSystem = std::make_unique<cb::sdl::CSystem>(cb::sdl::System::Video);
    mWindow = std::make_unique<cb::sdl::CWindow>(L"Pong", 
                                                 cb::sdl::CWindow::PosCentered, 
                                                 glm::uvec2{800u, 480u}, 
                                                 cb::sdl::WindowFlag::OPENGL);

    mGLContext = std::make_unique<cb::sdl::CGLContext>(*mWindow, cb::sdl::GLAttributeMapT{
      {cb::sdl::GLAttribute::BUFFER_SIZE, 32},
      {cb::sdl::GLAttribute::DEPTH_SIZE, 24},
      {cb::sdl::GLAttribute::STENCIL_SIZE, 8},
      {cb::sdl::GLAttribute::DOUBLEBUFFER, 1}
    });
  }

  CApp::CApp(CApp && other) {
    std::swap(mSDLSystem, other.mSDLSystem);
    std::swap(mWindow, other.mWindow);
    std::swap(mGLContext, other.mGLContext);
  }

  CApp::~CApp() {}

  int CApp::Execute() {

    mWindow->Show();

    while(mRun) {
      PollEvents();

      UpdateRender();
      Render();
      Update();

      mGLContext->SwapWindow(*mWindow);
    }

    mWindow->Hide();

    return 0;
  }

  void CApp::PollEvents() {
    using namespace cb::sdl;

    auto event = CEvent();
    while(CEvent::Poll(event)) {
      if(event.GetType() == EventType::QUIT ||
        (event.GetType() == EventType::WINDOWEVENT && event.Window().GetType() == WindowEventType::CLOSE)) {
        mRun = false;
      }
    }
  }

  void CApp::Update() {}

  void CApp::UpdateRender() {}

  void CApp::Render() {
    cb::gl::clearColor({0.2f, 0.2f, 0.2f, 1.0f});
    cb::gl::clear(cb::gl::ClearBuffer::COLOR);


  }
}
