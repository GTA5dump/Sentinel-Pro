#pragma once 
#include "../Common.hpp"
#include "System.hpp"
#include "Lists.hpp"
#include "../GTA/enums.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/Script/ScriptFunction.hpp"
#include "../Pointers/Pointers.hpp"
#include "../Hooks/HookTable.hpp"
#include "../Services/Players/PlayerService.hpp"
#include <GTAV-Classes\script\globals\GlobalPlayerBD.hpp>
#include <GTAV-Classes\script\globals\GPBD_FM.hpp>
#include <GTAV-Classes\script\globals\GPBD_FM_3.hpp>

namespace Sentinel::Target
{
	inline bool IsPlayerVehicle(Vehicle target)
	{
		if (ENTITY::IS_ENTITY_A_VEHICLE(target))
		{
			for (int i = -1; i < 16; i++)
			{
				if (!VEHICLE::IS_VEHICLE_SEAT_FREE(i, i, TRUE))
				{
					const auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(target, i, FALSE);
					if (PED::IS_PED_A_PLAYER(Ped))
						return true;
				}
			}

			return false;
		}
		else
			return false;
	}

	inline void SendRemoteEvent(PlayerStructPointer player, int index)
	{
		const auto ID = player->ID();

		if (index == 0) // CEO Kick
		{
			const auto Leader = *ScriptGlobal(1894573).At(ID, 608).Add(10).As<int*>();
			if (Leader == -1)
				return;
			else if (Leader == ID)
			{
				const std::size_t ArgCount = 2;
				std::int64_t Args[ArgCount] = {
					(std::int64_t)eRemoteEvent::CeoKick,
					(std::int64_t)PLAYER::GET_PLAYER_INDEX()
				};

				Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
			}
			else
			{
				// Use a more private method to remove associate
				const std::size_t ArgCount = 3;
				std::int64_t Args[ArgCount] = {
					(std::int64_t)eRemoteEvent::MarkPlayerAsBeast,
					(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
					Leader
				};

				Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
			}
		}
		else if (index == 1) // CEO Join
		{
			ScriptFunctions::JoinCEO({ ID, 0, false, false });
		}
		else if (index == 2) // Enter Interior
		{
			if (ScriptGlobal(1853910).As<GPBD_FM*>()->Entries[ID].PropertyData.Index != -1)
			{
				const std::size_t ArgCount = 9;
				std::int64_t Args[ArgCount] = {
					(std::int64_t)eRemoteEvent::Teleport,
					PLAYER::GET_PLAYER_INDEX(),
					(std::int64_t)ID,
					(std::int64_t)(int)-1,
					1,
					(std::int64_t)ScriptGlobal(1853910).As<GPBD_FM*>()->Entries[ID].PropertyData.Index,
					1,
					0,
					1 };

				Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << PLAYER::GET_PLAYER_INDEX());
			}
			else if (ScriptGlobal(2657589).As<GlobalPlayerBD*>()->Entries[ID].SimpleInteriorData.Index != eSimpleInteriorIndex::SIMPLE_INTERIOR_INVALID)
			{
				*ScriptGlobal(1950108).Add(3346).As<Player*>() =
					ScriptGlobal(2657589).As<GlobalPlayerBD*>()->Entries[ID].SimpleInteriorData.Owner;
				*ScriptGlobal(1950108).Add(3683).As<eSimpleInteriorIndex*>() =
					ScriptGlobal(2657589).As<GlobalPlayerBD*>()->Entries[ID].SimpleInteriorData.Index;
				*ScriptGlobal(1950108).Add(3682).As<bool*>() = true;
				ScriptGlobal(2657589).As<GlobalPlayerBD*>()->Entries[PLAYER::GET_PLAYER_INDEX()].SimpleInteriorData.InteriorSubtype =
					ScriptGlobal(2657589).As<GlobalPlayerBD*>()->Entries[ID].SimpleInteriorData.InteriorSubtype;
			}
			else
				return;
		}
		else if (index == 3) // Vehicle Kick
		{
			const std::size_t ArgCount = 9;
			std::int64_t Args[ArgCount] = {
				(int64_t)eRemoteEvent::VehicleKick,
				PLAYER::GET_PLAYER_INDEX(), 0, 0, 0, 0, 0, 0, 0
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 4) // Send To Mission
		{
			const std::size_t ArgCount = 3;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::ForceMission,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(), 0
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 5) // Send To Island
		{
			const std::size_t ArgCount = 2;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::SendToCayoPerico,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 6) // Send Sext
		{
			const std::size_t ArgCount = 8;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::SendTextLabelSMS,
				PLAYER::GET_PLAYER_INDEX()
			};

			static constexpr auto Strippers = std::to_array({ "HCH", "INF", "JUL", "NIK", "SAP", "TXI" });
			static constexpr auto SextTypes = std::to_array({ "1ST", "2ND", "NEED" });

			strcpy((char*)&Args[2],
				(std::string("SXT_") + Strippers[rand() % Strippers.size()] + "_" + SextTypes[rand() % SextTypes.size()])
				.data());
			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 7) // Send Transaction Error
		{
			const std::size_t ArgCount = 8;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::TransactionError,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
				1,
				0,
				0,
				ScriptGlobal(1894573).As<GPBD_FM_3*>()->Entries[ID].ScriptEventReplayProtectionCounter,
				*ScriptGlobal(1923597).Add(9).As<int*>(),
				*ScriptGlobal(1923597).Add(10).As<int*>()
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 8) // Send Ban Message
		{
			const std::size_t ArgCount = 8;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::SendTextLabelSMS,
				PLAYER::GET_PLAYER_INDEX()
			};

			strcpy((char*)&Args[2], "HUD_ROSBANPERM");

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 9) // Send Money Stolen Message
		{
			const std::size_t ArgCount = 4;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::Notification,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
				(std::int64_t)eRemoteEvent::NotificationMoneyStolen,
				INT_MAX
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 10) // Send Money Removed Message
		{
			const std::size_t ArgCount = 4;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::Notification,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
				(std::int64_t)eRemoteEvent::NotificationMoneyRemoved,
				INT_MAX
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 11) // Send Money Banked Message
		{
			const std::size_t ArgCount = 4;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::Notification,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
				(std::int64_t)eRemoteEvent::NotificationMoneyBanked,
				INT_MAX
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 12) // Send Spectate Message
		{
			const std::size_t ArgCount = 3;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::Notification,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
				(std::int64_t)eRemoteEvent::Spectate
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
		else if (index == 13) // Sound Spam
		{
			const size_t ArgCount = 2;
			std::int64_t Args[ArgCount] = {
				(std::int64_t)eRemoteEvent::SoundSpam,
				(std::int64_t)PLAYER::GET_PLAYER_INDEX()
			};

			Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << ID);
		}
	}

	inline void BlameExplodeCoord(PlayerStructPointer to_blame, NativeVector3 pos, eExplosionTag explosion_type, float damage, bool is_audible, bool is_invisible, float camera_shake)
	{
		System::AddOwnedExplosionBypass(true);
		FIRE::ADD_OWNED_EXPLOSION(
			PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(to_blame->ID()),
			pos.x, pos.y, pos.z,
			(int)explosion_type,
			damage,
			is_audible,
			is_invisible,
			camera_shake
		);
		System::AddOwnedExplosionBypass(false);
	}

	inline void BlameExplodePlayer(PlayerStructPointer to_blame, PlayerStructPointer target, eExplosionTag explosion_type, float damage, bool is_audible, bool is_invisible, float camera_shake)
	{
		auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID()), true);
		BlameExplodeCoord(to_blame, Coords, explosion_type, damage, is_audible, is_invisible, camera_shake);
	}

	inline void BurnPlayer(PlayerStructPointer player)
	{
		auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->ID());
		auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
		if (!PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
			FIRE::START_SCRIPT_FIRE(Coords.x, Coords.y, Coords.z, 5, TRUE);
	}

	inline void ShootPlayer(PlayerStructPointer player, Hash bullet)
	{
		if (!(player->GetPed()->m_damage_bits & (1 << 8)))
		{
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(bullet))
			{
				WEAPON::REQUEST_WEAPON_ASSET(bullet, 31, 0);
				Script::Current()->Yield(5ms);
			}

			const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player->ID());
			constexpr auto MaxAttempts = 20;
			for (std::size_t Attempts = 0; Attempts <= MaxAttempts; Attempts++)
			{
				const auto Destination = PED::GET_PED_BONE_COORDS(Ped, (int)0x0, 0.0f, 0.0f, 0.0f);
				const auto Origin = PED::GET_PED_BONE_COORDS(Ped, (int)0xDEAD, 0.0f, 0.0f, 0.2f);

				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Origin.x, Origin.y, Origin.z, Destination.x, Destination.y, Destination.z, 1, 0, bullet, PLAYER::PLAYER_PED_ID(), TRUE, FALSE, 1.f);
				WEAPON::REMOVE_WEAPON_ASSET(bullet);
			}
		}
	}

	inline void SetBounty(PlayerStructPointer target, int value, bool anonymous)
	{
		const std::size_t ArgCount = 22;
		std::int64_t Args[ArgCount] = {
			(std::int64_t)eRemoteEvent::Bounty,
			PLAYER::GET_PLAYER_INDEX(),
			target->ID(),
			1,
			value,
			0,
			anonymous ? 1 : 0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			*ScriptGlobal(1923597).Add(9).As<std::int64_t*>(),
			*ScriptGlobal(1923597).Add(10).As<std::int64_t*>() };

		Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << target->ID());
	}

	inline void SendTextMessage(PlayerStructPointer player, char* message, int color, int font)
	{
		std::string Color = Lists::g_GTAFontColors[color];
		std::string Font = Lists::g_GTAFontTypes[font];

		std::ostringstream Message{};
		Message << Font << Color << message;

		// Required
		NETWORK::NETWORK_SET_VOICE_ACTIVE(TRUE);

		// Get RID Handle
		int Handle[76];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player->ID(), &Handle[0], 13);

		// Send Message and Restore
		if (NETWORK::NETWORK_SEND_TEXT_MESSAGE(const_cast<char*>(Message.str().c_str()), &Handle[0]))
			NETWORK::NETWORK_SET_VOICE_ACTIVE(FALSE);
	}

	inline void SendPrivateChatMessage(PlayerStructPointer target, char* message, bool view_local)
	{
		PLAYER::SET_PLAYER_TEAM(PLAYER::PLAYER_ID(), 1);
		PLAYER::SET_PLAYER_TEAM(target->ID(), 1);

		if (const auto GamePlayer = GTA::GetNetworkPlayerMgr()->m_local_net_player; GamePlayer)
		{
			if (Hooking::GetHook<DetourHook>("SCM")->Original<decltype(&GTA::SendChatMessage)>()(*Pointers::pSendChatMessagePointer, GamePlayer->get_net_data(), message, true))
			{
				if (view_local)
					System::DrawChat(message, GamePlayer->get_name(), true);
			}
		}

		Script::Current()->Yield(100ms);

		PLAYER::SET_PLAYER_TEAM(PLAYER::PLAYER_ID(), -1);
		PLAYER::SET_PLAYER_TEAM(target->ID(), -1);
	}

	inline void ClearPlayerWantedLevel(Player target)
	{
		constexpr std::size_t ArgCount = 3;
		std::int64_t Args[ArgCount] = {
			static_cast<std::int64_t>(eRemoteEvent::ClearWantedLevel),
			PLAYER::GET_PLAYER_INDEX(),
			*ScriptGlobal(1894573).At(target, 608).Add(510).As<int*>()
		};

		Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << target);
	}
}