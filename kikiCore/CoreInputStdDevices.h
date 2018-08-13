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
    InputDeviceId mDevId;

  public:
    CInputKeyboardDevice(CAppBase& app, InputDeviceId devId);
    ~CInputKeyboardDevice();

    void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) override;
  };
}