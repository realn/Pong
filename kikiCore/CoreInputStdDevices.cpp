#include "stdafx.h"

#include <CBSDL/Consts.h>

#include "CoreApp.h"
#include "CoreInputDeviceEvent.h"

#include "CoreInputStdDevices.h"

namespace core {
  constexpr float stateToRange(bool value) {
    return value ? 1.0f : 0.0f;
  }

  CInputKeyboardDevice::CInputKeyboardDevice(InputDeviceId devId) 
    : mDevId(devId) {
    //core::bind<IAppKeyEvents>(app, *this);
  }

  CInputKeyboardDevice::~CInputKeyboardDevice() {}

  void CInputKeyboardDevice::OnKeyState(cb::sdl::ScanCode const code, 
                                        cb::sdl::KeyState const state) {
    auto eventSink = mSink.lock();
    if(!eventSink) {
      return;
    }
    bool prevState = mKeys[code];
    bool thisState = state == cb::sdl::KeyState::PRESSED;

    auto eventId = static_cast<InputDeviceEventId>(code);

    if(prevState && !thisState) {
      eventSink->SendEventAction(mDevId, eventId);
    }
    if(prevState != thisState) {
      eventSink->SendEventState(mDevId, eventId, thisState);
      eventSink->SendEventRange(mDevId, eventId, stateToRange(thisState), stateToRange(prevState));
    }    
  }
}
