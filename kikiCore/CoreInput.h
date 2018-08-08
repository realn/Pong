#pragma once

#include <vector>
#include <memory>

#include "CoreInputDeviceEvent.h"

namespace core {
  class IInputDevice;
  class CInputDeviceEvent;

  class CInput 
    : public IInputDeviceEventSink
  {
  private:
    using DevicesT = std::vector<std::shared_ptr<IInputDevice>>;
    using EventsT = std::vector<CInputDeviceEvent>;

    DevicesT mDevices;
    EventsT mEvents;

  public:
    CInput();

    void AddDevice(std::shared_ptr<IInputDevice> device);

    // Inherited via IInputEventSink
    virtual bool SendEvent(CInputDeviceEvent && event) override;

    void Update(float const timeDelta);
  };
}