#include "../HookTable.hpp"
#include "../../GTA/Crashes.hpp"

namespace Sentinel
{
	__int64 GTA::WritePedCreationDataNode(std::int64_t ptr, CPedCreationDataNode* node)
	{
		if (Crashes::g_PogoCrash)
		{
			node->m_model = rage::joaat("slod_human"); // Sync Crash Ped

			return true;
		}
		else
			return Hooking::GetHook<DetourHook>("WPCDN")->Original<decltype(&WritePedCreationDataNode)>()(ptr, node);
	}
}