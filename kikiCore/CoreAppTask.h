#pragma once

#include <CBStr/Defines.h>
#include <CBSDL/Fwd.h>
#include <glm/fwd.hpp>

namespace core {
  class CAppConfig;
  class CAppBase;

  class IAppTask {
  protected:
    IAppTask() {}
    IAppTask(IAppTask const&) = delete;
    IAppTask(IAppTask&&) = default;

  public:
    virtual ~IAppTask() = default;

    virtual void PrepareConfig(cb::strvector const& args, CAppConfig& config) {}
    virtual bool Init(CAppBase& app) = 0;

    virtual void Update(CAppBase& app, float const timeDelta) = 0;
    virtual void UpdateRender(CAppBase const& app, float const timeDelta) = 0;
    virtual void Render(CAppBase const& app) const = 0;

  };
}