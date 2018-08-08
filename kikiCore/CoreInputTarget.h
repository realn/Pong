#pragma once

namespace core {
  using InputEventId = unsigned;

  class IInputTarget {
  public:
    virtual void OnInputAction(InputEventId const id) {};
    virtual void OnInputState(InputEventId const id, bool value) {}
    virtual void OnInputRange(InputEventId const id, float const value, float const prevValue) {}
  };
}