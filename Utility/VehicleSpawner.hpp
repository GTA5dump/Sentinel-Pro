#pragma once
#include "System.hpp"
#include "../Pointers/Pointers.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/Script.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "Joaat.hpp"

namespace Sentinel::SpawnVehicles
{
	struct VehicleSpawner
	{
		std::string m_Name;
		std::uint32_t m_Model;
	};
	inline std::vector<VehicleSpawner> g_CompactsVector;
	inline std::vector<VehicleSpawner> g_SedansVector;
	inline std::vector<VehicleSpawner> g_SUVsVector;
	inline std::vector<VehicleSpawner> g_CoupesVector;
	inline std::vector<VehicleSpawner> g_MuscleVector;
	inline std::vector<VehicleSpawner> g_SportsClassicsVector;
	inline std::vector<VehicleSpawner> g_SportsVector;
	inline std::vector<VehicleSpawner> g_SuperVector;
	inline std::vector<VehicleSpawner> g_MotorcyclesVector;
	inline std::vector<VehicleSpawner> g_OffRoadVector;
	inline std::vector<VehicleSpawner> g_IndustrialVector;
	inline std::vector<VehicleSpawner> g_UtilityVector;
	inline std::vector<VehicleSpawner> g_VansVector;
	inline std::vector<VehicleSpawner> g_CyclesVector;
	inline std::vector<VehicleSpawner> g_BoatsVector;
	inline std::vector<VehicleSpawner> g_HelicoptersVector;
	inline std::vector<VehicleSpawner> g_PlanesVector;
	inline std::vector<VehicleSpawner> g_ServiceVector;
	inline std::vector<VehicleSpawner> g_EmergencyVector;
	inline std::vector<VehicleSpawner> g_MilitaryVector;
	inline std::vector<VehicleSpawner> g_CommercialVector;
	inline std::vector<VehicleSpawner> g_TrainsVector;
	inline std::vector<VehicleSpawner> g_OpenWheelVector;
	inline bool g_SpawnUpgraded = false;
	inline bool g_SpawnInVehicle = false;
	inline bool g_DeletePrevious = false;
	inline bool g_DespawnBypass = true;
	inline bool g_PreventCrash = true;
	inline bool g_SpawnInAir = true;
	inline bool g_FadeAnimation = true;
	inline Hash g_SelectedVehicle;

	inline void PushIntoVector(int veh_class)
	{
		if (Pointers::pListPool != nullptr)
		{
			for (std::int32_t Index = 0; Index < Pointers::pListPool->capacity; Index++)
			{
				std::uint64_t Info = Pointers::pListPool->get(Index);
				if (Info != NULL)
				{
					if ((*(BYTE*)(Info + 157) & 0x1F) == 5)
					{
						std::string MakePointer = (char*)((std::uintptr_t)Info + 0x2A4);
						std::string ModelPointer = (char*)((std::uintptr_t)Info + 0x298);

						if (VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(rage::joaat(ModelPointer)) == veh_class)
						{
							std::stringstream StringStream;
							std::string Make(MakePointer);
							std::string Model(ModelPointer);

							if (Make[0] || Model[0])
							{
								Make = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(Make.c_str());
								Model = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(Model.c_str());

								if (Make != "NULL" && Make != "NULL") {
									StringStream << Make << " " << Model;
								}
								else if (Model != "NULL") {
									StringStream << Model;
								}
								else {
									StringStream << "Unknown";
								}

								StringStream << "##" <<  std::to_string(Math::RandomNumberInRange(0, INT_MAX));
							}

							// Push Into Vector
							switch (veh_class)
							{
							case 0:
								g_CompactsVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 1:
								g_SedansVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 2:
								g_SUVsVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 3:
								g_CoupesVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 4:
								g_MuscleVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 5:
								g_SportsClassicsVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 6:
								g_SportsVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 7:
								g_SuperVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 8:
								g_MotorcyclesVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 9:
								g_OffRoadVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 10:
								g_IndustrialVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 11:
								g_UtilityVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 12:
								g_VansVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 13:
								g_CyclesVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 14:
								g_BoatsVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 15:
								g_HelicoptersVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 16:
								g_PlanesVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 17:
								g_ServiceVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 18:
								g_EmergencyVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 19:
								g_MilitaryVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 20:
								g_CommercialVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 21:
								g_TrainsVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							case 22:
								g_OpenWheelVector.push_back({ StringStream.str(), *(std::uint32_t*)(Info + 0x18) });
								break;
							}
						}
					}
				}
			}
		}
	}

	inline void AllocateVectors()
	{
		for (int Iterator = 0; Iterator < 23; Iterator++) {
			PushIntoVector(Iterator);
		}
	}

