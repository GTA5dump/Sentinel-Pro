#pragma once 
#include "../Common.hpp"
#include "Lists.hpp"
#include "System.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/enums.hpp"
#include "../GTA/Script/ScriptManager.hpp"

namespace Sentinel::Teleport
{
	inline const char* const g_Transition[3]
	{
		"None",
		"Camera Glide",
		"Player Switch"
	};
	inline int g_TransitionPos = 1;
	inline bool g_KeepVehicle = true;

	inline void CameraGlideToCoords(float x, float y, float z, bool keep_veh = true)
	{
		static Cam TransitionCam;
		if (!CAM::DOES_CAM_EXIST(TransitionCam))
		{
			TransitionCam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
			keep_veh ? PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), x, y, z) : ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), x, y, z, FALSE, FALSE, FALSE, TRUE);
			CAM::SET_CAM_ACTIVE(TransitionCam, TRUE);
			CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 4000, TRUE, FALSE, NULL);
			STREAMING::SET_FOCUS_POS_AND_VEL(x, y, z, 0.f, 0.f, 0.f);
			CAM::SET_CAM_COORD(TransitionCam, x, y, (z + 500.f));
			STREAMING::SET_FOCUS_ENTITY(PLAYER::PLAYER_PED_ID());
			Script::Current()->Yield(4000ms);
			STREAMING::SET_FOCUS_ENTITY(PLAYER::PLAYER_PED_ID());
			CAM::SET_CAM_ACTIVE(TransitionCam, FALSE);
			CAM::RENDER_SCRIPT_CAMS(FALSE, TRUE, 3000, TRUE, FALSE, NULL);
			CAM::DESTROY_CAM(TransitionCam, FALSE);
		}
	}

	inline void PlayerSwitchToCoords(float x, float y, float z, bool keep_veh = true)
	{
		const auto ModelHash = 0x62018559;
		if (!STREAMING::HAS_MODEL_LOADED(ModelHash)) {
			STREAMING::REQUEST_MODEL(ModelHash);
			Script::Current()->Yield();
		}

		System::ModelSpawnBypass(true);
		auto CreatedPed = PED::CREATE_PED(1, ModelHash, x, y, z, 0.f, TRUE, FALSE);
		System::ModelSpawnBypass(false);
		STREAMING::START_PLAYER_SWITCH(PLAYER::PLAYER_PED_ID(), CreatedPed, 0, 0);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(CreatedPed, TRUE, TRUE);
		ENTITY::DELETE_ENTITY(&CreatedPed);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(ModelHash);
		if (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
		{
			while (STREAMING::GET_PLAYER_SWITCH_STATE() != 8)
			{
				Script::Current()->Yield();
				if (STREAMING::GET_PLAYER_SWITCH_STATE() == 12)
					break;
				keep_veh ? PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), x, y, z) : ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), x, y, z, FALSE, FALSE, FALSE, TRUE);
			}
		}
	}

	inline bool LoadGround(NativeVector3& location)
	{
		float GroundZ;
		const std::uint8_t Attempts = 10;

		for (std::uint8_t i = 0; i < Attempts; i++)
		{
			// Only request a collision after the first try failed because the location might already be loaded on first attempt
			for (std::uint16_t z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(location.x, location.y, (float)z);

				Script::Current()->Yield();
			}

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, 1000.f, &GroundZ, FALSE, FALSE))
			{
				location.z = GroundZ + 1.f;

				return true;
			}

			Script::Current()->Yield();
		}

		location.z = 1000.f;

		return false;
	}

	inline bool GetBlipLocation(NativeVector3& location, int sprite, int color = -1)
	{
		Blip Blip{};
		for (Blip = HUD::GET_FIRST_BLIP_INFO_ID(sprite); HUD::DOES_BLIP_EXIST(Blip) && color != -1 && HUD::GET_BLIP_COLOUR(Blip) != color; Blip = HUD::GET_NEXT_BLIP_INFO_ID(sprite))
			;

		if (!HUD::DOES_BLIP_EXIST(Blip) || (color != -1 && HUD::GET_BLIP_COLOUR(Blip) != color))
			return false;

		location = HUD::GET_BLIP_COORDS(Blip);

		return true;
	}

	inline bool GetObjectiveLocation(NativeVector3& location)
	{
		for (const auto& [icon, color] : Lists::g_BlipColors)
		{
			if (GetBlipLocation(location, icon, color))
				return true;
		}

		static const int Blips[] = { 1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288 };
		for (const auto& Blip : Blips)
		{
			if (GetBlipLocation(location, Blip, 5))
				return true;
		}

		return false;
	}

	inline bool ToBlip(Entity ent, int sprite, int color = -1)
	{
		NativeVector3 Coords{};

		if (!GetBlipLocation(Coords, sprite, color))
			return false;

		if (sprite == (int)eBlipIcons::Waypoint)
			LoadGround(Coords);

		ENTITY::SET_ENTITY_COORDS(ent, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
		return true;
	}

	inline bool ToWaypoint(Entity ent)
	{
		if (!ToBlip(ent, (int)eBlipIcons::Waypoint))
			return false;
		return true;
	}

	inline bool ToObjective(Entity ent)
	{
		NativeVector3 Coords{};

		if (!GetObjectiveLocation(Coords))
			return false;

		ENTITY::SET_ENTITY_COORDS(ent, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
		return true;
	}

	inline void LocalToCoords(float x, float y, float z)
	{
		switch (g_TransitionPos)
		{
		case 0:
			g_KeepVehicle ? PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), x, y, z) : ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), x, y, z, FALSE, FALSE, FALSE, TRUE);
			break;
		case 1:
			CameraGlideToCoords(x, y, z, g_KeepVehicle);
			break;
		case 2:
			PlayerSwitchToCoords(x, y, z, g_KeepVehicle);
			break;
		}
	}

	inline void NearestVehicle(bool driver)
	{
		const auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.f, 0.f, 0.f);
		const auto Vehicle = VEHICLE::GET_CLOSEST_VEHICLE(Coords.x, Coords.y, Coords.z, 10000.f, 0, 100359);
		const auto VehicleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Vehicle, 0.f, 0.f, 0.f);

		if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
		{
			switch (g_TransitionPos)
			{
			case 0:
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, FALSE, FALSE, FALSE, TRUE);
				break;
			case 1:
				CameraGlideToCoords(VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, false);
				break;
			case 2:
				PlayerSwitchToCoords(VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, false);
				break;
			}

			if (driver)
			{
				if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, -1, FALSE))
				{
					const auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, -1, FALSE);
					PED::IS_PED_A_PLAYER(Ped) ? Pointers::pClearPedTasksNetworked(g_PlayerService->GetByPed(Ped)->GetPed(), true) : Entities::ClearPedTasks(Ped);
				}

				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, -1);
			}
			else
			{
				for (int Iterator = 0; Iterator < VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(Vehicle)); Iterator++)
				{
					if (VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, Iterator, FALSE) && Iterator != -1)
						PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, Iterator);
					return;
				}
			}
		}
	}

	inline void LastVehicle()
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			return;

		const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);
		const auto VehicleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Vehicle, 0.f, 0.f, 0.f);

		if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
		{
			switch (g_TransitionPos)
			{
			case 0:
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, FALSE, FALSE, FALSE, TRUE);
				break;
			case 1:
				CameraGlideToCoords(VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, false);
				break;
			case 2:
				PlayerSwitchToCoords(VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, false);
				break;
			}

			if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, -1, FALSE))
			{
				const auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, -1, FALSE);
				PED::IS_PED_A_PLAYER(Ped) ? Pointers::pClearPedTasksNetworked(g_PlayerService->GetByPed(Ped)->GetPed(), true) : Entities::ClearPedTasks(Ped);
			}

			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, -1);
		}
	}

	inline void PersonalVehicle()
	{
		const auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.f, 0.f, 0.f);
		const auto PersonalVehicle = *ScriptGlobal(2793046).Add(299).As<Vehicle*>();
		const auto VehicleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PersonalVehicle, 0.f, 0.f, 0.f);

		if (ENTITY::DOES_ENTITY_EXIST(PersonalVehicle))
		{
			switch (g_TransitionPos)
			{
			case 0:
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, FALSE, FALSE, FALSE, TRUE);
				break;
			case 1:
				CameraGlideToCoords(VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, false);
				break;
			case 2:
				PlayerSwitchToCoords(VehicleCoords.x + 10.f, VehicleCoords.y + 10.f, VehicleCoords.z, false);
				break;
			}

			if (!VEHICLE::IS_VEHICLE_SEAT_FREE(PersonalVehicle, -1, FALSE))
			{
				const auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(PersonalVehicle, -1, FALSE);
				PED::IS_PED_A_PLAYER(Ped) ? Pointers::pClearPedTasksNetworked(g_PlayerService->GetByPed(Ped)->GetPed(), true) : Entities::ClearPedTasks(Ped);
			}

			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PersonalVehicle, -1);
		}
	}

	inline float g_DirectionalDistance = 5.f;
	inline void DirectionalTeleport(int direction)
	{
		Vector3 Coords = { 0.f, 0.f, 0.f };

		switch (direction) 
		{
		case 0:
			Coords.y = g_DirectionalDistance + 1.f;
			break;
		case 1:
			Coords.y = (g_DirectionalDistance * -1.f) + 1.f;
			break;
		case 2:
			Coords.z = g_DirectionalDistance + 1.f;
			break;
		case 3:
			Coords.z = (g_DirectionalDistance * -1.f) + 1.f;
			break;
		case 4:
			Coords.x = (g_DirectionalDistance * -1.f) + 1.f;
			break;
		case 5:
			Coords.x = g_DirectionalDistance + 1.f;
			break;
		}

		const auto WorldCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
		LocalToCoords(WorldCoords.x, WorldCoords.y, WorldCoords.z);
	}

	inline bool LocalToObjective()
	{
		NativeVector3 Coords{};

		if (!GetObjectiveLocation(Coords))
			return false;

		LocalToCoords(Coords.x, Coords.y, Coords.z);
		return true;
	}

	inline bool LocalToWaypoint()
	{
		NativeVector3 Coords{};

		if (!GetBlipLocation(Coords, (int)eBlipIcons::Waypoint, -1))
			return false;

		LoadGround(Coords);

		LocalToCoords(Coords.x, Coords.y, Coords.z);
		return true;
	}
}