#pragma once 
#include "../HookTable.hpp"
#include "../../GTA/Include.hpp"
#include "../../GTA/GTA.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/enums.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../GTA/datBitBuffer.hpp"
#include "../../GUI/Overlays/Overlays.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../Services/Players/PlayerService.hpp"
#include <GTAV-Classes\script\globals\GlobalPlayerBD.hpp>
#include <GTAV-Classes\script\globals\GPBD_FM.hpp>
#include <GTAV-Classes\script\globals\GPBD_FM_3.hpp>

namespace Sentinel
{
	bool IsPlayerDriver(Player sender)
	{
		const auto Player = g_PlayerService->GetByID(sender);

		if (!Player || !Player->GetCurrentVehicle() || !g_PlayerService->GetSelf()->GetCurrentVehicle())
			return false;

		return g_PlayerService->GetSelf()->GetCurrentVehicle()->m_driver == Player->GetPed();
	}
	
	bool IsPlayerGoon(Player sender)
	{
		auto& BossGoon = ScriptGlobal(1894573).As<GPBD_FM_3*>()->Entries[g_PlayerService->GetSelf()->ID()].BossGoon;

		if (BossGoon.Boss != g_PlayerService->GetSelf()->ID())
			return false;

		for (int i = 0; i < BossGoon.Goons.Size; i++)
		{
			if (BossGoon.Goons[i] == sender)
			{
				return true;
			}
		}

		return false;
	}