	inline Vehicle CreateVehicle(Hash model, bool vehspawner = false)
	{
		if (vehspawner && g_PreventCrash)
		{
			if (model == Joaat("cyclone2"))
				return 0;

			if (model == Joaat("ignus2"))
				return 0;

			if (model == Joaat("astron2"))
				return 0;

			if (model == Joaat("arbitergt"))
				return 0;

			if (model == Joaat("s95"))
				return 0;
		}

		if (vehspawner && g_DeletePrevious) {
			auto Previous = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);
			if (g_FadeAnimation && *Pointers::pIsSessionStarted)
			{
				NETWORK::NETWORK_FADE_OUT_ENTITY(Previous, TRUE, FALSE);
				Script::Current()->Yield(500ms);
			}
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Previous, TRUE, TRUE);
			ENTITY::DELETE_ENTITY(&Previous);
		}

		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			STREAMING::REQUEST_MODEL(model);
			Script::Current()->Yield(5ms);
		}

		auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);

		System::ModelSpawnBypass(true);
		const auto Vehicle = VEHICLE::CREATE_VEHICLE(model, Coords.x, Coords.y, Coords.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), TRUE, TRUE, FALSE);
		System::ModelSpawnBypass(false);

		Script::Current()->Yield();

		if (*Pointers::pIsSessionStarted)
		{
			DECORATOR::DECOR_SET_INT(Vehicle, "MPBitset", 0);
			ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Vehicle, TRUE);
			const auto NID = NETWORK::VEH_TO_NET(Vehicle);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Vehicle))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
			VEHICLE::SET_VEHICLE_IS_STOLEN(Vehicle, FALSE);
		}

		if (vehspawner)
		{
			if (g_SpawnInVehicle) {
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, -1);
			}

			if (g_SpawnUpgraded && !VEHICLE::IS_THIS_MODEL_A_PLANE(model) && !VEHICLE::IS_THIS_MODEL_A_HELI(model)) {
				VEHICLE::SET_VEHICLE_FIXED(Vehicle);
				VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0.f);
				VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);

				for (int Iterator = MOD_SPOILER; Iterator <= MOD_ARMOR; Iterator++) {
					if (VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, Iterator) > 0) {
						VEHICLE::SET_VEHICLE_MOD(Vehicle, Iterator, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, Iterator) - 1, FALSE);
					}
				}

				for (int Iterator = MOD_PLATEHOLDER; Iterator <= MOD_LIVERY; Iterator++) {
					if (VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, Iterator) > 0) {
						VEHICLE::SET_VEHICLE_MOD(Vehicle, Iterator, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, Iterator) - 1, FALSE);
					}
				}

				VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_HORNS, 30, FALSE);

				if (ENTITY::GET_ENTITY_MODEL(Vehicle) != Joaat("vigilante")) {
					VEHICLE::SET_VEHICLE_WHEEL_TYPE(Vehicle, 7);
					VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_FRONTWHEELS, 39, TRUE);
					VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_BACKWHEELS, 39, TRUE);
				}

				for (int Iterator = MOD_UNK17; Iterator <= MOD_XENONLIGHTS; Iterator++) {
					VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, Iterator, TRUE);
				}

				VEHICLE::SET_VEHICLE_WINDOW_TINT(Vehicle, 1);
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle, 99, 66, 245);
				VEHICLE::SET_VEHICLE_NEON_COLOUR(Vehicle, 99, 66, 245);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle, 99, 66, 245);
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle, 0, 0, 0);
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle, 5);
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, "SENTINEL");

				for (int Iterator = 0; Iterator < 4; Iterator++) {
					VEHICLE::SET_VEHICLE_NEON_ENABLED(Vehicle, Iterator, TRUE);
				}

				// Performance: brakes, engine, armor, transmission, suspension, turbo, bullet proof tires
				VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_BRAKES, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_BRAKES) - 1, FALSE);
				VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_ENGINE, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_ENGINE) - 1, FALSE);
				VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_ARMOR, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_ARMOR) - 1, FALSE);
				VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_TRANSMISSION, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_TRANSMISSION) - 1, FALSE);
				VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_SUSPENSION, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_SUSPENSION) - 1, FALSE);
				VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, MOD_TURBO, TRUE);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, FALSE);
			}

			if (g_SpawnInAir) {
				const auto Coords = ENTITY::GET_ENTITY_COORDS(Vehicle, FALSE);
				if (VEHICLE::IS_THIS_MODEL_A_PLANE(model) || VEHICLE::IS_THIS_MODEL_A_HELI(model))
				{
					ENTITY::SET_ENTITY_COORDS(Vehicle, Coords.x, Coords.y, Coords.z + 500.f, FALSE, FALSE, FALSE, FALSE);
				}
			}

			const auto PlayerHash = NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(PLAYER::PLAYER_ID());
			DECORATOR::DECOR_SET_INT(Vehicle, "Player_Vehicle", PlayerHash);
			DECORATOR::DECOR_SET_INT(Vehicle, "Previous_Owner", PlayerHash);
			DECORATOR::DECOR_SET_INT(Vehicle, "Veh_Modded_By_Player", PlayerHash);
			DECORATOR::DECOR_SET_INT(Vehicle, "Not_Allow_As_Saved_Veh", 0);
			DECORATOR::DECOR_SET_INT(Vehicle, "PV_Slot", 2);

			if (*Pointers::pIsSessionStarted && g_FadeAnimation) {
				NETWORK::NETWORK_FADE_IN_ENTITY(Vehicle, FALSE, NULL);
			}

			*ScriptGlobal(4540726).As<int*>() = g_DespawnBypass;

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		}

		return Vehicle;
	}
}