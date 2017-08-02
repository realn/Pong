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
#include <CBGL/Rendering.h>
#include <CBGL/System.h>
#include <CBGL/Shader.h>
#include <CBGL/Program.h>
#include <CBGL/Buffer.h>
#include <CBIO/File.h>

namespace pong {
  CApp::CApp(cb::strvector const & cmdLineArgs)
    : mRun(true) {
    using namespace cb::sdl;
    using namespace cb::gl;

    cb::info(L"Application initialization.");

    cb::info(L"Creating window");
    mSDLSystem = std::make_unique<CSystem>(System::Video);
    mWindow = std::make_unique<CWindow>(L"Pong",
                                        CWindow::PosCentered,
                                        glm::uvec2{800u, 480u},
                                        WindowFlag::OPENGL);

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
        {-0.5f, -0.2f},
        {0.5f, -0.2f},
        {0.0f, 0.5f},
      };

      mGLBuffer = std::make_unique<CBuffer>();
      mGLBuffer->SetData(verts);
    }

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

    auto gprog = cb::gl::bind(*mGLProgram);
    auto gbuf = cb::gl::bind(*mGLBuffer);
    auto gvert = cb::gl::bind(CVertex::Def);

    mGLProgram->SetUniform(L"mTransform", glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f));

    cb::gl::drawArrays(cb::gl::PrimitiveType::TRIANGLES, 3);
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
