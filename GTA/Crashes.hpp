#pragma once 
#include "../Common.hpp"
#include "../Pointers/Pointers.hpp"
#include "Include.hpp"
#include "GTA.hpp"
#include "Invoker/Natives.hpp"
#include "Script/ScriptQueue.hpp"
#include "Script/ScriptGlobal.hpp"
#include "../Services/Players/PlayerService.hpp"
#include "../Services/Players/PlayerStruct.hpp"
#include <GTAV-Classes/script/globals/GPBD_FM_3.hpp>
#include "../Utility/Entities.hpp"
#include "../Features/Misc.hpp"

namespace Sentinel::Crashes
{
	inline void SyncCrash(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID());
			const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
			if (!INTERIOR::IS_VALID_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(Ped)) && ENTITY::DOES_ENTITY_EXIST(Ped))
			{
				{
					const auto Hash = Joaat("adder");
					while (!STREAMING::HAS_MODEL_LOADED(Hash))
					{
						STREAMING::REQUEST_MODEL(Hash);
						Script::Current()->Yield(5ms);
					}
					const auto ModelInfo = GTA::GetModelInfo(Hash);
					if (ModelInfo != NULL) 
					{
						*(std::int32_t*)(ModelInfo + 0x340) = 8;
						System::ModelSpawnBypass(true);
						const auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Coords.x, Coords.y, Coords.z, 0.f, TRUE, FALSE, FALSE);
						System::ModelSpawnBypass(false);
						DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
						ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
						const auto NID = NETWORK::VEH_TO_NET(Vehicle);
						if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
							NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
						VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
						if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
						{
							ENTITY::SET_ENTITY_VISIBLE(Vehicle, FALSE, NULL);
							VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, FALSE);
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 100.f);
							Script::Current()->Yield(1s);
							Entities::DeleteEntity(Vehicle);
						}
						*(std::int32_t*)(ModelInfo + 0x340) = 0;
					}

					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
				}
				{
					const auto Hash = Joaat("blazer");
					while (!STREAMING::HAS_MODEL_LOADED(Hash))
					{
						STREAMING::REQUEST_MODEL(Hash);
						Script::Current()->Yield(5ms);
					}
					const auto ModelInfo = GTA::GetModelInfo(Hash);
					if (ModelInfo != NULL)
					{
						*(std::int32_t*)(ModelInfo + 0x340) = 11;
						System::ModelSpawnBypass(true);
						const auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Coords.x, Coords.y, Coords.z, 0.f, TRUE, FALSE, FALSE);
						System::ModelSpawnBypass(false);
						DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
						ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
						const auto NID = NETWORK::VEH_TO_NET(Vehicle);
						if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
							NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
						VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
						if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
						{
							ENTITY::SET_ENTITY_VISIBLE(Vehicle, FALSE, NULL);
							VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, FALSE);
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 100.f);
							Script::Current()->Yield(1s);
							Entities::DeleteEntity(Vehicle);
						}
						*(std::int32_t*)(ModelInfo + 0x340) = 3;
					}

					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
				}
				{
					const auto Hash = Joaat("dinghy");
					while (!STREAMING::HAS_MODEL_LOADED(Hash))
					{
						STREAMING::REQUEST_MODEL(Hash);
						Script::Current()->Yield(5ms);
					}
					const auto ModelInfo = GTA::GetModelInfo(Hash);
					if (ModelInfo != NULL)
					{
						*(std::int32_t*)(ModelInfo + 0x340) = 1;
						System::ModelSpawnBypass(true);
						const auto Vehicle = VEHICLE::CREATE_VEHICLE(Hash, Coords.x, Coords.y, Coords.z, 0.f, TRUE, FALSE, FALSE);
						System::ModelSpawnBypass(false);
						DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
						ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
						const auto NID = NETWORK::VEH_TO_NET(Vehicle);
						if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
							NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
						VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
						if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
						{
							ENTITY::SET_ENTITY_VISIBLE(Vehicle, FALSE, NULL);
							VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, FALSE);
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 100.f);
							Script::Current()->Yield(1s);
							Entities::DeleteEntity(Vehicle);
						}
						*(std::int32_t*)(ModelInfo + 0x340) = 13;
					}

					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
				}
			}
		});
	}

	inline void TaskCrash(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID());
			if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
			{
				const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);
				TASK::TASK_VEHICLE_TEMP_ACTION(Ped, Vehicle, 15, 100);
				TASK::TASK_VEHICLE_TEMP_ACTION(Ped, Vehicle, 16, 100);
				TASK::TASK_VEHICLE_TEMP_ACTION(Ped, Vehicle, 17, 100);
				TASK::TASK_VEHICLE_TEMP_ACTION(Ped, Vehicle, 18, 100);
			}
		});
	}

	inline void KitCrash(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			g_MiscFeatures->m_AntiCrashCamera = true;
			const auto VehicleHash = 1349725314;
			const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID());
			const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

			while (!STREAMING::HAS_MODEL_LOADED(VehicleHash))
			{
				STREAMING::REQUEST_MODEL(VehicleHash);
				Script::Current()->Yield(5ms);
			}
			
			System::ModelSpawnBypass(true);
			const auto Vehicle = VEHICLE::CREATE_VEHICLE(VehicleHash, Coords.x, Coords.y, Coords.z, 0.f, TRUE, FALSE, FALSE);
			System::ModelSpawnBypass(false);

			Script::Current()->Yield();
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(VehicleHash);

			DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
			ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
			const auto NID = NETWORK::VEH_TO_NET(Vehicle);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
			VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);

			Entities::RequestControl(Vehicle);

			VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
			VEHICLE::SET_VEHICLE_MOD(Vehicle, 34, 3, FALSE);
			VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, 34, TRUE);
			Script::Current()->Yield(1s);
			VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
			VEHICLE::SET_VEHICLE_MOD(Vehicle, 0, 0, FALSE);
			VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, 0, TRUE);
			Script::Current()->Yield(1s);
			Entities::DeleteEntity(Vehicle);
			g_MiscFeatures->m_AntiCrashCamera = false;
		});
	}

	inline bool g_PogoCrash = false;
	inline void PogoCrash(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			g_PogoCrash = true;
			const auto Hash = Joaat("u_m_y_pogo_01");
			while (!STREAMING::HAS_MODEL_LOADED(Hash))
			{
				STREAMING::REQUEST_MODEL(Hash);
				Script::Current()->Yield(5ms);
			}

			const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID());
			const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

			System::ModelSpawnBypass(true);
			const auto CrashPed = PED::CREATE_PED(4, Hash, Coords.x, Coords.y, Coords.z, ENTITY::GET_ENTITY_HEADING(Ped), TRUE, FALSE);
			System::ModelSpawnBypass(false);

			Script::Current()->Yield(2s);

			Entities::DeleteEntity(CrashPed);
			g_PogoCrash = false;
		});
	}

	inline void ScriptCrash(PlayerStructPointer target)
	{
		g_ScriptQueue->Queue([=]
		{
			std::int64_t Args1[] = {(std::int64_t)eRemoteEvent::Crash, (std::int64_t)PLAYER::GET_PLAYER_INDEX()};
			Pointers::pTriggerScriptEvent(1, Args1, sizeof(Args1) / sizeof(Args1[0]), 1 << target->ID());

			std::int64_t Args2[] = { (std::int64_t)eRemoteEvent::Crash2, (std::int64_t)PLAYER::GET_PLAYER_INDEX(), rand() % INT64_MAX };
			Pointers::pTriggerScriptEvent(1, Args2, sizeof(Args2) / sizeof(Args2[0]), 1 << target->ID());

			std::int64_t Args3[] = { (std::int64_t)eRemoteEvent::Crash3, (std::int64_t)PLAYER::GET_PLAYER_INDEX(), rand() % INT64_MAX, rand() % INT64_MAX };
			Pointers::pTriggerScriptEvent(1, Args3, sizeof(Args3) / sizeof(Args3[0]), 1 << target->ID());
		});
	}
}