#include "stdafx.h"
#include "App.h"
#include "Consts.h"
#include "Game.h"
#include "Assets.h"

#include <CoreFont.h>
#include <GFXCanvas.h>
#include <GFXCanvasView.h>

#include <GUIWidget.h>
#include <GUIScreen.h>
#include <GUILayerStack.h>

#include <CBSDL/System.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Timer.h>
#include <CBGL/System.h>
#include <CBGL/Shader.h>
#include <CBGL/Texture.h>
#include <CBGL/State.h>
#include <CBIO/File.h>


namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs)
    : mRun(true), mScreenSize(800, 480) {
    using namespace cb::sdl;
    using namespace cb::gl;

    cb::info(L"Application initialization.");

    cb::info(L"Creating window");
    mSDLSystem = std::make_unique<CSystem>(System::VIDEO);
    mWindow = std::make_unique<CWindow>(L"Pong",
                                        CWindow::PosCentered,
                                        mScreenSize,
                                        WindowFlag::OPENGL);

    mTimer = std::make_unique<CPerfTimer>();

    cb::info(L"Initializing opengl context.");
    mGLContext = std::make_unique<CGLContext>(*mWindow, GLAttributeMapT{
      {GLAttribute::BUFFER_SIZE, 32},
      {GLAttribute::DEPTH_SIZE, 24},
      {GLAttribute::STENCIL_SIZE, 8},
      {GLAttribute::DOUBLEBUFFER, 1}
    });

    initextensions();

    mAssets = std::make_unique<CAssets>(L"Assets"s);

    auto aspect = static_cast<float>(mScreenSize.x) / static_cast<float>(mScreenSize.y);
    mGameScreenSize = glm::vec2(2.0f) * glm::vec2(aspect, 1.0f);

    mGame = std::make_shared<CGame>(mGameScreenSize, *mAssets);

    core::bind<core::IInputKeyEvents>(*this, *mGame);
    core::bind<core::IInputMouseEvents>(*this, *mGame);
  }

  CApp::~CApp() {}

  int CApp::Execute() {
    mWindow->Show();

    while(mRun) {
      mTimer->Update();

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
    for(auto i = 0u; i < UPDATE_MAX_EVENTS && CEvent::Poll(event); i++) {
      if(event.GetType() == EventType::QUIT ||
        (event.GetType() == EventType::WINDOWEVENT && event.Window().GetType() == WindowEventType::CLOSE)) {
        mRun = false;
      }
      if(event.GetType() == EventType::MOUSEMOTION) {
        auto pos = glm::vec2(event.Motion().GetPosition()) / glm::vec2(mScreenSize);
        pos.y = 1.0f - pos.y;
        pos *= mGameScreenSize;
        auto observers = core::IEventSource<core::IInputMouseEvents>::GetObservers();
        for(auto observer : observers) {
          observer->OnMouseMotion(pos, { 0.0f, 0.0f });
        }
      }
      if(event.GetType() == EventType::KEYDOWN || event.GetType() == EventType::KEYUP) {
        auto observers = core::IEventSource<core::IInputKeyEvents>::GetObservers();
        for(auto observer : observers) {
          observer->OnKeyState(event.Key().GetScanCode(), event.Key().GetType());
        }
      }
    }
  }

  void CApp::Update() {
    mFrameTime += mTimer->GetTimeDelta();
    for(auto i = 0u; i < UPDATE_MAX_STEPS && mFrameTime > UPDATE_TIME_STEP; i++) {
      Update(UPDATE_TIME_STEP);
      mFrameTime -= UPDATE_TIME_STEP;
    }
  }

  void CApp::Update(float const timeDelta) {
    mGame->Update(timeDelta);
  }

  void CApp::UpdateRender() {
    mGame->UpdateRender();
  }

  void CApp::Render() {
    cb::gl::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
    cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

    {
      auto trans = glm::ortho(0.0f, mGameScreenSize.x, 0.0f, mGameScreenSize.y);
      mGame->Render(trans);
    }
  }
}
