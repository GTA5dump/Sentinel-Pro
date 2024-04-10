#pragma once 
#include "../Common.hpp"

namespace Sentinel
{
	class NetworkFeatures
	{
	public:
		void Tick()
		{
		}
		bool m_EnableMovementSpoofing = false;
	};

	inline std::unique_ptr<NetworkFeatures> g_NetworkFeatures;
}