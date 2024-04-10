#include "../HookTable.hpp"
#include "../../Features/Self.hpp"
#include "../../GTA/GTA.hpp"

namespace Sentinel
{
	std::uint64_t* GTA::FallTaskConstructor(std::uint64_t* _this, std::uint32_t flags)
	{
		if (g_SelfFeatures->m_GracefulLanding) {
			GTA::Bits::SetBits<std::uint32_t>(&flags, 10, 11, 12, 16);
		}
		return Hooking::GetHook<DetourHook>("FTC")->Original<decltype(&FallTaskConstructor)>()(_this, flags);
	}
}