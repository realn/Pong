#pragma once

namespace core {
  class CAppBase;

  class IAppEvents {
  public:
    IAppEvents() = default;
    IAppEvents(IAppEvents const&) = delete;
    virtual ~IAppEvents() = default;

    virtual void OnAppClose(CAppBase& app) {}
    virtual void OnAppQuit(CAppBase& app) {}
  };
}