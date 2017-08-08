#pragma once

#include <glm/fwd.hpp>
#include <CBSDL/Fwd.h>

namespace pong {
  class IMouseEventObserver {
  protected:
    IMouseEventObserver() {}
  public:
    virtual ~IMouseEventObserver() {}

    virtual void EventMouseMove(glm::vec2 const& pos) {}
  };

  class IKeyboardEventObserver {
  protected:
    IKeyboardEventObserver() {}
  public:
    virtual ~IKeyboardEventObserver() {}

    virtual void EventKeyPress(cb::sdl::ScanCode key, cb::sdl::KeyState state) {}
  };
}