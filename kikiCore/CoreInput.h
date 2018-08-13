#pragma once

#include <vector>
#include <memory>
#include <map>

#include "CoreEvents.h"
#include "CoreInputDeviceEvent.h"
#include "CoreInputTarget.h"

namespace core {
  using InputDeviceId = unsigned;
  using InputDeviceEventId = unsigned;
  using InputEventId = unsigned;
  using InputBindingId = std::pair<InputDeviceId, InputDeviceEventId>;

  class IInputDevice;
  class CInputDeviceEvent;

  class CInput 
    : public IInputDeviceEventSink
    , public IEventSource<IInputTarget>
    , public std::enable_shared_from_this<CInput>
  {
  private:
    using DevicesT = std::vector<std::shared_ptr<IInputDevice>>;
    using DeviceEventsT = std::vector<CInputDeviceEvent>;
    using DeviceBindingsT = std::map<InputBindingId, InputEventId>;

    DevicesT mDevices;
    DeviceEventsT mDeviceEvents;
    DeviceBindingsT mBindings;

  public:
    CInput();

    void AddDevice(std::shared_ptr<IInputDevice> device);

    void AddBinding(InputDeviceId devId, InputDeviceEventId devEventId, InputEventId eventId);

    void Update(float const timeDelta);

  private:
    // Inherited via IInputEventSink
    virtual void SendEventAction(InputDeviceId devId, InputDeviceEventId id) override;
    virtual void SendEventState(InputDeviceId devId, InputDeviceEventId id, bool state);
    virtual void SendEventRange(InputDeviceId devId, InputDeviceEventId id, float range, float prevRange);
  };
}