	bool ScriptedGameEventHandler(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		const auto Args = scripted_game_event->m_args;
		const auto Hash = static_cast<eRemoteEvent>(Args[0]);
		const auto Player = g_PlayerService->GetByID(player->m_player_id);
		const auto PlayerID = g_PlayerService->GetSelf()->ID();

		switch (Hash)
		{
		case eRemoteEvent::Bounty:
			if (g_ProtectionScript->m_Bounty && Args[2] == PlayerID)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Bounty Event");
				return true;
			}
			break;
		case eRemoteEvent::CeoKick:
			if (g_ProtectionScript->m_CEOKick && player->m_player_id != ScriptGlobal(1894573).As<GPBD_FM_3*>()->Entries[PlayerID].BossGoon.Boss)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "CEO Kick Event");
				return true;
			}
			break;
		case eRemoteEvent::CeoMoney:
			if (g_ProtectionScript->m_CEOMoney
				&& player->m_player_id != ScriptGlobal(1894573).As<GPBD_FM_3*>()->Entries[PlayerID].BossGoon.Boss)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "CEO Money Event");
				return true;
			}
			break;
		case eRemoteEvent::TriggerCEORaid:
		{
			if (const auto Script = GTA::FindScriptThread(Joaat("freemode")))
			{
				if (Script->m_net_component)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "CEO Raid");
				}
			}

			return true;
			break;
		}
		case eRemoteEvent::ClearWantedLevel:
			if (g_ProtectionScript->m_ClearWantedLevel && !IsPlayerDriver(player->m_player_id))
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Clear Wanted Level Event");
				return true;
			}
			break;
		case eRemoteEvent::Notification:
			switch (static_cast<eRemoteEvent>(Args[2]))
			{
			case eRemoteEvent::NotificationMoneyBanked:
			case eRemoteEvent::NotificationMoneyRemoved:
			case eRemoteEvent::NotificationMoneyStolen:
				if (g_ProtectionScript->m_MoneyNotifications)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Malicious Notification Event");
					return true;
				}
				break;
			}
			break;
		case eRemoteEvent::ForceMission:
			if (g_ProtectionScript->m_ForceMission)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Force To Mission Event");
				return true;
			}
			break;
		case eRemoteEvent::GiveCollectible:
			if (g_ProtectionScript->m_GiveCollectible)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Give Collectible Event");
				return true;
			}
			break;
		case eRemoteEvent::GtaBanner:
			if (g_ProtectionScript->m_GTABanner)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "GTA Banner Event");
				return true;
			}
			break;
		case eRemoteEvent::MCTeleport:
			if (g_ProtectionScript->m_RemoteTeleport && Args[3] <= 32)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Remote Teleport Event");
				return true;
			}
			else if (Args[3] > 32 && g_ProtectionScript->m_TSECrash)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Script Event Crash");
				return true;
			}
			break;
		case eRemoteEvent::Crash: 
		{
			if (g_ProtectionScript->m_TSECrash)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Script Event Crash");
				return true;
			}
			break;
		}
		case eRemoteEvent::Crash2:
		{
			if (g_ProtectionScript->m_TSECrash)
			{
				if (Args[2] > 32)// Actual crash condition is if args[2] is above 255
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Script Event Crash");
					return true;
				}
			}
			break;
		}
		case eRemoteEvent::Crash3:
		{
			if (g_ProtectionScript->m_TSECrash)
			{
				if (isnan(*(float*)&Args[3]) || isnan(*(float*)&Args[4]))
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Script Event Crash");
					return true;
				}
			}
			break;
		}
		case eRemoteEvent::PersonalVehicleDestroyed:
			if (g_ProtectionScript->m_VehicleMessage)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Vehicle Message Event");
				return true;
			}
			break;
		case eRemoteEvent::RemoteOffradar:
			if (g_ProtectionScript->m_RemoteOffRadar
				&& player->m_player_id != ScriptGlobal(1894573).As<GPBD_FM_3*>()->Entries[PlayerID].BossGoon.Boss)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Off-Radar Event");
				return true;
			}
			break;
		case eRemoteEvent::TSECommand:
			if (g_ProtectionScript->m_RotateCam && static_cast<eRemoteEvent>(Args[2]) == eRemoteEvent::TSECommandRotateCam
				&& !GTA::GetNetwork()->m_is_activity_session)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Rotate Cam Event");
				return true;
			}
			break;
		case eRemoteEvent::SendToCayoPerico:
			if (g_ProtectionScript->m_ForceToCayo && Args[3] == 0)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Force To Cayo Perico Event");
				return true;
			}
			break;
		case eRemoteEvent::SendToLocation:
		{
			bool KnownLocation = false;

			if (Args[2] == 0 && Args[3] == 0)
			{
				if (Args[4] == 4 && Args[5] == 0)
				{
					KnownLocation = true;

					if (g_ProtectionScript->m_RemoteTeleport)
					{
						if (g_ProtectionScript->m_ScriptEventsNotify)
							Overlays::ProtectionMessage(Player->GetName(), "Remote Teleport Event");
						return true;
					}
				}
				else if ((Args[4] == 3 || Args[4] == 4) && Args[5] == 1)
				{
					KnownLocation = true;

					if (g_ProtectionScript->m_RemoteTeleport)
					{
						if (g_ProtectionScript->m_ScriptEventsNotify)
							Overlays::ProtectionMessage(Player->GetName(), "Remote Teleport Event");
						return true;
					}
				}
			}

			if (!KnownLocation)
			{
				if (g_ProtectionScript->m_Freeze)
				{
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Freeze Event");
					return true;
				}
			}
			break;
		}
		case eRemoteEvent::SoundSpam:
		{
			if (g_ProtectionScript->m_SoundSpam)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Sound Event");
				return true;
			}
			break;
		}
		case eRemoteEvent::Spectate:
			if (g_ProtectionScript->m_SpectateMessage)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Spectate Message Event");
				return true;
			}
			break;
		case eRemoteEvent::Teleport:
			if (g_ProtectionScript->m_RemoteTeleport && !IsPlayerDriver(player->m_player_id))
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Remote Teleport Event");
				return true;
			}
			break;
		case eRemoteEvent::TransactionError:
			if (g_ProtectionScript->m_TransactionError)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Transaction Error Event");
				return true;
			}
			break;
		case eRemoteEvent::VehicleKick:
			if (g_ProtectionScript->m_VehicleKick)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Vehicle Kick Event");
				return true;
			}
			break;
		case eRemoteEvent::NetworkBail:
			if (g_ProtectionScript->m_BailKick)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Network Bail Event");
				return true;
			}
			break;
		case eRemoteEvent::TeleportToWarehouse:
			if (g_ProtectionScript->m_RemoteTeleport && !IsPlayerDriver(player->m_player_id))
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Remote Teleport Event");
				return true;
			}
			break;
		case eRemoteEvent::StartActivity:
		{
			eActivityType activity = static_cast<eActivityType>(Args[2]);
			if (g_ProtectionScript->m_ForceActivity)
			{
				if (activity == eActivityType::Survival || activity == eActivityType::Mission || activity == eActivityType::Deathmatch || activity == eActivityType::BaseJump || activity == eActivityType::Race)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Force Activity Event");
					return true;
				}
				else if (activity == eActivityType::Darts)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Force Activity Event");
					return true;
				}
				else if (activity == eActivityType::PilotSchool)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Force Activity Event");
					return true;
				}
				else if (activity == eActivityType::ImpromptuDeathmatch)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Force Activity Event");
					return true;
				}
				else if (activity == eActivityType::DefendSpecialCargo || activity == eActivityType::GunrunningDefend || activity == eActivityType::BikerDefend || Args[2] == 238)
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Force Activity Event");
					return true;
				}
			}
			else if (activity == eActivityType::Tennis)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Script Event Crash");
				return true;
			}

			if (g_ProtectionScript->m_ForceActivity && !IsPlayerGoon(player->m_player_id))
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Force Activity Event");
				return true;
			}

			break;
		}
		case eRemoteEvent::InteriorControl:
		{
			if (g_ProtectionScript->m_NullKick)
			{
				const int Iterior = (int)Args[2];
				if (Iterior < 0 || Iterior > 161) // The upper bound will change after an update
				{
					if (g_ProtectionScript->m_ScriptEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Null Function Event Kick");
					return true;
				}
				break;
			}
		}
		case eRemoteEvent::SMS:
		{
			if (g_ProtectionScript->m_SMS)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "SMS Event");
				return true;
			}
			break;
		}
		case eRemoteEvent::DestroyPersonalVehicle:
		{
			if (g_ProtectionScript->m_VehicleMessage)
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Vehicle Message Event");
				return true;
			}
			break;
		}
		case eRemoteEvent::KickFromInterior:
		{
			if (g_ProtectionScript->m_InteriorKick && ScriptGlobal(2657589).As<GlobalPlayerBD*>()->Entries[PlayerID].SimpleInteriorData.Owner != Player->ID())
			{
				if (g_ProtectionScript->m_ScriptEventsNotify)
					Overlays::ProtectionMessage(Player->GetName(), "Interior Kick Event");
				return true;
			}
			break;
		}
		}
		return false;
	}

	bool IncrementStatEventHandler(CNetworkIncrementStatEvent* net_event, CNetGamePlayer* sender)
	{
		const auto Name = sender->get_name();
		switch (net_event->m_stat)
		{
		case rage::joaat("MPPLY_BAD_CREW_STATUS"):
		{
			if (g_ProtectionScript->m_BadCrewStatus) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Bad Crew Status");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_BAD_CREW_MOTTO"):
		{
			if (g_ProtectionScript->m_BadCrewMotto) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Bad Crew Motto");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_BAD_CREW_NAME"):
		{
			if (g_ProtectionScript->m_BadCrewName) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Bad Crew Name");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_BAD_CREW_EMBLEM"):
		{
			if (g_ProtectionScript->m_BadCrewEmblem) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Bad Crew Emblem");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_EXPLOITS"):
		{
			if (g_ProtectionScript->m_Exploits) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Exploits");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_GAME_EXPLOITS"):
		{
			if (g_ProtectionScript->m_GameExploits) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Game Exploits");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_TC_ANNOYINGME"):
		{
			if (g_ProtectionScript->m_ChatAnnoying) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Chat Annoying");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_TC_HATE"):
		{
			if (g_ProtectionScript->m_ChatHate) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Chat Hate");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_VC_ANNOYINGME"):
		{
			if (g_ProtectionScript->m_VoiceAnnoying) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Voice Annoying");
				return true;
			}
			break;
		}
		case rage::joaat("MPPLY_VC_HATE"):
		{
			if (g_ProtectionScript->m_VoiceHate) {
				if (g_ProtectionScript->m_IncrementEventsNotify)
					Overlays::ReportMessage(Name, "Voice Hate");
				return true;
			}
			break;
		}
		}
		return false;
	}

	void GTA::ReceivedEventHandler(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, uint16_t event_id, int event_index, int event_handled_bitset, int unk, rage::datBitBuffer* bit_buffer)
	{
		if (event_id > 91u)
		{
			Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
			return;
		}

		const auto EventName = *(char**)((DWORD64)event_manager + 8i64 * event_id + 243376);
		if (EventName == nullptr || source_player == nullptr || source_player->m_player_id < 0 || source_player->m_player_id >= 32)
		{
			Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
			return;
		}

		const auto Player = g_PlayerService->GetByID(source_player->m_player_id);
		const auto LocalPlayer = g_PlayerService->GetSelf()->GetPed();

		switch (static_cast<eNetworkEvents>(event_id))
		{
		case eNetworkEvents::SCRIPTED_GAME_EVENT:
		{
			const auto ScriptedGameEvent = std::make_unique<CScriptedGameEvent>();
			bit_buffer->ReadDword(&ScriptedGameEvent->m_args_size, 32);
			if (ScriptedGameEvent->m_args_size - 1 <= 0x1AF)
				bit_buffer->ReadArray(&ScriptedGameEvent->m_args, 8 * ScriptedGameEvent->m_args_size);

			if (ScriptedGameEventHandler(ScriptedGameEvent.get(), source_player))
			{
				Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);

				return;
			}
			bit_buffer->Seek(0);
			break;
		}
		case eNetworkEvents::NETWORK_INCREMENT_STAT_EVENT:
		{
			const auto IncrementStatEvent = std::make_unique<CNetworkIncrementStatEvent>();
			bit_buffer->ReadDword(&IncrementStatEvent->m_stat, 0x20);
			bit_buffer->ReadDword(&IncrementStatEvent->m_amount, 0x20);
			if (IncrementStatEventHandler(IncrementStatEvent.get(), source_player))
			{
				Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
				return;
			}
			bit_buffer->Seek(0);
			break;
		}
		case eNetworkEvents::KICK_VOTES_EVENT:
		{
			if (g_ProtectionScript->m_VoteKick)
			{
				const std::uint32_t BitField = bit_buffer->Read<std::uint32_t>(32);
				if (BitField & (1 << target_player->m_player_id))
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::PushWarning("Protection Script", std::format("Player {} is voting to kick you.", Player->GetName()), 5000);
				}
				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::NETWORK_CLEAR_PED_TASKS_EVENT:
		{
			if (g_ProtectionScript->m_Freeze)
			{
				const int NetworkID = bit_buffer->Read<int>(13);

				if (LocalPlayer && LocalPlayer->m_net_object && LocalPlayer->m_net_object->m_object_id == NetworkID)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Freeze Event");
					return;
				}

				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::RAGDOLL_REQUEST_EVENT:
		{
			if (g_ProtectionScript->m_Ragdoll)
			{
				const int NetworkID = bit_buffer->Read<int>(13);

				if (LocalPlayer && LocalPlayer->m_net_object && LocalPlayer->m_net_object->m_object_id == NetworkID)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Ragdoll Event");
					return;
				}

				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::SCRIPT_ENTITY_STATE_CHANGE_EVENT:
		{
			if (g_ProtectionScript->m_StateChange)
			{
				const std::uint16_t Entity = bit_buffer->Read<std::uint16_t>(13); // Unused
				const auto Type = bit_buffer->Read<ScriptEntityChangeType>(4);
				const std::uint32_t Unknown = bit_buffer->Read<std::uint32_t>(32);
				if (Type == ScriptEntityChangeType::SettingOfTaskVehicleTempAction)
				{
					const std::uint16_t ID = bit_buffer->Read<std::uint16_t>(13); // Unused
					const std::uint32_t Action = bit_buffer->Read<std::uint32_t>(8);

					if ((Action >= 15 && Action <= 18) || Action == 33)
					{
						Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
						if (g_ProtectionScript->m_NetworkEventsNotify)
							Overlays::ProtectionMessage(Player->GetName(), "State Crash");
						return;
					}
				}
				else if (Type > ScriptEntityChangeType::SetVehicleExclusiveDriver || Type < ScriptEntityChangeType::BlockingOfNonTemporaryEvents)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "State Crash");
					return;
				}
				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::REQUEST_CONTROL_EVENT:
		{
			if (g_ProtectionScript->m_RequestControl)
			{
				const int NetworkID = bit_buffer->Read<int>(13);
				if (LocalPlayer && LocalPlayer->m_vehicle && LocalPlayer->m_vehicle->m_net_object
					&& LocalPlayer->m_vehicle->m_net_object->m_object_id == NetworkID && LocalPlayer->m_vehicle->m_driver == LocalPlayer)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Request Control Event");
					return;
				}
				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::GIVE_CONTROL_EVENT:
		{
			if (g_ProtectionScript->m_GiveControl)
			{
				const std::uint32_t Time = bit_buffer->Read<std::uint32_t>(32);
				int Count = bit_buffer->Read<int>(2);
				const bool Migrate = bit_buffer->Read<bool>(1);

				if (Count > 3)
					Count = 3;

				for (int i = 0; i < Count; i++)
				{
					const int NetworkID = bit_buffer->Read<int>(13);
					const eNetObjType ObjectType = bit_buffer->Read<eNetObjType>(4);
					const int MigrationType = bit_buffer->Read<int>(3);

					if (ObjectType < eNetObjType::NET_OBJ_TYPE_AUTOMOBILE || ObjectType > eNetObjType::NET_OBJ_TYPE_TRAIN)
					{
						Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
						if (g_ProtectionScript->m_NetworkEventsNotify)
							Overlays::ProtectionMessage(Player->GetName(), "Give Control Event");
						return;
					}
				}

				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::REMOVE_WEAPON_EVENT:
		{
			if (g_ProtectionScript->m_RemoveWeapon)
			{
				const int NetworkID = bit_buffer->Read<int>(13);
				std::uint32_t Hash = bit_buffer->Read<std::uint32_t>(32);

				if (Hash == Joaat("WEAPON_UNARMED"))
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Remove Weapon Event");
					return;
				}

				if (LocalPlayer && LocalPlayer->m_net_object && LocalPlayer->m_net_object->m_object_id == NetworkID)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Remove Weapon Event");
					return;
				}

				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::REMOVE_ALL_WEAPONS_EVENT:
		{
			if (g_ProtectionScript->m_RemoveAllWeapons)
			{
				const int NetworkID = bit_buffer->Read<int>(13);

				if (LocalPlayer && LocalPlayer->m_net_object && LocalPlayer->m_net_object->m_object_id == NetworkID)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "Remove All Weapons Event");
					return;
				}

				bit_buffer->Seek(0);
			}
			break;
		}
		case eNetworkEvents::GIVE_WEAPON_EVENT: 
		{
			if (g_ProtectionScript->m_GiveWeapons)
			{
				std::uint16_t NetworkID{};
				bit_buffer->ReadWord(&NetworkID, 0xD);
				if (const auto NetworkObject = Pointers::pGetNetObject(*Pointers::pNetworkObjectMgr, NetworkID, FALSE); NetworkObject)
				{
					if (NetworkObject->m_owner_id == g_PlayerService->GetSelf()->ID())
					{
						Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
						if (g_ProtectionScript->m_NetworkEventsNotify)
							Overlays::ProtectionMessage(Player->GetName(), "Give Weapon Event");
						return;
					}
				}
				bit_buffer->Seek(0);
			}
		} 
		break;
		case eNetworkEvents::EXPLOSION_EVENT: 
		{
			if (g_ProtectionScript->m_Explosions)
			{
				std::uint16_t ExplosionOwner{};
				std::int32_t ExplosionType{};
				std::uint32_t ExplosionFlags{};
				bit_buffer->Seek(0x1D);
				bit_buffer->ReadWord(&ExplosionOwner, 0xD);
				bit_buffer->Seek(0x37);
				bit_buffer->ReadInt32(&ExplosionType, 0x8);
				bit_buffer->Seek(0x95);
				bit_buffer->ReadDword(&ExplosionFlags, 0x20);
				if (ExplosionOwner != NULL) 
				{
					if (const auto NetworkObject = Pointers::pGetNetObject(*Pointers::pNetworkObjectMgr, ExplosionOwner, FALSE); NetworkObject) 
					{
						if (const auto Owner = NetworkObject->m_owner_id; Owner != NULL) 
						{
							Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
							if (g_ProtectionScript->m_NetworkEventsNotify)
								Overlays::ProtectionMessage(Player->GetName(), (Owner != source_player->m_player_id && (ExplosionFlags & 11)) ? "Blamed Explosion" : "Explosion");
							return;
						}
					}
				}
				bit_buffer->Seek(0);
			}
		} 
		break;
		case eNetworkEvents::NETWORK_PTFX_EVENT:
		{
			if (g_ProtectionScript->m_PTFX)
			{
				const int NetworkID = bit_buffer->Read<int>(13);

				if (LocalPlayer && LocalPlayer->m_net_object && LocalPlayer->m_net_object->m_object_id == NetworkID)
				{
					Pointers::pSendEventAck(event_manager, source_player, target_player, event_index, event_handled_bitset);
					if (g_ProtectionScript->m_NetworkEventsNotify)
						Overlays::ProtectionMessage(Player->GetName(), "PTFX Event");
					return;
				}
				bit_buffer->Seek(0);
			}
		}
		break;
		default: break;
		}

		return Hooking::GetHook<DetourHook>("REH")->Original<decltype(&ReceivedEventHandler)>()(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, unk, bit_buffer);
	}
}