#pragma once

#include <CBStr/Defines.h>

namespace core {
  using InputEventId = unsigned;

  class IInputEventRegistry {
  public:
    virtual ~IInputEventRegistry() {}

    virtual void RegisterEvent(const cb::string& name, const InputEventId id) = 0;
  };
}