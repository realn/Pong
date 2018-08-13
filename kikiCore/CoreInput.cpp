#include "stdafx.h"
#include "CoreInput.h"
#include "CoreInputDevice.h"

namespace core {
  CInput::CInput() {}

  void CInput::AddDevice(std::shared_ptr<IInputDevice> device) {
    device->SetEventSink(this->weak_from_this());
    mDevices.push_back(device);
  }

  void CInput::AddBinding(InputDeviceId devId, InputDeviceEventId devEventId, InputEventId eventId) {
    InputBindingId bindId(devId, devEventId);
    mBindings[bindId] = eventId;
  }

  void CInput::Update(float const timeDelta) {
    auto observers = IEventSource<IInputTarget>::GetObservers();

    for(auto event : mDeviceEvents) {
      InputBindingId bindId(event.mDevId, event.mId);

      auto it = mBindings.find(bindId);
      if(it == mBindings.end()) {
        continue;
      }

      for(auto observer : observers) {
        switch(event.mType) {
        default:
        case InputEventType::Action:  observer->OnInputAction(it->second); break;
        case InputEventType::State:   observer->OnInputState(it->second,
                                                             event.mState.mValue);
          break;
        case InputEventType::Range:   observer->OnInputRange(it->second,
                                                             event.mRange.mValue,
                                                             event.mRange.mPrevValue);
          break;
        }
      }
    }
    mDeviceEvents.clear();
  }

  void CInput::SendEventAction(InputDeviceId devId, InputDeviceEventId id) {
    mDeviceEvents.push_back(CInputDeviceEvent(devId, 
                                              id, 
                                              InputEventType::Action));
  }

  void CInput::SendEventState(InputDeviceId devId, InputDeviceEventId id, bool state) {
    mDeviceEvents.push_back(CInputDeviceEvent(devId,
                                              id,
                                              InputEventType::State,
                                              state, !state));
  }

  void CInput::SendEventRange(InputDeviceId devId, InputDeviceEventId id, float range, float prevRange) {
    mDeviceEvents.push_back(CInputDeviceEvent(devId,
                                              id,
                                              InputEventType::Range,
                                              range, prevRange));
  }

}
