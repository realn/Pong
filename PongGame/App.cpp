#include "stdafx.h"
#include "App.h"
#include "Vertex.h"
#include "Consts.h"
#include "Game.h"
#include "Font.h"
#include "Canvas.h"

#include "GUILabel.h"
#include "GUIWidget.h"
#include "GUIRect.h"
#include "GUIRenderContext.h"

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
    {
      auto glProg = CreateShaderProgram(L"font_vs.glsl"s, L"font_fs.glsl"s);
      mFontProgram = std::make_unique<CProgram>(std::move(glProg));
    }

    auto aspect = static_cast<float>(mScreenSize.x) / static_cast<float>(mScreenSize.y);
    mGameScreenSize = glm::vec2(2.0f) * glm::vec2(aspect, 1.0f);

    mGame = std::make_unique<CGame>(mGameScreenSize);

    mFont = std::make_unique<gfx::CFont>(gfx::CFont::Load(L"font.xml"));

    auto texAtlas = gfx::CTextureAtlas(L"base.png", glm::uvec2(256));
    mCanvas = std::make_unique<gfx::CCanvas>(mFont->GetTexture(),
                                             mFont->GetTexture(),
                                             texAtlas);

    {
      mGUIWidget = std::make_unique<gui::CRect>(gui::CWidget::NoId);
    }
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

      mFrameTime += mTimer->GetTimeDelta();
      for(auto i = 0u; i < UPDATE_MAX_STEPS && mFrameTime > UPDATE_TIME_STEP; i++) {
        Update(UPDATE_TIME_STEP);
        mFrameTime -= UPDATE_TIME_STEP;
      }

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

  void CApp::Update(float const timeDelta) {
    mGame->Update(timeDelta);
  }

  void CApp::UpdateRender() {
    mGame->UpdateRender();
  }

  void CApp::Render() {
    cb::gl::clearColor({0.1f, 0.1f, 0.1f, 1.0f});
    cb::gl::clear(cb::gl::ClearBuffer::COLOR);

    {
      auto trans = glm::ortho(0.0f, mGameScreenSize.x, 0.0f, mGameScreenSize.y);
      auto gprog = cb::gl::bind(*mGLProgram);

      mGame->Render(*mGLProgram, trans);
    }

    {
      mCanvas->Clear();

      auto ctx = gui::CRenderContext{*mCanvas, *mFont};
      mGUIWidget->UpdateRender(ctx, mGameScreenSize);
      mGUIWidget->Render(ctx, {0.0f, 0.0f});

      auto buf = mCanvas->CreateVertexBuffer();

      auto trans = glm::ortho(0.0f, mGameScreenSize.x, mGameScreenSize.y, 0.0f);
      auto gprog = cb::gl::bind(*mFontProgram);
      auto gtex0 = cb::gl::bind(*mCanvas->GetBaseTexture(), 0);
      auto gtex1 = cb::gl::bind(*mCanvas->GetFontTexture(), 1);
      auto gbuf = cb::gl::bind(buf);
      auto gdef = cb::gl::bind(gfx::CVertex::Def);

      mFontProgram->SetUniform(gfx::UNI_TRANSFORM, trans);
      mFontProgram->SetUniform(gfx::UNI_BASE_TEXTURE, 0);
      mFontProgram->SetUniform(gfx::UNI_FONT_TEXTURE, 1);

      {
        auto blend = cb::gl::CBlendState();
        blend.SrcFactor = cb::gl::BlendFactor::SRC_ALPHA;
        blend.DstFactor = cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA;
        cb::gl::setState(blend);
      }

      auto gstate = cb::gl::bindStateEnabled(cb::gl::State::BLEND, true);
      cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, mCanvas->GetIndices());
    }
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
