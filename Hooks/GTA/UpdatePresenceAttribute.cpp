#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	bool GTA::UpdatePresenceAttributeInt(void* presence_data, int profile_index, char* attr, std::uint64_t value)
	{
		const auto Hash = rage::joaat(attr);
		if (g_ProtectionScript->m_BlockRIDJoin && (Hash == rage::joaat("gstok") || Hash == rage::joaat("gsid") || Hash == rage::joaat("gstype") || Hash == rage::joaat("gshost") || Hash == rage::joaat("gsjoin")))
		{
			return true;
		}
		return Hooking::GetHook<DetourHook>("UPAI")->Original<decltype(&UpdatePresenceAttributeInt)>()(presence_data, profile_index, attr, value);
	}

	bool GTA::UpdatePresenceAttributeString(void* presence_data, int profile_index, char* attr, char* value)
	{
		const auto Hash = rage::joaat(attr);
		if (g_ProtectionScript->m_BlockRIDJoin && Hash == rage::joaat("gsinfo"))
		{
			return true;
		}

		// Shouldn't have any side effects
		if (Hash == rage::joaat("peeraddr"))
		{
			value = (char*)"";
		}

		return Hooking::GetHook<DetourHook>("UPAS")->Original<decltype(&UpdatePresenceAttributeString)>()(presence_data, profile_index, attr, value);
	}
}