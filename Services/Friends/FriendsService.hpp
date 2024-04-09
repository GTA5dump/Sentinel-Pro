#pragma once
#include "../../Common.hpp"
#include "../../GTA/Include.hpp"

namespace Sentinel
{
	class FriendsService final
	{
	public:
		FriendsService();
		~FriendsService();

		FriendsService(const FriendsService&) = delete;
		FriendsService(FriendsService&&) noexcept = delete;
		FriendsService& operator=(const FriendsService&) = delete;
		FriendsService& operator=(FriendsService&&) noexcept = delete;

		[[nodiscard]] static bool IsFriend(CNetGamePlayer* player);
	};

	inline FriendsService* g_FriendsService{};
}