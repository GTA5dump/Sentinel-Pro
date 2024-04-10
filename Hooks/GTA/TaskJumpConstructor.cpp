#include "../HookTable.hpp"
#include "../../Features/Self.hpp"

namespace Sentinel
{
    std::uint64_t GTA::TaskJumpConstructor(std::uint64_t a1, int a2)
    {
        if (g_SelfFeatures->m_SuperJump || g_SelfFeatures->m_NinjaJump || g_SelfFeatures->m_SuperMan)
            a2 |= 1 << 15;
        if (g_SelfFeatures->m_BeastJump)
            a2 |= (1 << 15) | (1 << 17);
        return Hooking::GetHook<DetourHook>("TJC")->Original<decltype(&TaskJumpConstructor)>()(a1, a2);
    }
}