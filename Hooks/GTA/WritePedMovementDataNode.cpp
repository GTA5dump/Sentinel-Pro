#include "../HookTable.hpp"
#include "../../Features/Network.hpp"

namespace Sentinel
{
	std::uint64_t GTA::WritePedMovementDataNode(CPedMovementDataNode* node, rage::netObject* object)
	{
		if (g_NetworkFeatures->m_EnableMovementSpoofing)
		{
			node->m_has_desired_move_blend_ratio_x = false;
			node->m_has_desired_move_blend_ratio_y = false;
		}

		return Hooking::GetHook<DetourHook>("WPMDN")->Original<decltype(&WritePedMovementDataNode)>()(node, object);
	}
}