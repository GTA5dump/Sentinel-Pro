#include "PlayerStruct.hpp"
#include "../../GTA/GTA.hpp"
#include "../Friends/FriendsService.hpp"
#include <GTAV-Classes/network/RemoteGamerInfoMsg.hpp>

namespace Sentinel
{
	PlayerStruct::PlayerStruct(CNetGamePlayer* player)
		: m_NetGamePlayer(player)
	{
		m_IsFriend = FriendsService::IsFriend(player);
	}

	CVehicle* PlayerStruct::GetCurrentVehicle() const
	{
		if (const auto Ped = this->GetPed(); Ped != nullptr)
			if (const auto Vehicle = Ped->m_vehicle; Vehicle != nullptr)
				return Vehicle;
		return nullptr;
	}

	const char* PlayerStruct::GetName() const
	{
		return m_NetGamePlayer == nullptr ? "" : m_NetGamePlayer->get_name();
	}

	rage::rlGamerInfo* PlayerStruct::GetNetData() const
	{
		return m_NetGamePlayer == nullptr ? nullptr : m_NetGamePlayer->get_net_data();
	}

	CNetGamePlayer* PlayerStruct::GetNetPlayer() const
	{
		return m_NetGamePlayer;
	}

	CPed* PlayerStruct::GetPed() const
	{
		if (const auto PlayerInfo = this->GetPlayerInfo(); PlayerInfo != nullptr)
			if (const auto Ped = PlayerInfo->m_ped; Ped != nullptr)
				return Ped;
		return nullptr;
	}

	CPlayerInfo* PlayerStruct::GetPlayerInfo() const
	{
		if (m_NetGamePlayer != nullptr && m_NetGamePlayer->m_player_info != nullptr)
			return m_NetGamePlayer->m_player_info;
		return nullptr;
	}

	rage::snPlayer* PlayerStruct::GetSessionPlayer()
	{
		for (std::uint32_t i = 0; i < GTA::GetNetwork()->m_game_session_ptr->m_player_count; i++)
		{
			if (GTA::GetNetwork()->m_game_session_ptr->m_players[i]->m_player_data.m_host_token == GetNetData()->m_host_token)
			{
				return GTA::GetNetwork()->m_game_session_ptr->m_players[i];
			}
		}

		if (GTA::GetNetwork()->m_game_session_ptr->m_local_player.m_player_data.m_host_token == GetNetData()->m_host_token)
			return &GTA::GetNetwork()->m_game_session_ptr->m_local_player;

		return nullptr;
	}

	rage::snPeer* PlayerStruct::GetSessionPeer()
	{
		for (std::uint32_t i = 0; i < GTA::GetNetwork()->m_game_session_ptr->m_peer_count; i++)
		{
			if (GTA::GetNetwork()->m_game_session_ptr->m_peers[i]->m_peer_data.m_gamer_handle.m_rockstar_id == GetNetData()->m_gamer_handle.m_rockstar_id)
			{
				return GTA::GetNetwork()->m_game_session_ptr->m_peers[i];
			}
		}

		return nullptr;
	}

	netAddress PlayerStruct::GetIPAddress()
	{
		if (this == g_PlayerService->GetSelf().get() && GetNetData())
			return GetNetData()->m_external_ip;

		if (auto session_player = GetSessionPlayer())
			if (auto peer = Pointers::pGetConnectionPeer(GTA::GetNetwork()->m_game_session_ptr->m_net_connection_mgr, (int)GetSessionPlayer()->m_player_data.m_peer_id_2))
				return netAddress{ ((netConnectionPeer*)peer)->m_external_ip };

		return { 0 };
	}

	std::uint16_t PlayerStruct::GetPort()
	{
		if (this == g_PlayerService->GetSelf().get() && GetNetData())
			return GetNetData()->m_external_port;

		if (auto session_player = GetSessionPlayer())
			if (auto peer = Pointers::pGetConnectionPeer(GTA::GetNetwork()->m_game_session_ptr->m_net_connection_mgr, (int)GetSessionPlayer()->m_player_data.m_peer_id_2))
				return ((netConnectionPeer*)peer)->m_external_port;

		return 0;
	}

	std::uint8_t PlayerStruct::ID() const
	{
		return m_NetGamePlayer == nullptr ? -1 : m_NetGamePlayer->m_player_id;
	}

	bool PlayerStruct::IsHost() const
	{
		return m_NetGamePlayer == nullptr ? false : m_NetGamePlayer->is_host();
	}

	bool PlayerStruct::IsFriend() const
	{
		return m_IsFriend;
	}

	bool PlayerStruct::IsValid() const
	{
		return m_NetGamePlayer == nullptr ? false : m_NetGamePlayer->is_valid();
	}

	bool PlayerStruct::Equals(const CNetGamePlayer* player) const
	{
		return player == m_NetGamePlayer;
	}

	std::string PlayerStruct::ToLowerCaseIdentifier() const
	{
		std::string lower = this->GetName();
		std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

		return lower;
	}
}