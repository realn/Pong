#include "App.h"
#include "Vertex.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <CBLog/Log.h>
#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/Events.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Timer.h>
#include <CBGL/Rendering.h>
#include <CBGL/System.h>
#include <CBGL/Shader.h>
#include <CBGL/Program.h>
#include <CBGL/Buffer.h>
#include <CBIO/File.h>

namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs)
    : mRun(true) 
    , mScreenSize(800, 480)
    , mVec(0.1f)
  {
    using namespace cb::sdl;
    using namespace cb::gl;

    cb::info(L"Application initialization.");

    cb::info(L"Creating window");
    mSDLSystem = std::make_unique<CSystem>(System::Video);
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
      mGLProgram->SetInLocation({{0, L"vInVertex"}, {1, L"vInColor"}});
      mGLProgram->Link();
      if(!mGLProgram->IsLinked()) {
        cb::warn(L"Failed to link gl program.");
        cb::info(mGLProgram->GetLinkLog());
        throw std::exception("GL program compilation failed.");
      }
    }

    {
      auto verts = std::vector<CVertex>{
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
      };

      mGLBuffer = std::make_unique<CBuffer>();
      mGLBuffer->SetData(verts);
    }

    float aspect = static_cast<float>(mScreenSize.x) / static_cast<float>(mScreenSize.y);
    mGameScreenSize = glm::vec2(2.0f) * glm::vec2(aspect, 1.0f);
  }

  CApp::CApp(CApp && other) {
    std::swap(mSDLSystem, other.mSDLSystem);
    std::swap(mWindow, other.mWindow);
    std::swap(mGLContext, other.mGLContext);
    std::swap(mGLProgram, other.mGLProgram);
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
    }
  }

  void CApp::Update(float const timeDelta) {
    auto size = glm::vec2(1.0f);
    auto next = mPos + mVec * timeDelta;
    if(next.x < 0.0f || next.x > mGameScreenSize.x - size.x) {
      mVec.x = -mVec.x;
    }
    if(next.y < 0.0f || next.y > mGameScreenSize.y - size.y) {
      mVec.y = -mVec.y;
    }
    mPos += mVec * timeDelta;
  }

  void CApp::UpdateRender() {}

  void CApp::Render() {
    cb::gl::clearColor({0.2f, 0.2f, 0.2f, 1.0f});
    cb::gl::clear(cb::gl::ClearBuffer::COLOR);

    auto gprog = cb::gl::bind(*mGLProgram);
    auto gbuf = cb::gl::bind(*mGLBuffer);
    auto gvert = cb::gl::bind(CVertex::Def);

    auto trans = glm::ortho(0.0f, mGameScreenSize.x, 0.0f, mGameScreenSize.y);
    auto movem = glm::translate(glm::mat4(1.0f), glm::vec3(mPos, 0.0f));
    mGLProgram->SetUniform(L"mTransform", trans * movem);

    cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, std::vector<glm::u16>{0, 1, 2, 0, 2, 3});
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
