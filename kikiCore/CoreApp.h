#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include <CBStr/Defines.h>
#include <CBSDL/Timer.h>
#include <CBSDL/System.h>
#include <CBSDL/Fwd.h>

#include "CoreEvents.h"

namespace core {
  class CAppConfig {
  public:
    float UpdateTimeStep = 0.025f;
    cb::u32 UpdateMaxStepsPerFrame = 20;
    cb::u32 EventMaxIters = 20;
    cb::string WindowTitle = L"App Title";
    glm::ivec2 WindowPos = {20, 20};
    glm::uvec2 WindowSize = {800, 480};
    cb::s32 GfxBufferSize = 32;
    cb::s32 GfxDepthSize = 24;
    cb::s32 GfxStencilSize = 8;

    float GetAspectRatio() const {
      return static_cast<float>(WindowSize.x) / static_cast<float>(WindowSize.y);
    }
  };
  
  class CInput;
  class CInputKeyboardDevice;
  class CScene;

  class CApp {
  private:
    cb::strvector mArgs;
    CAppConfig mConfig;
    cb::sdl::CSystem mSystem;
    cb::sdl::CPerfTimer mTimer;
    bool mRun = true;

    std::unique_ptr<cb::sdl::CWindow> mWindow;
    std::unique_ptr<cb::sdl::CGLContext> mGLContext;

    std::shared_ptr<CInput> mInput;  

    std::unique_ptr<CScene> mScene;

  public:
    CApp(cb::strvector const& args, CAppConfig const& config = CAppConfig());
    CApp(CApp&&) = default;
    virtual ~CApp();

    int Execute();

    void Quit() { mRun = false; }

    CAppConfig const& GetConfig() const { return mConfig; }
    const cb::strvector& GetArgs() const { return mArgs; }
    CInput& GetInput() const { return *mInput; }

  private:
    void MainLoop();

    bool InitBase();

    void ProcessEvents();
    void Update(float& frameTime);
    void UpdateFrame(float const timeDelta);
    void UpdateRender(float const timeDelta);
    void Render();

    void ProcessWindowEvent(cb::sdl::CEvent const& event);
    void ProcessMouseEvent(cb::sdl::CEvent const& event);
    void ProcessKeyEvent(cb::sdl::CEvent const& event);

  protected:
    virtual bool AdjustConfig(CAppConfig& config) { return true; }
    virtual bool Init() = 0;
    virtual std::unique_ptr<CScene> CreateScene() = 0;

    virtual void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) {}
    virtual void OnMouseMotion(glm::vec2 const & pos, glm::vec2 const & delta) {}
    virtual void OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) {}

    virtual void OnAppClose() {}
  };
}
