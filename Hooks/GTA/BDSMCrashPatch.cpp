#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../Features/Weapon.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	void GTA::BDSMCrashPatch(__int64 a1, __int64 a2, unsigned int a3, int a4, __int64 a5)
	{
		if (g_ProtectionScript->m_BDSMCrash && !g_WeaponFeatures->m_RopeGun && g_ProtectionScript->m_SyncingPlayer != nullptr) {
			if (a3 <= 0 && g_ProtectionScript->m_SyncingPlayer->is_valid()) {
				// a3 = 1;
				if (g_ProtectionScript->m_ManualPatchesNotify)
					Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "BDSM Crash");
				return Hooking::GetHook<DetourHook>("BDSMCP")->Original<decltype(&BDSMCrashPatch)>()(a1, a2, 1, a4, a5);
			}
		}
		return Hooking::GetHook<DetourHook>("BDSMCP")->Original<decltype(&BDSMCrashPatch)>()(a1, a2, a3, a4, a5);
	}
}