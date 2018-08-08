#pragma once

#include <memory>

namespace core {
  class IInputDeviceEventSink;

  class IInputDevice {
  protected:
    std::weak_ptr<IInputDeviceEventSink> mSink;

    IInputDevice(std::weak_ptr<IInputDeviceEventSink> sink) : mSink(sink) {}

  public:
    IInputDevice(IInputDevice const&) = delete;
    virtual ~IInputDevice() {}


  };
}