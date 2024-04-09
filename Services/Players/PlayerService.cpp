#include "PlayerService.hpp"
#include "../../Common.hpp"
#include "../../GTA/GTA.hpp"
#include "../../GTA/Invoker/Natives.hpp"

namespace Sentinel
{
	PlayerService::PlayerService()
		: m_Self(nullptr), m_SelectedPlayer(m_Dummy)
	{
		g_PlayerService = this;

		const auto NPM = GTA::GetNetworkPlayerMgr();
		if (!NPM)
			return;

		m_Self = &NPM->m_local_net_player;

		for (std::uint16_t i = 0; i < NPM->m_player_limit; ++i)
			PlayerJoin(NPM->m_player_list[i]);
	}

	PlayerService::~PlayerService()
	{
		g_PlayerService = nullptr;
	}

	void PlayerService::DoCleanup()
	{
		m_SelectedPlayer = m_Dummy;
		m_Players.clear();
	}

	PlayerStructPointer PlayerService::GetByName(std::string name)
	{
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (const auto it = m_Players.find(name); it != m_Players.end())
			return it->second;
		return nullptr;
	}

	PlayerStructPointer PlayerService::GetByMsgID(std::uint32_t msg_id) const
	{
		for (const auto& [_, player] : m_Players)
			if (player->GetNetPlayer()->m_msg_id == msg_id)
				return player;
		return nullptr;
	}

	PlayerStructPointer PlayerService::GetByID(std::uint32_t id) const
	{
		for (const auto& [name, player] : m_Players)
			if (player->ID() == id)
				return player;
		return nullptr;
	}

	PlayerStructPointer PlayerService::GetByPed(std::int32_t ped) const
	{
		for (const auto& [name, player] : m_Players)
			if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->ID()) == ped)
				return player;
		return nullptr;
	}

	PlayerStructPointer PlayerService::GetByHostToken(std::uint64_t token) const
	{
		for (const auto& [name, player] : m_Players)
			if (player->GetNetData()->m_host_token == token)
				return player;
		return nullptr;
	}

	PlayerStructPointer PlayerService::GetSelected() const
	{
		return m_SelectedPlayer;
	}

	PlayerStructPointer PlayerService::GetSelf()
	{
		if (!m_SelfPointer || !m_SelfPointer->Equals(*m_Self))
			m_SelfPointer = std::make_shared<PlayerStruct>(*m_Self);

		return m_SelfPointer;
	}

	void PlayerService::PlayerJoin(CNetGamePlayer* player)
	{
		if (player == nullptr || player == *m_Self)
			return;

		auto Player = std::make_shared<PlayerStruct>(player);
		m_Players.emplace(
			Player->ToLowerCaseIdentifier(),
			std::move(Player)
		);
	}

	void PlayerService::PlayerLeave(CNetGamePlayer* net_game_player)
	{
		if (net_game_player == nullptr)
			return;
		if (m_SelectedPlayer && m_SelectedPlayer->Equals(net_game_player))
			m_SelectedPlayer = m_Dummy;

		auto Player = std::make_unique<PlayerStruct>(net_game_player);
		m_Players.erase(Player->ToLowerCaseIdentifier());
	}

	void PlayerService::SetSelected(PlayerStructPointer player)
	{
		m_SelectedPlayer = player;
	}
}