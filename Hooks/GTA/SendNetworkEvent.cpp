#include "../HookTable.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	template<typename T, typename P1, typename P2>
	static T CallAddress(std::uint64_t address, P1 p1, P2 p2) { return ((T(*)(P1, P2))address)(p1, p2); }

	void GTA::SendNetworkEvent(std::uint64_t net_table, std::uint64_t event)
	{
		if (g_ProtectionScript->m_AntiCheatEvents && event)
		{
			short Type = *(short*)(event + 8);
			if (Type == (int)eNetworkEvents::REPORT_MYSELF_EVENT) {
				if (g_ProtectionScript->m_MiscellaneousNotify)
					Overlays::PushWarning("Anti Cheat Protection", "You're doing something shady, stop it. [REPORT_MYSELF_EVENT]", 5000);

				std::uint64_t Table = *(std::uint64_t*)event;

				CallAddress<int>(*(std::uint64_t*)Table, event, 1); // Deallocate event
				return;
			}

			if (Type == (int)eNetworkEvents::REPORT_CASH_SPAWN_EVENT) {
				if (g_ProtectionScript->m_MiscellaneousNotify)
					Overlays::PushWarning("Anti Cheat Protection", "You're doing something shady, stop it. [REPORT_CASH_SPAWN_EVENT]", 5000);

				std::uint64_t Table = *(std::uint64_t*)event;

				CallAddress<int>(*(std::uint64_t*)Table, event, 1); // Deallocate event
				return;
			}

			if (Type == (int)eNetworkEvents::NETWORK_CHECK_CODE_CRCS_EVENT) {
				if (g_ProtectionScript->m_MiscellaneousNotify)
					Overlays::PushWarning("Anti Cheat Protection", "You're doing something shady, stop it. [NETWORK_CHECK_CODE_CRCS_EVENT]", 5000);

				std::uint64_t Table = *(std::uint64_t*)event;

				CallAddress<int>(*(std::uint64_t*)Table, event, 1); // Deallocate event
				return;
			}
		}

		return Hooking::GetHook<DetourHook>("SNE")->Original<decltype(&SendNetworkEvent)>()(net_table, event);
	}
}