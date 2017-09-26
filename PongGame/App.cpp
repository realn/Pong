#include "stdafx.h"
#include "App.h"
#include "Consts.h"
#include "Game.h"
#include "GFXCanvas.h"
#include "GFXCanvasView.h"

#include "GUILabel.h"
#include "GUIWidget.h"
#include "GUIRect.h"
#include "GUIPanel.h"
#include "GUIStackPanel.h"
#include "GUIScreen.h"
#include "GUIScreenView.h"

#include <CBSDL/System.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Timer.h>
#include <CBGL/System.h>
#include <CBGL/Shader.h>
#include <CBGL/Texture.h>
#include <CBGL/State.h>
#include <CBIO/File.h>

#include <CoreFont.h>

namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs)
    : mRun(true)
    , mScreenSize(800, 480) {
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
      auto glProg = CreateShaderProgram(L"main_vs.glsl"s, L"main_fs.glsl"s);
      mGLProgram = std::make_unique<CProgram>(std::move(glProg));
    }

    auto aspect = static_cast<float>(mScreenSize.x) / static_cast<float>(mScreenSize.y);
    mGameScreenSize = glm::vec2(2.0f) * glm::vec2(aspect, 1.0f);

    mGame = std::make_unique<CGame>(mGameScreenSize);

    {
      mFont = std::make_shared<core::CFont>(core::CFont::Load(L"font.xml"s));

      mScreen = std::make_unique<gui::CScreen>(gui::CScreen::Load(L"gameui.xml"s));

      auto texAtlas = gfx::CTextureAtlas(L"texture.png"s, glm::uvec2(256));

      auto cnvProg = 
        std::make_shared<cb::gl::CProgram>(CreateShaderProgram(L"font_vs.glsl"s, L"font_fs.glsl"s));

      mScreenView = std::make_unique<gui::CScreenView>(mFont, cnvProg, texAtlas);
    }
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
        mGame->EventMousePos(pos);
      }
      if(event.GetType() == EventType::KEYDOWN || event.GetType() == EventType::KEYUP) {
        mGame->EventKeyPress(event.Key().GetScanCode(), event.Key().GetType());
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

    mScreen->Update(timeDelta);
  }

  void CApp::UpdateRender() {
    mGame->UpdateRender();

    auto canvas = mScreenView->CreateCanvas();
    mScreen->UpdateRender(canvas, mScreenView->GetFont());
    mScreenView->UpdateRender(*mScreen, canvas);
  }

  void CApp::Render() {
    cb::gl::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
    cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

    {
      auto trans = glm::ortho(0.0f, mGameScreenSize.x, 0.0f, mGameScreenSize.y);
      auto gprog = cb::gl::bind(*mGLProgram);

      mGame->Render(*mGLProgram, trans);
    }

    mScreenView->Render();
  }

  cb::gl::CProgram CApp::CreateShaderProgram(cb::string const & vertFilePath, cb::string const & fragFilePath) {
    using namespace cb::gl;

    auto vsh = LoadShader(ShaderType::VERTEX, vertFilePath);
    auto fsh = LoadShader(ShaderType::FRAGMENT, fragFilePath);

    auto glProgram = CProgram();
    glProgram.Attach({std::move(vsh), std::move(fsh)});
    glProgram.SetInLocation({
      {gfx::IDX_VERTEX4, gfx::VIN_VERTEX4},
      {gfx::IDX_COLOR4, gfx::VIN_COLOR4}
    });
    glProgram.Link();
    if(!glProgram.IsLinked()) {
      cb::warn(L"Failed to link gl program.");
      cb::info(glProgram.GetLinkLog());
      throw std::exception("GL program compilation failed.");
    }

    return glProgram;
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
