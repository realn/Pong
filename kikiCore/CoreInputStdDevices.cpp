#include "stdafx.h"

#include <CBSDL/Consts.h>

#include "CoreApp.h"
#include "CoreInputDeviceEvent.h"

#include "CoreInputStdDevices.h"

namespace core {
  constexpr float stateToRange(bool value) {
    return value ? 1.0f : 0.0f;
  }

  CInputKeyboardDevice::CInputKeyboardDevice(CAppBase & app,
                                             std::weak_ptr<IInputDeviceEventSink> sink)
    : IInputDevice(sink)
  {
    core::bind<IAppKeyEvents>(app, *this);
  }

  CInputKeyboardDevice::~CInputKeyboardDevice() {}

  void CInputKeyboardDevice::OnKeyState(cb::sdl::ScanCode const code, 
                                        cb::sdl::KeyState const state) {
    auto eventSink = mSink.lock();
    bool prevState = mKeys[code];
    bool thisState = state == cb::sdl::KeyState::PRESSED;

    auto eventId = static_cast<InputDeviceEventId>(code);

    if(prevState && !thisState) {
      eventSink->SendEvent(CInputDeviceEvent(0, eventId, InputEventType::Action));
    }
    if(prevState != thisState) {
      eventSink->SendEvent(CInputDeviceEvent(0, eventId, InputEventType::State,
                                       thisState, prevState));
      eventSink->SendEvent(CInputDeviceEvent(0, eventId, InputEventType::Range,
                                       stateToRange(thisState), stateToRange(prevState)));
    }
    
  }

}
