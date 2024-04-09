#include "Common.hpp"

namespace Sentinel
{
    HANDLE gMainThread{ };
    HINSTANCE gInstance{ nullptr };

    bool gRunning{ true };
}