#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	std::uint64_t GTA::InvalidDecalCrashPatch(std::uintptr_t a1, int a2)
	{
		if (g_ProtectionScript->m_InvalidDecalCrash && g_ProtectionScript->m_SyncingPlayer != nullptr) {
			if (g_ProtectionScript->m_SyncingPlayer->is_valid()) {
				if (a1 && a2 == 2) {
					if (const auto Pointer = *reinterpret_cast<std::uintptr_t*>((a1 + 0x48)); Pointer) {
						if (const auto Pointer2 = *reinterpret_cast<std::uintptr_t*>((Pointer + 0x30)); Pointer2) {
							if (*reinterpret_cast<std::uintptr_t*>(Pointer2 + 0x2C8) == 0) {
								if (g_ProtectionScript->m_ManualPatchesNotify)
									Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "Invalid Decal Crash");
								return 0;
							}
						}
					}
				}
			}
		}
		return Hooking::GetHook<DetourHook>("IDCP")->Original<decltype(&InvalidDecalCrashPatch)>()(a1, a2);
	}
}