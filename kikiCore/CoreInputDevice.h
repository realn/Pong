#pragma once

#include <memory>

namespace core {
  class IInputDeviceEventSink;

  class IInputDevice {
  protected:
    std::weak_ptr<IInputDeviceEventSink> mSink;

    IInputDevice() {}

  public:
    IInputDevice(IInputDevice const&) = delete;
    virtual ~IInputDevice() {}

    void SetEventSink(std::weak_ptr<IInputDeviceEventSink> sink) { mSink = sink; }
  };
}