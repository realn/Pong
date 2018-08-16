#include "stdafx.h"

#include <CBSDL/Consts.h>
#include <CBSDL/Funcs.h>

#include "CoreApp.h"
#include "CoreInputDeviceEvent.h"

#include "CoreInputStdDevices.h"

namespace core {
  constexpr float stateToRange(bool value) {
    return value ? 1.0f : 0.0f;
  }

  CInputKeyboardDevice::CInputKeyboardDevice(const InputDeviceId devId, const cb::string& name) 
    : IInputDevice(devId, name) {
    //core::bind<IAppKeyEvents>(app, *this);
  }

  CInputKeyboardDevice::~CInputKeyboardDevice() {}

  bool CInputKeyboardDevice::ParseEventId(const cb::string & value, InputDeviceEventId & outValue) const {
    cb::sdl::ScanCode code = cb::sdl::GetScanCodeFromName(value);
    if(code == cb::sdl::ScanCode::UNKNOWN)
      return false;

    outValue = static_cast<InputDeviceEventId>(code);
    return true;
  }

  cb::string CInputKeyboardDevice::EventIdToString(const InputDeviceEventId value) const {
    cb::sdl::ScanCode code = static_cast<cb::sdl::ScanCode>(value);
    return cb::sdl::GetNameFromScanCode(code);
  }

  void CInputKeyboardDevice::OnKeyState(cb::sdl::ScanCode const code, 
                                        cb::sdl::KeyState const state) {
    auto eventSink = mSink.lock();
    if(!eventSink) {
      return;
    }
    bool prevState = mKeys[code];
    bool thisState = state == cb::sdl::KeyState::PRESSED;
    mKeys[code] = thisState;

    auto eventId = static_cast<InputDeviceEventId>(code);

    if(prevState && !thisState) {
      eventSink->SendEventAction(mId, eventId);
    }
    if(prevState != thisState) {
      eventSink->SendEventState(mId, eventId, thisState);
      eventSink->SendEventRange(mId, eventId, stateToRange(thisState), stateToRange(prevState));
    }    
  }
}
