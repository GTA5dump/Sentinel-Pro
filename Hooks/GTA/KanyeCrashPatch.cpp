#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	std::uint64_t GTA::KanyeCrashPatch(std::uintptr_t a1, bool a2)
	{
        if (g_ProtectionScript->m_KanyeCrash && g_ProtectionScript->m_SyncingPlayer != nullptr) {
            if (g_ProtectionScript->m_SyncingPlayer->is_valid()) { 
                if (const auto Pointer = *reinterpret_cast<std::uintptr_t*>(a1 + 0x10); a2 && Pointer) {
                    if (const auto Pointer2 = *reinterpret_cast<std::uintptr_t*>(Pointer + 0x10F0); Pointer2) {
                        if (const auto Pointer3 = *reinterpret_cast<std::uintptr_t*>(Pointer2 + 0x10); Pointer3) {
                            if (const auto Pointer4 = *reinterpret_cast<std::uintptr_t*>(Pointer3 + 0x20); Pointer4) {
                                if (const auto Pointer5 = *reinterpret_cast<std::uintptr_t*>(Pointer4 + 0x90); !Pointer5) {
                                    if (g_ProtectionScript->m_ManualPatchesNotify)
                                        Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "Kanye Crash");
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        return Hooking::GetHook<DetourHook>("KCP")->Original<decltype(&KanyeCrashPatch)>()(a1, a2);
	}
}