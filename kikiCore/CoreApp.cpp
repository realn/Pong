#include "stdafx.h"
#include "CoreApp.h"
#include "CoreInput.h"
#include "CoreInputDevice.h"
#include "CoreInputStdDevices.h"
#include "CoreScene.h"

#include <CBSDL/System.h>
#include <CBSDL/GLContext.h>
#include <CBGL/System.h>

namespace core {
  CApp::CApp(cb::strvector const& args, CAppConfig const& config)
    : mArgs(args)
    , mConfig(config)
    , mSystem(cb::sdl::System::VIDEO | cb::sdl::System::TIMER | cb::sdl::System::EVENTS) 
  {}

  CApp::~CApp() {}

  int CApp::Execute() {
    if(!Init()) {
      return -1;
    }

    mTimer.Update();
    MainLoop();

    return 0;
  }

  void CApp::MainLoop() {
    auto frameTime = 0.0f;
    mTimer.Update();
    while(mRun && mScene) {
      mScene->Render();

      ProcessEvents();
      mInput->Update(frameTime);
      Update(frameTime);

      mGLContext->SwapWindow(*mWindow);

      mScene->UpdateRender(frameTime);

      mTimer.Update();
      frameTime += mTimer.GetTimeDelta();

      if(mScene->IsFinished()) {
        mScene = mScene->OnCreateScene();
      }
    }
  }

  bool CApp::Init() {
    if(!OnAdjustConfig(mConfig)) {
      return false;
    }

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

    mInput = std::make_shared<CInput>();
    mInput->AddDevice(std::make_shared<core::CInputKeyboardDevice>(0, L"Keyboard"));

    if(!OnInit()) {
      return false;
    }

    mScene = OnCreateScene();
    return true;
  }

  void CApp::ProcessEvents() {
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

      mInput->ProcessSystemEvent(event);
    }
  }

  void CApp::Update(float& frameTime) {
    for(auto i = 0u; i < mConfig.UpdateMaxStepsPerFrame; i++) {
      if(frameTime < mConfig.UpdateTimeStep)
        return;

    mScene->Update(mConfig.UpdateTimeStep);
      frameTime -= mConfig.UpdateTimeStep;
    }
  }

  void CApp::ProcessWindowEvent(cb::sdl::CEvent const & event) {
    using namespace cb::sdl;
    auto const winEvent = event.Window();

    if(winEvent.GetType() == WindowEventType::CLOSE) {
      OnAppClose();
    }
  }

  void CApp::ProcessMouseEvent(cb::sdl::CEvent const & event) {
    using namespace cb::sdl;
    if(event.GetType() == EventType::MOUSEMOTION) {
      auto const mouseEvent = event.Motion();

      auto pos =
        glm::vec2(mouseEvent.GetPosition()) / glm::vec2(mConfig.WindowSize);
      auto delta =
        glm::vec2(mouseEvent.GetRelative()) / glm::vec2(mConfig.WindowSize);

      OnMouseMotion(pos, delta);
    }
    if(event.GetType() == EventType::MOUSEBUTTONUP ||
        event.GetType() == EventType::MOUSEBUTTONDOWN) {
      auto const mouseEvent = event.Button();

      OnMouseButton(mouseEvent.GetButton(), mouseEvent.GetType());
    }
  }

  void CApp::ProcessKeyEvent(cb::sdl::CEvent const & event) {
    OnKeyState(event.Key().GetScanCode(), event.Key().GetType());
  }
}
