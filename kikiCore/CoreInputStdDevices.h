#pragma once

#include "CoreInputDevice.h"

namespace core {
  class CInputKeyboardDevice
    : public IInputDevice 
  {
    using KeyCodesT = std::map<cb::sdl::ScanCode, bool>;
  private:
    KeyCodesT mKeys;
    InputDeviceId mDevId;

  public:
    CInputKeyboardDevice(InputDeviceId devId);
    ~CInputKeyboardDevice();

    void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state);
  };
}