#pragma once

#include <memory>

namespace core {
  using InputDeviceEventId = unsigned;
  using InputDeviceId = unsigned;

  class IInputDeviceEventSink;

  class IInputDevice {
  protected:
    InputDeviceId mId;
    cb::string mName;
    std::weak_ptr<IInputDeviceEventSink> mSink;

    IInputDevice(const InputDeviceId id, const cb::string& name)
      : mId(id), mName(name)
    {}

  public:
    IInputDevice(IInputDevice const&) = delete;
    virtual ~IInputDevice() {}

    InputDeviceId GetId() const { return mId; }
    const cb::string& GetName() const { return mName; }

    virtual bool ParseEventId(const cb::string& value,
                              InputDeviceEventId& outValue) const = 0;

    virtual cb::string EventIdToString(const InputDeviceEventId value) const = 0;

    void SetEventSink(std::weak_ptr<IInputDeviceEventSink> sink) { mSink = sink; }
  };
}