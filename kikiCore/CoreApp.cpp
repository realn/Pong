#include "stdafx.h"
#include "CoreApp.h"
#include "CoreAppTask.h"
#include "CoreAppEvents.h"
#include "CoreAppInputEvents.h"

#include <CBSDL/System.h>
#include <CBSDL/GLContext.h>
#include <CBGL/System.h>

namespace core {
  CAppBase::CAppBase(cb::strvector const& args, CAppConfig const& config)
    : mArgs(args)
    , mConfig(config)
    , mSystem(cb::sdl::System::VIDEO | cb::sdl::System::TIMER | cb::sdl::System::EVENTS) 
  {}

  CAppBase::~CAppBase() {}

  int CAppBase::Execute() {
    if(!Init()) {
      return -1;
    }

    mTimer.Update();
    MainLoop();

    return 0;
  }

  void CAppBase::MainLoop() {
    auto frameTime = 0.0f;
    mTimer.Update();
    while(mRun) {
      Render();
      
      ProcessEvents();
      Update(frameTime);

      mGLContext->SwapWindow(*mWindow);

      UpdateRender(frameTime);

      mTimer.Update();
      frameTime += mTimer.GetTimeDelta();
    }
  }

  bool CAppBase::Init() {
    mTask = CreateTask();
    mTask->PrepareConfig(mArgs, mConfig);

    mWindow =
      std::make_unique<cb::sdl::CWindow>(mConfig.WindowTitle,
                                         mConfig.WindowPos, 
                                         mConfig.WindowSize,
                                         cb::sdl::WindowFlag::OPENGL);
    mWindow->Show();

    {
      using namespace cb::sdl;
      auto attribs = GLAttributeMapT{
        {GLAttribute::BUFFER_SIZE, mConfig.GfxBufferSize},
        {GLAttribute::DEPTH_SIZE, mConfig.GfxDepthSize},
        {GLAttribute::STENCIL_SIZE, mConfig.GfxStencilSize},
        {GLAttribute::DOUBLEBUFFER, 1},
      };
      mGLContext = 
        std::make_unique<cb::sdl::CGLContext>(*mWindow, attribs);
    }
    mGLContext->MakeCurrent(*mWindow);

    cb::gl::initextensions();

    if(!mTask->Init(*this)) {
      return false;
    }

    return true;
  }

  void CAppBase::ProcessEvents() {
    using namespace cb::sdl;
    auto event = CEvent();
    for(int i = 0u; mConfig.EventMaxIters; i++) {
      if(!CEvent::Poll(event))
        return;

      switch(event.GetType()) {
      case EventType::QUIT:
        Quit();
        break;

      case EventType::WINDOWEVENT:  
        ProcessWindowEvent(event);
        break;

      case EventType::MOUSEMOTION:
      case EventType::MOUSEWHEEL:
      case EventType::MOUSEBUTTONUP:
      case EventType::MOUSEBUTTONDOWN:
        ProcessMouseEvent(event);
        break;

      case EventType::KEYUP:
      case EventType::KEYDOWN:
        ProcessKeyEvent(event);
        break;

      default:
        break;
      }
    }
  }

  void CAppBase::Update(float& frameTime) {
    for(auto i = 0u; i < mConfig.UpdateMaxStepsPerFrame; i++) {
      if(frameTime < mConfig.UpdateTimeStep)
        return;

      UpdateFrame(mConfig.UpdateTimeStep);
      frameTime -= mConfig.UpdateTimeStep;
    }
  }

  void CAppBase::UpdateFrame(float const timeDelta) {
    mTask->Update(*this, timeDelta);
  }

  void CAppBase::UpdateRender(float const timeDelta) {
    mTask->UpdateRender(*this, timeDelta);
  }

  void CAppBase::Render() {
    mTask->Render(*this);
  }

  void CAppBase::ProcessWindowEvent(cb::sdl::CEvent const & event) {
    using namespace cb::sdl;
    auto observers = IEventSource<IAppEvents>::GetObservers();
    if(observers.empty())
      return;
    auto const winEvent = event.Window();

    for(auto observer : observers) {
      if(winEvent.GetType() == WindowEventType::CLOSE) {
        observer->OnAppClose(*this);
      }
    }
  }

  void CAppBase::ProcessMouseEvent(cb::sdl::CEvent const & event) {
    auto observers = IEventSource<IAppMouseEvents>::GetObservers();
    if(observers.empty())
      return;

    for(auto observer : observers) {
      using namespace cb::sdl;
      if(event.GetType() == EventType::MOUSEMOTION) {
        auto const mouseEvent = event.Motion();

        auto pos =
          glm::vec2(mouseEvent.GetPosition()) / glm::vec2(mConfig.WindowSize);
        auto delta =
          glm::vec2(mouseEvent.GetRelative()) / glm::vec2(mConfig.WindowSize);

        observer->OnMouseMotion(pos, delta);
      }
      if(event.GetType() == EventType::MOUSEBUTTONUP ||
         event.GetType() == EventType::MOUSEBUTTONDOWN) {
        auto const mouseEvent = event.Button();

        observer->OnMouseButton(mouseEvent.GetButton(), mouseEvent.GetType());
      }
    }
  }

  void CAppBase::ProcessKeyEvent(cb::sdl::CEvent const & event) {
    auto observers = IEventSource<IAppKeyEvents>::GetObservers();
    if(observers.empty())
      return;
    for(auto observer : observers) {
      observer->OnKeyState(event.Key().GetScanCode(), event.Key().GetType());
    }
  }
}
