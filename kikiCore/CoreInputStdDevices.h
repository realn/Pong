#pragma once

#include "CoreInputDevice.h"

namespace core {
  class CInputKeyboardDevice
    : public IInputDevice 
  {
    using KeyCodesT = std::map<cb::sdl::ScanCode, bool>;
  private:
    KeyCodesT mKeys;

  public:
    CInputKeyboardDevice(const InputDeviceId devId, const cb::string& name);
    ~CInputKeyboardDevice();

    virtual bool ParseEventId(const cb::string& value,
                              InputDeviceEventId& outValue) const override;

    virtual cb::string EventIdToString(const InputDeviceEventId value) const override;

    void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state);
  };
}