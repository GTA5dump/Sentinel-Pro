#include "../HookTable.hpp"

namespace Sentinel
{
	bool GTA::SendChatMessage(void* team_mgr, rage::rlGamerInfo* local_gamer_info, char* message, bool team)
	{
		return Hooking::GetHook<DetourHook>("SCM")->Original<decltype(&SendChatMessage)>()(team_mgr, local_gamer_info, message, team);
	}
}