#pragma once 
#include "../Common.hpp"
#include "Math.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptManager.hpp"

namespace Sentinel::Weapons
{
	inline bool IsPedShooting(Ped ped)
	{
		// PED::IS_PED_SHOOTING
		const auto Coords = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
		return PED::IS_PED_SHOOTING_IN_AREA(ped, Coords.x, Coords.y, Coords.z, Coords.x, Coords.y, Coords.z, TRUE, TRUE);
	}

	inline bool IsPedAiming(Ped ped)
	{
		return PED::GET_PED_CONFIG_FLAG(ped, 78, TRUE);
	}

	inline Entity GetCurrent(Ped ped)
	{
		return WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(ped, NULL);
	}

	inline BOOL g_ShapeTestHit;
	inline std::int32_t g_ShapeTestEntity;
	inline NativeVector3 g_ShapeTestDestination;
	inline NativeVector3 g_ShapeTestSurface;
	inline std::int32_t g_ShapeTestRay;

	inline void Raycast()
	{
		if (IsPedShooting(PLAYER::PLAYER_PED_ID())) 
		{
			const auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(GetCurrent(PLAYER::PLAYER_PED_ID()), 1.5f, 0.f, 0.f);
			const auto AimCoords = Math::CoordsInFrontOfCam(1000.f);
			g_ShapeTestHit = FALSE;
			g_ShapeTestEntity = NULL;
			g_ShapeTestDestination = {};
			g_ShapeTestSurface = {};
			g_ShapeTestRay = NULL;
			g_ShapeTestRay = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(Coords.x, Coords.y, Coords.z, AimCoords.x, AimCoords.y, AimCoords.z, -1, PLAYER::PLAYER_PED_ID(), 7);
			SHAPETEST::GET_SHAPE_TEST_RESULT(g_ShapeTestRay, &g_ShapeTestHit, &g_ShapeTestDestination, &g_ShapeTestSurface, &g_ShapeTestEntity);
		}
	}

	inline static void RaycastTick()
	{
		while (true)
		{
			Raycast();
			Script::Current()->Yield();
		}
	}

	inline NativeVector3 GetAimCoords()
	{
		BOOL Hit;
		NativeVector3 EndCoords;
		NativeVector3 SurfaceNormal;

		NativeVector3 CamCoords = CAM::GET_GAMEPLAY_CAM_COORD();
		NativeVector3 Direction = Math::RotationToDirection(CAM::GET_GAMEPLAY_CAM_ROT(2));
		NativeVector3 FarCoords;

		FarCoords.x = CamCoords.x + Direction.x * 1000.f;
		FarCoords.y = CamCoords.y + Direction.y * 1000.f;
		FarCoords.z = CamCoords.z + Direction.z * 1000.f;

		Entity Entity;
		const int Cast = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(CamCoords.x, CamCoords.y, CamCoords.z, FarCoords.x, FarCoords.y, FarCoords.z, -1, PLAYER::PLAYER_PED_ID(), 7);
		SHAPETEST::GET_SHAPE_TEST_RESULT(Cast, &Hit, &EndCoords, &SurfaceNormal, &Entity);

		return EndCoords;
	}
}