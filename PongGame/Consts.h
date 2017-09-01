#pragma once

#include <CBStr/Defines.h>

namespace pong {
  static const cb::string UNI_TRANSFORM = L"mTransform";
  static const cb::string UNI_BASE_TEXTURE = L"texBase";
  static const cb::string UNI_FONT_TEXTURE = L"texFont";
  static const cb::string VIN_VERTEX4 = L"vInVertex";
  static const cb::string VIN_COLOR4 = L"vInColor";

  constexpr auto IDX_VERTEX4 = 0u;
  constexpr auto IDX_COLOR4 = 1u;

  constexpr auto UPDATE_TIME_STEP = 0.005f;
  constexpr auto UPDATE_MAX_STEPS = 20u;
  constexpr auto UPDATE_MAX_EVENTS = 50u;
}