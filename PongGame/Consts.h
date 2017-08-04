#pragma once

#include <CBStr/Defines.h>

namespace pong {
  namespace render {
    static const cb::string UNI_TRANSFORM = L"mTransform";
    static const cb::string VIN_VERTEX4 = L"vInVertex";
    static const cb::string VIN_COLOR4 = L"vInColor";

    constexpr unsigned IDX_VERTEX4 = 0;
    constexpr unsigned IDX_COLOR4 = 1;
  }
}