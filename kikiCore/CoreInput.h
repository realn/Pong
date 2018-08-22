#pragma once

#include <vector>
#include <memory>
#include <map>
#include <set>

#include "CoreEvents.h"
#include "CoreInputDeviceEvent.h"
#include "CoreInputObserver.h"
#include "CoreInputEventRegistry.h"

namespace core {
  using InputDeviceId = unsigned;
  using InputDeviceEventId = unsigned;
  using InputEventId = unsigned;
  using InputBindingId = std::pair<InputDeviceId, InputDeviceEventId>;

  class IInputDevice;
  class CInputDeviceEvent;

  class CInput 
    : public IInputEventRegistry
    , public IInputDeviceEventSink
    , public IEventSource<IInputObserver>
    , public std::enable_shared_from_this<CInput>
  {
  private:
    class CBindData {
    public:
      InputEventId mEventId;
      bool mNegated = false;
      float mAddValue = 0.0f;
      float mMulValue = 0.0f;
    };

    using DevicesT = std::map<InputDeviceId, std::shared_ptr<IInputDevice>>;
    using DeviceNameIdsT = std::map<cb::string, InputDeviceId>;
    using DeviceEventsT = std::vector<CInputDeviceEvent>;
    using DeviceBindingsT = std::map<InputBindingId, CBindData>;
    using EventIdsT = std::set<InputEventId>;
    using EventNameIdsT = std::map<cb::string, InputEventId>;

    DevicesT mDevices;
    DeviceNameIdsT mDeviceNameIds;
    DeviceEventsT mDeviceEvents;
    DeviceBindingsT mBindings;
    EventIdsT mEventIds;
    EventNameIdsT mEventNameIds;

  public:
    CInput();

    void AddDevice(std::shared_ptr<IInputDevice> device);
    void RegisterEvent(const cb::string& name, const InputEventId eventId) override;

    bool AddBinding(const InputDeviceId devId, 
                    const InputDeviceEventId devEventId, 
                    const InputEventId eventId,
                    const bool negate = false,
                    const float addValue = 0.0f,
                    const float mulValue = 1.0f);
    bool AddBinding(const cb::string& devName, 
                    const cb::string& devEventName, 
                    const cb::string& eventName);

    void Update(float const timeDelta);

  private:
    // Inherited via IInputEventSink
    virtual void SendEventAction(InputDeviceId devId, InputDeviceEventId id) override;
    virtual void SendEventState(InputDeviceId devId, InputDeviceEventId id, bool state);
    virtual void SendEventRange(InputDeviceId devId, InputDeviceEventId id, float range, float prevRange);

    bool ModValue(const CBindData& data, const bool value) const;
    float ModValue(const CBindData& data, const float value) const;
  };
}