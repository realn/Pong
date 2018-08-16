#include "stdafx.h"

#include <CBStr/Defines.h>

#include "CoreInputDevice.h"

#include "CoreInput.h"

namespace core {
  CInput::CInput() {}

  void CInput::AddDevice(std::shared_ptr<IInputDevice> device) {
    device->SetEventSink(this->weak_from_this());
    mDevices[device->GetId()] = device;
    mDeviceNameIds[device->GetName()] = device->GetId();
  }

  void CInput::RegisterEvent(const cb::string & name, const InputEventId eventId) {
    mEventIds.insert(eventId);
    mEventNameIds[name] = eventId;
  }

  bool CInput::AddBinding(const InputDeviceId devId, 
                          const InputDeviceEventId devEventId, 
                          const InputEventId eventId) {
    if(mDevices.find(devId) == mDevices.end() ||
       mEventIds.find(eventId) == mEventIds.end()) {
      return false;
    }
    InputBindingId bindId(devId, devEventId);
    mBindings[bindId] = eventId;
    return true;
  }

  bool CInput::AddBinding(const cb::string& devName, 
                          const cb::string & devEventName, 
                          const cb::string & eventName) {
    auto eventIt = mEventNameIds.find(eventName);
    if(eventIt == mEventNameIds.end()) {
      return false;
    }
    auto devIdIt = mDeviceNameIds.find(devName);
    if(devIdIt == mDeviceNameIds.end()) {
      return false;
    }
    auto devIt = mDevices.find(devIdIt->second);
    if(devIt == mDevices.end()) {
      return false;
    }
    InputDeviceEventId devEventId = 0;
    if(!devIt->second->ParseEventId(devEventName, devEventId)) {
      return false;
    }
    return AddBinding(devIdIt->second, devEventId, eventIt->second);
  }

  void CInput::Update(float const timeDelta) {
    auto observers = IEventSource<IInputObserver>::GetObservers();

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
