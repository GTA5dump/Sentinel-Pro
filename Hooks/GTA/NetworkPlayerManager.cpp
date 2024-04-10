#include "../HookTable.hpp"
#include "../../GTA/Include.hpp"
#include "../../Services/Players/PlayerService.hpp"

namespace Sentinel
{
	void GTA::NetworkPlayerManagerInitialize(CNetworkPlayerMgr* _this, std::uint64_t a2, std::uint32_t a3, std::uint32_t a4[4])
	{
		Hooking::GetHook<DetourHook>("NPMI")->Original<decltype(&NetworkPlayerManagerInitialize)>()(_this, a2, a3, a4);

		g_PlayerService->PlayerJoin(_this->m_local_net_player);
	}

	void GTA::NetworkPlayerManagerDestroy(CNetworkPlayerMgr* _this)
	{
		g_PlayerService->DoCleanup();

		Hooking::GetHook<DetourHook>("NPMD")->Original<decltype(&NetworkPlayerManagerDestroy)>()(_this);
	}
}