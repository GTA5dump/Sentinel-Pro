#include "../HookTable.hpp"
#include "../../Features/Network.hpp"

namespace Sentinel
{
	std::uint64_t GTA::WritePedOrientationDataNode(std::uint64_t net_obj, CPedOrientationDataNode* node)
	{
		const auto Return = Hooking::GetHook<DetourHook>("WPODN")->Original<decltype(&WritePedOrientationDataNode)>()(net_obj, node);
		if (g_NetworkFeatures->m_EnableMovementSpoofing)
		{
			static float value = 0.f;
			value += 15;

			if (value > 180.f)
				value = -180.f + (value - 180.f);
			node->m_has_desired_heading_x = true;
			node->m_desired_heading_x = value * 0.0174532924f;
			return true;
		}
		
		return Return;
	}
}