#pragma once 
#include "../Common.hpp"
#include "../Pointers/Pointers.hpp"
#include "Include.hpp"
#include "GTA.hpp"
#include "enums.hpp"
#include "Invoker/Natives.hpp"
#include "Script/ScriptQueue.hpp"
#include "Script/ScriptGlobal.hpp"
#include "../Services/Players/PlayerService.hpp"
#include "../Services/Players/PlayerStruct.hpp"
#include <GTAV-Classes/script/globals/GPBD_FM_3.hpp>

namespace Sentinel::Kicks
{
	inline void BreakupKick(PlayerStructPointer target, bool cheater = false)
	{
		g_ScriptQueue->Queue([=]
		{
			rage::snMsgRemoveGamersFromSessionCmd cmd{};
			cmd.m_session_id = GTA::GetNetwork()->m_game_session_ptr->m_rline_session.m_session_id;
			cmd.m_num_peers = 1;
			cmd.m_peer_ids[0] = target->GetSessionPeer()->m_peer_data.m_peer_id_2;

			if (cheater)
				cmd.m_unk = 19;

			if (GTA::GetNetwork()->m_game_session.is_host())
				Pointers::pHandleRemoveGamerCommand(GTA::GetNetwork()->m_game_session_ptr, target->GetSessionPlayer(), &cmd);
			else if (target->IsHost())
			{
				for (auto& [_, player] : g_PlayerService->Players())
				{
					if (player->ID() != target->ID())
						Pointers::pSendRemoveGamerCommand(GTA::GetNetwork()->m_game_session_ptr->m_net_connection_mgr,
							Pointers::pGetConnectionPeer(GTA::GetNetwork()->m_game_session_ptr->m_net_connection_mgr, (int)player->GetSessionPlayer()->m_player_data.m_peer_id_2),
							GTA::GetNetwork()->m_game_session_ptr->m_connection_identifier, &cmd, 0x1000000);
				}

				Pointers::pHandleRemoveGamerCommand(GTA::GetNetwork()->m_game_session_ptr, target->GetSessionPlayer(), &cmd);
			}
			else
			{
				for (auto& [_, player] : g_PlayerService->Players())
				{
					if (player->IsHost())
					{
						Pointers::pSendRemoveGamerCommand(GTA::GetNetwork()->m_game_session_ptr->m_net_connection_mgr,
							Pointers::pGetConnectionPeer(GTA::GetNetwork()->m_game_session_ptr->m_net_connection_mgr, (int)player->GetSessionPlayer()->m_player_data.m_peer_id_2),
							GTA::GetNetwork()->m_game_session_ptr->m_connection_identifier, &cmd, 0x1000000);

						break;
					}
				}
			}
		});
	}

	inline void BailKick(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			const std::size_t ArgCount = 3;
			std::int64_t Args[ArgCount] =
			{
				(std::int64_t)eRemoteEvent::NetworkBail,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
				ScriptGlobal(1894573).As<GPBD_FM_3*>()->Entries[target->ID()].ScriptEventReplayProtectionCounter
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << target->ID());
		});
	}

	inline void HostKick(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			if (g_PlayerService->GetSelf()->IsHost()) {
				NETWORK::NETWORK_SESSION_KICK_PLAYER(target->ID());
			}
		});
	}

	inline void ScriptHostKick(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			if (!g_PlayerService->GetSelf()->m_ScriptHost)
				GTA::ForceHostOfScript(g_PlayerService->GetSelf(), "freemode"_J);
			*ScriptGlobal(1885209).At(target->ID(), 1).As<bool*>() = true;
		});
	}

	inline void PickupKick(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			// Fucks up your Pickup Handler
			// Weird kick, but YimMenu added the patch for it
			// Protection: https://bit.ly/41sWX7r
			const auto Model = Joaat("vw_prop_vw_colle_alien");
			STREAMING::REQUEST_MODEL(Model);
			while (!STREAMING::HAS_MODEL_LOADED(Model))
			{
				STREAMING::REQUEST_MODEL(Model);
				Script::Current()->Yield(5ms);
			}

			const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID()), TRUE);
			OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, Coords.x, Coords.y, Coords.z + 0.5f, 0, 2500, Model, FALSE, TRUE);
		});
	}
}