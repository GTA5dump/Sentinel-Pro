#include "FriendsService.hpp"
#include "../../Pointers/Pointers.hpp"
#include <GTAV-Classes/socialclub/FriendRegistry.hpp>

namespace Sentinel
{
	FriendsService::FriendsService()
	{
		g_FriendsService = this;
	}

	FriendsService::~FriendsService()
	{
		g_FriendsService = nullptr;
	}

	bool FriendsService::IsFriend(CNetGamePlayer* player)
	{
		if (player == nullptr)
			return false;

		const auto RID = player->get_net_data()->m_gamer_handle.m_rockstar_id;
		for (std::uint32_t i = 0; i < Pointers::pFriendRegistry->m_friend_count; i++)
			if (RID == Pointers::pFriendRegistry->get(i)->m_rockstar_id)
				return true;
		return false;
	}
}