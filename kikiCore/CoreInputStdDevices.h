#pragma once

#include "CoreAppEvents.h"
#include "CoreAppInputEvents.h"
#include "CoreInputDevice.h"

namespace core {
  class CInputKeyboardDevice
    : public IInputDevice 
    , public IEventTarget<IAppKeyEvents>
  {
    using KeyCodesT = std::map<cb::sdl::ScanCode, bool>;
  private:
    KeyCodesT mKeys;

  public:
    CInputKeyboardDevice(CAppBase& app, std::weak_ptr<IInputDeviceEventSink> sink);
    ~CInputKeyboardDevice();

    void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) override;
  };
}