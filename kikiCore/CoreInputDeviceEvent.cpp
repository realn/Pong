#include "stdafx.h"
#include "CoreInputDeviceEvent.h"

namespace core {
  CInputDeviceEvent::CInputDeviceEvent(InputDeviceId devId, InputDeviceEventId id, InputEventType type) 
    : mDevId(devId)
    , mId(id)
    , mType(type)
  {}

  CInputDeviceEvent::CInputDeviceEvent(InputDeviceId devId, InputDeviceEventId id, InputEventType type, 
                           bool state, bool prevState) 
    : CInputDeviceEvent(devId, id, type)
  {
    this->mState.mValue = state;
    this->mState.mPrevValue = prevState;
  }

  CInputDeviceEvent::CInputDeviceEvent(InputDeviceId devId, InputDeviceEventId id, InputEventType type, 
                           float range, float prevRange) 
    : CInputDeviceEvent(devId, id, type)
  {
    this->mRange.mValue = range;
    this->mRange.mPrevValue = prevRange;
  }

}
