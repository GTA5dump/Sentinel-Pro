#include "../HookTable.hpp"
#include "../../Services/Players/PlayerService.hpp"

namespace Sentinel
{
	void* GTA::AssignPhysicalIndex(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, std::uint8_t idx)
	{
		const auto* netData = player->get_net_data();

		if (idx == static_cast<std::uint8_t>(-1))
		{
			// Player Left
			g_PlayerService->PlayerLeave(player);

			return Hooking::GetHook<DetourHook>("API")->Original<decltype(&AssignPhysicalIndex)>()(netPlayerMgr, player, idx);
		}

		// Player Joined
		const auto Result = Hooking::GetHook<DetourHook>("API")->Original<decltype(&AssignPhysicalIndex)>()(netPlayerMgr, player, idx);
		g_PlayerService->PlayerJoin(player);
		if (netData)
		{
			// Notify and shit
		}

		return Result;
	}
}