#pragma once

#include <glm/fwd.hpp>
#include <CBSDL/Fwd.h>

#include "CoreEvents.h"

namespace core {
  class IAppMouseEvents {
  public:
    IAppMouseEvents() = default;
    IAppMouseEvents(IAppMouseEvents const&) = delete;
    virtual ~IAppMouseEvents() = default;

    virtual void OnMouseMotion(glm::vec2 const& pos, glm::vec2 const& delta) = 0;
    virtual void OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) = 0;
  };

  class IAppKeyEvents {
  public:
    IAppKeyEvents() = default;
    IAppKeyEvents(IAppKeyEvents const&) = delete;
    virtual ~IAppKeyEvents() = default;

    virtual void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) = 0;
  };
}