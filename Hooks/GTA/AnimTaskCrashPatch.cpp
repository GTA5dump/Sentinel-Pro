#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
    bool GTA::AnimTaskCrashPatch(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4, std::int64_t a5)
    {
        if (g_ProtectionScript->m_SweepCrash && g_ProtectionScript->m_SyncingPlayer != nullptr)
        {
            if (g_ProtectionScript->m_SyncingPlayer->is_valid()) {
                if (!GTA::Misc::IsValidPointer(a2)) {
                    if (g_ProtectionScript->m_ManualPatchesNotify)
                        Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "Sweep Crash");
                    return 0;
                }
            }
        }
        return Hooking::GetHook<DetourHook>("ATCP")->Original<decltype(&AnimTaskCrashPatch)>()(a1, a2, a3, a4, a5);
    }

}