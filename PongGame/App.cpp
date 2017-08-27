#include "stdafx.h"
#include "App.h"
#include "Vertex.h"
#include "Consts.h"
#include "Game.h"
#include "Font.h"

#include <CBSDL/System.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Timer.h>
#include <CBGL/System.h>
#include <CBGL/Shader.h>
#include <CBIO/File.h>

namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs)
    : mRun(true) 
    , mScreenSize(800, 480)
  {
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

    {
      auto vsh = LoadShader(ShaderType::VERTEX, L"main_vs.glsl");
      auto fsh = LoadShader(ShaderType::FRAGMENT, L"main_fs.glsl");

      mGLProgram = std::make_unique<CProgram>();
      mGLProgram->Attach({std::move(vsh), std::move(fsh)});
      mGLProgram->SetInLocation({
        {render::IDX_VERTEX4, render::VIN_VERTEX4}, 
        {render::IDX_COLOR4, render::VIN_COLOR4}
      });
      mGLProgram->Link();
      if(!mGLProgram->IsLinked()) {
        cb::warn(L"Failed to link gl program.");
        cb::info(mGLProgram->GetLinkLog());
        throw std::exception("GL program compilation failed.");
      }
    }

    auto aspect = static_cast<float>(mScreenSize.x) / static_cast<float>(mScreenSize.y);
    mGameScreenSize = glm::vec2(2.0f) * glm::vec2(aspect, 1.0f);

    mGame = std::make_unique<CGame>(mGameScreenSize);

    mFont = std::make_unique<CFont>(CFont::Load(L"font.xml"));
    mText = std::make_unique<CText>(Print(*mFont, L"Test"));
  }

  CApp::CApp(CApp && other) {
    std::swap(mSDLSystem, other.mSDLSystem);
    std::swap(mWindow, other.mWindow);
    std::swap(mGLContext, other.mGLContext);
    std::swap(mGLProgram, other.mGLProgram);
    std::swap(mTimer, other.mTimer);
    std::swap(mGame, other.mGame);
  }

  CApp::~CApp() {}

  int CApp::Execute() {

    mWindow->Show();

    while(mRun) {
      mTimer->Update();
      PollEvents();

      UpdateRender();
      Render();
      Update(mTimer->GetTimeDelta());

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
      if(event.GetType() == EventType::MOUSEMOTION) {
        auto pos = glm::vec2(event.Motion().GetPosition()) / glm::vec2(mScreenSize);
        pos.y = 1.0f - pos.y;
        pos *= mGameScreenSize;
        mGame->EventMousePos(pos);
      }
      if(event.GetType() == EventType::KEYDOWN || event.GetType() == EventType::KEYUP) {
        mGame->EventKeyPress(event.Key().GetScanCode(), event.Key().GetType());
      }
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
    cb::gl::clear(cb::gl::ClearBuffer::COLOR);

    auto gprog = cb::gl::bind(*mGLProgram);

    auto trans = glm::ortho(0.0f, mGameScreenSize.x, 0.0f, mGameScreenSize.y);
    mGame->Render(*mGLProgram, trans);
  }

  cb::gl::CShader CApp::LoadShader(cb::gl::ShaderType const type, cb::string const & filepath) {
    using namespace cb::gl;

    cb::info(L"Compiling shader from file {0}", filepath);
    auto source = cb::readtextfileutf8(filepath);

    auto sh = CShader(type, source);
    if(!sh.IsCompiled()) {
      cb::warn(L"Shader compilation failed.");
      cb::info(sh.GetCompileLog());
      throw std::exception("Shader compilation failed.");
    }

    return sh;
  }
}
