#pragma once

namespace core {
  class CAppConfig;

  class IAppTask;
  class IAppMouseEvents;
  class IAppKeyEvents;

  class CBSphere;
  class CBLine;
  class CBRect;
  class CFont;
  class CFontRepository;

  class CApp;

  template<typename _Type> class CAssetRepository;
  template<typename _ObserverType> class IEventSource;
  template<typename _ObserverType> class IEventTarget;
}
