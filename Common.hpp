#pragma once

#include <Windows.h>
#include <d3d11.h>

#include <stdexcept>

#include <filesystem>
#include <iostream>
#include <fstream>

#include <functional>
#include <memory>
#include <thread>
#include <cstdarg>

#include <array>
#include <map>
#include <ranges>
#include <stack>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>
#include <algorithm> 
#include <excpt.h>
#include <future>
#include <regex>
#include <xutility>

#include <string_view>

#include "Logger/Logger.hpp"
#include "Utility/Obfuscation.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <JSON/json.hpp>
#include <WorldShield/WorldShield.hpp>

namespace Sentinel
{
    extern HANDLE gMainThread;
    extern HINSTANCE gInstance;

    extern bool gRunning;
    inline bool gShouldCheck = true;
    inline bool gAtomFound = false;
    inline bool gResetVFX = false;
    typedef unsigned __int64 QWORD;

    using namespace std::chrono_literals;
}