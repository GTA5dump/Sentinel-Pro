#pragma once
#include "../../Common.hpp"
#include "../../GTA/Include.hpp"
#include "PlayerStruct.hpp"

namespace Sentinel
{
	class PlayerStruct;

	using PlayerStructPointer = std::shared_ptr<PlayerStruct>;
	using PlayerStructEntry = std::pair<std::string, PlayerStructPointer>;
	using Players = std::map<std::string, PlayerStructPointer>;

	class PlayerService final
	{
	public:
		CNetGamePlayer** m_Self;

		PlayerStructPointer m_SelfPointer;

		Players m_Players;

		PlayerStructPointer m_Dummy = std::make_shared<PlayerStruct>(nullptr);
		PlayerStructPointer m_SelectedPlayer;
	public:

		PlayerService();
		~PlayerService();

		PlayerService(const PlayerService&) = delete;
		PlayerService(PlayerService&&) noexcept = delete;
		PlayerService& operator=(const PlayerService&) = delete;
		PlayerService& operator=(PlayerService&&) noexcept = delete;

		void DoCleanup();

		[[nodiscard]] PlayerStructPointer GetSelf();

		[[nodiscard]] PlayerStructPointer GetByName(std::string name);
		[[nodiscard]] PlayerStructPointer GetByMsgID(std::uint32_t msg_id) const;
		[[nodiscard]] PlayerStructPointer GetByID(std::uint32_t id) const;
		[[nodiscard]] PlayerStructPointer GetByPed(std::int32_t ped) const;
		[[nodiscard]] PlayerStructPointer GetByHostToken(std::uint64_t token) const;
		[[nodiscard]] PlayerStructPointer GetSelected() const;

		void PlayerJoin(CNetGamePlayer* player);
		void PlayerLeave(CNetGamePlayer* player);

		Players& Players()
		{
			return m_Players;
		}

		void Iterate(const std::function< void(const PlayerStructEntry& entry) > func)
		{
			for (const auto& iter : m_Players)
				func(iter);
		}

		void SetSelected(PlayerStructPointer player);

	};

	inline PlayerService* g_PlayerService{};
}