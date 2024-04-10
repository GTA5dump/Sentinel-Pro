#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	bool GTA::ReceivePickup(rage::netObject* object, void* unk, CPed* ped)
	{
		if (g_ProtectionScript->m_Pickups && g_ProtectionScript->m_SyncingPlayer != nullptr) {
			if (g_ProtectionScript->m_SyncingPlayer->is_valid()) {
				if (g_ProtectionScript->m_MiscellaneousNotify)
					Overlays::ProtectionMessage(g_ProtectionScript->m_SyncingPlayer->get_name(), "Pickup");
			}
			return false;
		}
		return Hooking::GetHook<DetourHook>("RPI")->Original<decltype(&ReceivePickup)>()(object, unk, ped);
	}
}