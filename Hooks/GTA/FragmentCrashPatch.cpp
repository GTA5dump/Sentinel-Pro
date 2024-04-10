#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	bool GTA::FragmentCrashPatch(std::uintptr_t a1, std::uint32_t a2, std::uintptr_t a3, std::uintptr_t a4, std::uintptr_t a5)
	{
		if (g_ProtectionScript->m_FragmentCrash && g_ProtectionScript->m_SyncingPlayer != nullptr) {
			if (g_ProtectionScript->m_SyncingPlayer->is_valid()) {
				if (const auto Pointer = *reinterpret_cast<std::uintptr_t*>(a5 + 0x70); Pointer) {
					if (const auto Pointer2 = *reinterpret_cast<std::uintptr_t*>(Pointer + 8 * a2); !Pointer2) {
						if (g_ProtectionScript->m_ManualPatchesNotify)
							Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "Fragment Crash");
						return false;
					}
				}
			}
		}
		return Hooking::GetHook<DetourHook>("FCP")->Original<decltype(&FragmentCrashPatch)>()(a1, a2, a3, a4, a5);
	}

	bool GTA::FragmentCrashPatch2(float* a1, float* a2)
	{
		if (g_ProtectionScript->m_FragmentCrash && g_ProtectionScript->m_SyncingPlayer != nullptr) {
			if (g_ProtectionScript->m_SyncingPlayer->is_valid()) {
				if (!a1 || !a2) {
					if (g_ProtectionScript->m_ManualPatchesNotify)
						Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "Fragment Crash");
					return false;
				}
			}
		}
		return Hooking::GetHook<DetourHook>("FCP2")->Original<decltype(&FragmentCrashPatch2)>()(a1, a2);
	}
}