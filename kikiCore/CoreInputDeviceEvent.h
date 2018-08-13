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
    virtual ~IInputDeviceEventSink() {}

    virtual void SendEventAction(InputDeviceId devId, InputDeviceEventId id) = 0;
    virtual void SendEventState(InputDeviceId devId, InputDeviceEventId id, bool state) = 0;
    virtual void SendEventRange(InputDeviceId devId, InputDeviceEventId id, float range, float prevRange) = 0;
  };
}
