#pragma once

namespace core {
  using InputDeviceId = unsigned;
  using InputDeviceEventId = unsigned;

  enum class InputEventType {
    Action = 0,
    State,
    Range
  };

  class CInputDeviceEvent {
  public:
    InputDeviceId mDevId;
    InputDeviceEventId mId;
    InputEventType mType;
    union {
      struct StateData {
        bool mValue;
        bool mPrevValue;
      } mState;
      struct RangeData {
        float mValue;
        float mPrevValue;
      } mRange;
    };

    CInputDeviceEvent() = default;
    CInputDeviceEvent(InputDeviceId devId, InputDeviceEventId id, InputEventType type);
    CInputDeviceEvent(InputDeviceId devId, InputDeviceEventId id, InputEventType type, bool state, bool prevState);
    CInputDeviceEvent(InputDeviceId devId, InputDeviceEventId id, InputEventType type, float range, float prevRange);
  };

  class IInputDeviceEventSink {
  public:
    virtual bool SendEvent(CInputDeviceEvent&& event) = 0;
  };
}