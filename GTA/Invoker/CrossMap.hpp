#pragma once
#include "../../Common.hpp"
#include "../Include.hpp"

namespace Sentinel
{
    constexpr auto gNativeCount = 6494;
    using CrossMap = std::array<rage::scrNativePair, gNativeCount>;
    extern CrossMap gCrossMap;
}