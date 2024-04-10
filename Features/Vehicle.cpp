#include "Vehicle.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/Kicks.hpp"
#include "../GTA/enums.hpp"
#include "../Pointers/Pointers.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/Script/ScriptLocal.hpp"
#include "../Utility/Timer.hpp"
#include "../Utility/Joaat.hpp"
#include "../Utility/Math.hpp"
#include "../GUI/GUI.hpp"
#include "../GUI/Overlays/Overlays.hpp"
#include "../Utility/All.hpp"

namespace Sentinel
{
	void VehicleFeatures::GodMode(bool enable)
	{
		if (enable) {
			GTA::Offsets::SetValue<bool>({ 0x8, 0xD10, 0x189 }, true);
		}
	}

	void VehicleFeatures::AutoRepair(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (VEHICLE::GET_DOES_VEHICLE_HAVE_DAMAGE_DECALS(Vehicle))
				VEHICLE::SET_VEHICLE_FIXED(Vehicle);

			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);

			if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(Vehicle) > 1000.f)
				PED::IS_PED_IN_ANY_PLANE(PLAYER::PLAYER_PED_ID()) ? VEHICLE::SET_PLANE_ENGINE_HEALTH(Vehicle, 1000.f) : VEHICLE::SET_VEHICLE_ENGINE_HEALTH(Vehicle, 1000.f);
		}
	}

	void VehicleFeatures::RenderBurnt(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			ENTITY::SET_ENTITY_RENDER_SCORCHED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE);
	}

	void VehicleFeatures::AutoWash(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 0.f);
	}

	void VehicleFeatures::AutoFlip(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (!ENTITY::IS_ENTITY_UPRIGHT(Vehicle, 120.f))
			{
				const auto Rotation = ENTITY::GET_ENTITY_ROTATION(Vehicle, 0);
				ENTITY::SET_ENTITY_ROTATION(Vehicle, 0.f, Rotation.y, Rotation.z, 0, TRUE);
			}
		}
	}

	bool g_LastAntiLockon = false;
	void VehicleFeatures::AntiLockon(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			VEHICLE::SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE, FALSE);
		}
		else if (enable != g_LastAntiLockon)
			VEHICLE::SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE, FALSE);

		g_LastAntiLockon = enable;
	}

	void VehicleFeatures::AntiStickyBomb(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			// Ped argument can be NULL
			NETWORK::REMOVE_ALL_STICKY_BOMBS_FROM_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), NULL);
		}
	}

	void VehicleFeatures::EngineAlwaysOn(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			VEHICLE::SET_VEHICLE_ENGINE_ON(Vehicle, TRUE, TRUE, TRUE);
			VEHICLE::SET_VEHICLE_LIGHTS(Vehicle, 0);
			VEHICLE::SET_VEHICLE_HEADLIGHT_SHADOWS(Vehicle, 2);
		}
	}

	void VehicleFeatures::DisableFullCollision(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE, TRUE);
	}

	void VehicleFeatures::CollisionManager()
	{
		if (!m_DisableCollisionWithVehicles && !m_DisableCollisionWithPeds && !m_DisableCollisionWithObjects || !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			return;

		for (const auto Entity : Entities::GetEntities(m_DisableCollisionWithVehicles, m_DisableCollisionWithPeds, m_DisableCollisionWithObjects))
		{
			ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(Entity, PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE);
		}
	}

	static const eControllerInputs g_NoClipControls[] =
	{
		eControllerInputs::INPUT_SPRINT,
		eControllerInputs::INPUT_DUCK,
		eControllerInputs::INPUT_MOVE_UP_ONLY,
		eControllerInputs::INPUT_MOVE_DOWN_ONLY
	};

	void VehicleFeatures::NoClip(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			// Disable Controls
			for (const auto& Control : g_NoClipControls)
				PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(Control), TRUE);

			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			auto Position = ENTITY::GET_ENTITY_COORDS(Vehicle, TRUE);
			auto CamCoords = CAM::GET_GAMEPLAY_CAM_COORD();
			const auto CamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			auto Distance = std::distance(&CamCoords, &Position);

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Vehicle, Position.x, Position.y, Position.z, TRUE, TRUE, TRUE);
			ENTITY::SET_ENTITY_ROTATION(Vehicle, CamRot.x, CamRot.y, CamRot.z, 2, TRUE);

			// Forward
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControllerInputs::INPUT_MOVE_UP_ONLY))
			{
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControllerInputs::INPUT_SPRINT))
					Distance += 3.0;
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControllerInputs::INPUT_DUCK))
					Distance -= 1.f;

				Distance += m_NoClipSpeed;
				Distance += 1.f;
				const auto Coords = Math::Add(Position, Math::Multiply(Math::RotationToDirection(CamRot), Distance));
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Vehicle, Coords.x, Coords.y, Coords.z, TRUE, TRUE, TRUE);
			}

			// Backward
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControllerInputs::INPUT_MOVE_DOWN_ONLY))
			{
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControllerInputs::INPUT_SPRINT))
					Distance -= 3.0;
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControllerInputs::INPUT_DUCK))
					Distance += 1.f;
				Distance -= m_NoClipSpeed;
				Distance -= .3f;
				const auto Coords = Math::Add(Position, Math::Multiply(Math::RotationToDirection(CamRot), Distance));
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Vehicle, Coords.x, Coords.y, Coords.z, TRUE, TRUE, TRUE);
			}
		}
	}

	void VehicleFeatures::VehicleFly(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED((int)('W')) || PAD::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_RT)) // Forward
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, (ENTITY::GET_ENTITY_SPEED(Vehicle) + 0.5f));
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED((int)('S')) || PAD::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_LT)) // Backwards
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, -((ENTITY::GET_ENTITY_SPEED(Vehicle) + 0.5f) * 2.f), 0.f, 0.f, 0.f, 0.f, 1, TRUE, TRUE, TRUE, FALSE, TRUE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED(VK_SPACE) || PAD::IS_CONTROL_PRESSED(2, INPUT_VEH_BRAKE)) // Stop
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 0.f);
			if (KEYBOARD::IS_KEYBOARD_INPUT_JUST_PRESSED(VK_LEFT) || PAD::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_LEFT)) { // Left
				if (!VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vehicle)) {
					const auto LeftRotation = ENTITY::GET_ENTITY_ROTATION(Vehicle, 2);
					std::float_t NewRotation = LeftRotation.z;
					NewRotation += 1.f;
					ENTITY::SET_ENTITY_ROTATION(Vehicle, LeftRotation.x, LeftRotation.y, NewRotation, 2, 1);
				}
			}
			if (KEYBOARD::IS_KEYBOARD_INPUT_JUST_PRESSED(VK_RIGHT) || PAD::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_RIGHT)) { // Right
				if (!VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vehicle)) {
					const auto RightRotation = ENTITY::GET_ENTITY_ROTATION(Vehicle, 2);
					std::float_t NewRotation = RightRotation.z;
					NewRotation -= 1.f;
					ENTITY::SET_ENTITY_ROTATION(Vehicle, RightRotation.x, RightRotation.y, NewRotation, 2, 1);
				}
			}
			if (ENTITY::GET_ENTITY_SPEED(Vehicle) >= 40.f && VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vehicle))
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, 0.f, 0.28f, 0.f, 2.5f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
		}
	}

	void VehicleFeatures::Hydraulics(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED((int)('W')) || PAD::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_RT))
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, 0.f, 1.f, 0.f, 2.f, 0.f, 1, TRUE, TRUE, TRUE, FALSE, TRUE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED((int)('A')) || PAD::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_LT))
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, 0.f, 1.f, -2.f, 0.f, 0.f, 1, TRUE, TRUE, TRUE, FALSE, TRUE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED((int)('D')) || PAD::IS_CONTROL_PRESSED(2, INPUT_VEH_MOVE_RIGHT))
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, 0.f, 1.f, 2.f, 0.f, 0.f, 1, TRUE, TRUE, TRUE, FALSE, TRUE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED((int)('S')) || PAD::IS_CONTROL_PRESSED(2, INPUT_VEH_MOVE_DOWN))
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, 0.f, 1.f, 0.f, -2.f, 0.f, 1, TRUE, TRUE, TRUE, FALSE, TRUE);
		}
	}

	void VehicleFeatures::DriftMode(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (KEYBOARD::IS_KEYBOARD_INPUT_PRESSED(VK_SHIFT) || PAD::IS_CONTROL_PRESSED(2, INPUT_SPRINT)) {
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, 0.5f, 0.f, 0.f, 0.f, 0.f, true, TRUE, TRUE, TRUE, FALSE, TRUE);
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(Vehicle, TRUE);
			}
			else
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(Vehicle, FALSE);
		}
	}

	void VehicleFeatures::TumbleDryer(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vehicle)) {
				const auto Speed = ENTITY::GET_ENTITY_SPEED_VECTOR(Vehicle, TRUE);
				ENTITY::SET_ENTITY_VELOCITY(Vehicle, Speed.x * 1.01f, Speed.y * 1.01f, Speed.z * 1.01f);
			}
		}
	}

	// SET_VEHICLE_ON_GROUND_PROPERLY p1 is always 5.f in the scripts
	void VehicleFeatures::StickyTires(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 5.f);
	}

	void VehicleFeatures::SlippyWheels(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE);
	}

	void VehicleFeatures::NoGravity(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_GRAVITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
	}

	Object g_WaterContainer;
	bool g_LastDriveOnWater = false;
	void VehicleFeatures::DriveOnWater(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			const auto VehicleCoords = ENTITY::GET_ENTITY_COORDS(Vehicle, FALSE);
			float WaterHeight; WATER::GET_WATER_HEIGHT(VehicleCoords.x, VehicleCoords.y, VehicleCoords.z, &WaterHeight);
			const auto Model = Joaat("prop_container_ld2");
			NativeVector3 cMin, cMax;

			MISC::GET_MODEL_DIMENSIONS(Model, &cMin, &cMax);
			if (!STREAMING::HAS_MODEL_LOADED(Model))
				STREAMING::REQUEST_MODEL(Model);
			if (!ENTITY::DOES_ENTITY_EXIST(g_WaterContainer) || (Math::DistanceBetweenVectors(ENTITY::GET_ENTITY_COORDS(g_WaterContainer, FALSE), VehicleCoords) > 3.f))
			{
				if (ENTITY::DOES_ENTITY_EXIST(g_WaterContainer)) {
					Entities::ChangeNetworkObjectOwner(g_WaterContainer, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
					ENTITY::SET_ENTITY_COORDS(g_WaterContainer, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(g_WaterContainer, TRUE, TRUE);
					ENTITY::DELETE_ENTITY(&g_WaterContainer);
				}
				System::ModelSpawnBypass(true);
				g_WaterContainer = OBJECT::CREATE_OBJECT(Model, VehicleCoords.x, VehicleCoords.y, WaterHeight - cMax.z - .02f, TRUE, TRUE, FALSE);
				System::ModelSpawnBypass(false);
				ENTITY::SET_ENTITY_ROTATION(g_WaterContainer, 0.f, 0.f, ENTITY::GET_ENTITY_HEADING(Vehicle), 0, FALSE);
				ENTITY::SET_ENTITY_VISIBLE(g_WaterContainer, FALSE, FALSE);
				NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(g_WaterContainer, TRUE);
			}
		}
		else if (enable != g_LastDriveOnWater && ENTITY::DOES_ENTITY_EXIST(g_WaterContainer))
		{
			Entities::ChangeNetworkObjectOwner(g_WaterContainer, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
			ENTITY::SET_ENTITY_COORDS(g_WaterContainer, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(g_WaterContainer, TRUE, TRUE);
			ENTITY::DELETE_ENTITY(&g_WaterContainer);
		}

		g_LastDriveOnWater = enable;
	}

	void VehicleFeatures::DriveThroughWalls(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 5.f);
	}

	Object g_AirContainer;
	bool g_LastDriveOnAir = false;
	void VehicleFeatures::DriveOnAir(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			const auto VehicleCoords = ENTITY::GET_ENTITY_COORDS(Vehicle, FALSE);
			const auto Model = Joaat("prop_container_ld2");
			NativeVector3 cMin, cMax;

			MISC::GET_MODEL_DIMENSIONS(Model, &cMin, &cMax);
			if (!STREAMING::HAS_MODEL_LOADED(Model))
				STREAMING::REQUEST_MODEL(Model);
			if (!ENTITY::DOES_ENTITY_EXIST(g_AirContainer))
			{
				System::ModelSpawnBypass(true);
				g_AirContainer = OBJECT::CREATE_OBJECT(Model, VehicleCoords.x, VehicleCoords.y, VehicleCoords.z - cMax.z - 1.f, TRUE, TRUE, FALSE);
				System::ModelSpawnBypass(false);
				ENTITY::SET_ENTITY_ROTATION(g_AirContainer, 0.f, 0.f, ENTITY::GET_ENTITY_HEADING(Vehicle), 0, FALSE);
				ENTITY::SET_ENTITY_VISIBLE(g_AirContainer, FALSE, FALSE);
				NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(g_AirContainer, TRUE);
			}
			if (ENTITY::DOES_ENTITY_EXIST(g_AirContainer)) {
				ENTITY::FREEZE_ENTITY_POSITION(g_AirContainer, TRUE);
				ENTITY::SET_ENTITY_COORDS(g_AirContainer, VehicleCoords.x, VehicleCoords.y, ENTITY::GET_ENTITY_COORDS(g_AirContainer, FALSE).z, FALSE, FALSE, FALSE, FALSE);
				ENTITY::SET_ENTITY_ROTATION(g_AirContainer, 0.f, 0.f, ENTITY::GET_ENTITY_HEADING(Vehicle), 0, FALSE);
			}
		}
		else if (enable != g_LastDriveOnAir && ENTITY::DOES_ENTITY_EXIST(g_AirContainer))
		{
			Entities::ChangeNetworkObjectOwner(g_AirContainer, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
			ENTITY::SET_ENTITY_COORDS(g_AirContainer, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(g_AirContainer, TRUE, TRUE);
			ENTITY::DELETE_ENTITY(&g_AirContainer);
			g_AirContainer = 0;
		}

		g_LastDriveOnAir = enable;
	}

	void VehicleFeatures::BypassMaxSpeed(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_MAX_SPEED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 9999.f);
	}

	bool g_LastNoTurbulence = false;
	void VehicleFeatures::NoTurbulence(bool enable)
	{
		const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
		if (enable && PED::IS_PED_IN_FLYING_VEHICLE(PLAYER::PLAYER_PED_ID()))
		{
			if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(Vehicle))) {
				VEHICLE::SET_PLANE_TURBULENCE_MULTIPLIER(Vehicle, 0.f);
			}
		}
		else if (enable != g_LastNoTurbulence && PED::IS_PED_IN_FLYING_VEHICLE(PLAYER::PLAYER_PED_ID()))
		{
			if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(Vehicle))) {
				VEHICLE::SET_PLANE_TURBULENCE_MULTIPLIER(Vehicle, 0.5f);
			}
		}

		g_LastNoTurbulence = enable;
	}

	void VehicleFeatures::HandBrake(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE) && PAD::IS_CONTROL_PRESSED(0, INPUT_VEH_HANDBRAKE))
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 0.f);
	}

	void VehicleFeatures::InstantlyEnter(bool enable)
	{
		if (enable && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			if (TASK::GET_IS_TASK_ACTIVE(PLAYER::PLAYER_PED_ID(), 160))
			{
				const auto Vehicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(PLAYER::PLAYER_PED_ID());
				const auto Index = PED::GET_SEAT_PED_IS_TRYING_TO_ENTER(PLAYER::PLAYER_PED_ID());
				TASK::TASK_ENTER_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, 1, Index, 1.f, 16, 0);
			}
		}
	}

	void VehicleFeatures::InstantlyExit(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_VEH_EXIT, TRUE);
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_VEH_EXIT))
				Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());
		}
	}

	void VehicleFeatures::EnableParticleTrail(bool enable)
	{
		if (enable) 
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (m_ParticleTrailPlayStationary || (!m_ParticleTrailPlayStationary && ENTITY::GET_ENTITY_SPEED(Vehicle) > 1.f)) 
			{
				for (int Iterator = 0; Iterator < std::size(m_ActiveVehicleBones); Iterator++)
				{
					if (m_ActiveVehicleBones[Iterator])
					{
						const auto BoneIndex = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(Vehicle, Lists::g_VehicleBones[Iterator].c_str());
						if (BoneIndex != -1) {
							const auto BoneCoords = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(Vehicle, BoneIndex);
							const auto Asset = std::get<0>(Lists::g_VehicleTrailParticles[m_VehicleParticlesPos]);
							const auto Name = std::get<1>(Lists::g_VehicleTrailParticles[m_VehicleParticlesPos]);
							STREAMING::REQUEST_NAMED_PTFX_ASSET(Asset);
							GRAPHICS::USE_PARTICLE_FX_ASSET(Asset);
							if (STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(Asset))
								GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(Name, BoneCoords.x, BoneCoords.y, BoneCoords.z, 0.f, 0.f, 0.f, m_ParticleTrailScale, FALSE, FALSE, FALSE, FALSE);
						}
					}
				}
			}
		}
	}

	void VehicleFeatures::EnableBoost(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			if (!PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_ID()))
				return;

			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (m_BoostSound)
				AUDIO::SET_VEHICLE_BOOST_ACTIVE(Vehicle, TRUE);
			if (m_BoostEffect)
				GRAPHICS::ANIMPOSTFX_PLAY("RaceTurbo", 0, FALSE);
			if (m_BoostForwards)
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, m_BoostSpeed);
			if (m_BoostBackwards)
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, 0.f, -(m_BoostSpeed), 0.f, 0.f, 0.f, 0.f, 1, TRUE, TRUE, TRUE, FALSE, TRUE);
			if (m_BoostSound)
				AUDIO::SET_VEHICLE_BOOST_ACTIVE(Vehicle, FALSE);
		}
	}

	void VehicleFeatures::EnableVehicleWeapon(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			// Took some inspiration from Ozark, re did their math though, now it looks actually decent.
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			static int Timer{};
			NativeVector3 MinimumModelSize{};
			NativeVector3 MaximumModelSize{};
			MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(Vehicle), &MinimumModelSize, &MaximumModelSize);

			NativeVector3 OriginLeft{};
			NativeVector3 OriginRight{};
			NativeVector3 TargetLeft{};
			NativeVector3 TargetRight{};

			NativeVector3 Calculation = Math::Subtract(MaximumModelSize, MinimumModelSize);

			const auto Direction = Math::RotationToDirection(ENTITY::GET_ENTITY_ROTATION(Vehicle, 0));
			const auto Coords = ENTITY::GET_ENTITY_COORDS(Vehicle, FALSE);
			OriginLeft = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(Vehicle, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(Vehicle, Lists::g_VehicleBones[5].c_str()));
			OriginRight = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(Vehicle, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(Vehicle, Lists::g_VehicleBones[6].c_str()));

			switch (m_VehicleWeaponDirectionPos)
			{
			case 0:
				TargetLeft = Math::Add(Coords, (Math::Multiply(Direction, 100.f)));
				TargetRight = Math::Add(Coords, (Math::Multiply(Direction, 100.f)));
				break;
			case 1:
				TargetLeft = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Vehicle, (Calculation.x / 4.f) + 1.f, MaximumModelSize.y + 100.f, 0.5f);
				TargetRight = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Vehicle, -(Calculation.x / 4.f) - 1.f, MaximumModelSize.y + 100.f, 0.5f);
				break;
			case 2:
				TargetLeft = Math::Add(OriginLeft, (Math::Multiply(Direction, 100.f)));
				TargetRight = Math::Add(OriginRight, (Math::Multiply(Direction, 100.f)));
				break;
			}

			if (m_ViewVehicleWeaponDirection) {
				GRAPHICS::DRAW_LINE(OriginLeft.x, OriginLeft.y, OriginLeft.z, TargetLeft.x, TargetLeft.y, TargetLeft.z, 99, 66, 245, 255);
				GRAPHICS::DRAW_LINE(OriginRight.x, OriginRight.y, OriginRight.z, TargetRight.x, TargetRight.y, TargetRight.z, 99, 66, 245, 255);
			}

			// Actually shoot the weapon asset
			Math::RunTimed(&Timer, 100, [=] {
				if (PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_ID()))
				{
					Hash WeaponHash = Lists::g_BulletChangerHashes[m_VehicleWeaponBulletPos];

					while (!WEAPON::HAS_WEAPON_ASSET_LOADED(WeaponHash))
					{
						WEAPON::REQUEST_WEAPON_ASSET(WeaponHash, 31, 0);
						Script::Current()->Yield(5ms);
					}

					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(OriginLeft.x, OriginLeft.y, OriginLeft.z, TargetLeft.x, TargetLeft.y, TargetLeft.z, 250, FALSE, WeaponHash, PLAYER::PLAYER_PED_ID(), m_VehicleWeaponAudible, !m_VehicleWeaponVisible, m_VehicleWeaponSpeed, Vehicle, NULL);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(OriginRight.x, OriginRight.y, OriginRight.z, TargetRight.x, TargetRight.y, TargetRight.z, 250, FALSE, WeaponHash, PLAYER::PLAYER_PED_ID(), m_VehicleWeaponAudible, !m_VehicleWeaponVisible, m_VehicleWeaponSpeed, Vehicle, NULL);
					WEAPON::REMOVE_WEAPON_ASSET(WeaponHash);
				}
			});
		}
	}

	void VehicleFeatures::AcrobaticOnHorn(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			static int Timer{};
			Math::RunTimed(&Timer, 500, [=] {
				const auto FirstVector = std::get<0>(Lists::g_VehicleAcrobatics[g_VehicleFeatures->m_VehicleAcrobaticsPos]);
				const auto SecondVector = std::get<1>(Lists::g_VehicleAcrobatics[g_VehicleFeatures->m_VehicleAcrobaticsPos]);
				if (PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_ID()))
					ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 1, FirstVector.x, FirstVector.y, FirstVector.z, SecondVector.x, SecondVector.y, SecondVector.z, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
			});
		}
	}

	void VehicleFeatures::InfiniteBoost(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			GTA::GetLocalPed()->m_vehicle->m_boost = 6.f;
		}
	}

	bool g_LastAllowBoost = false;
	void VehicleFeatures::AllowBoost(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			GTA::Offsets::SetValue<BYTE>({ 0x08, 0xD10, 0x20, 0x58B }, 95);
		}
		else if (enable != g_LastAllowBoost)
		{
			GTA::Offsets::SetValue<BYTE>({ 0x08, 0xD10, 0x20, 0x58B }, NULL);
		}

		g_LastAllowBoost = enable;
	}

	bool g_LastAllowJumping = false;
	void VehicleFeatures::AllowJumping(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			GTA::Offsets::SetValue<BYTE>({ 0x08, 0xD10, 0x20, 0x58B }, 97);
		}
		else if (enable != g_LastAllowJumping)
		{
			GTA::Offsets::SetValue<BYTE>({ 0x08, 0xD10, 0x20, 0x58B }, NULL);
		}
		g_LastAllowJumping = enable;
	}

	bool g_LastAllowParachute = false;
	void VehicleFeatures::AllowParachute(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			GTA::Offsets::SetValue<BYTE>({ 0x08, 0xD10, 0x20, 0x58C }, TRUE);
		}
		else if (enable != g_LastAllowParachute)
		{
			GTA::Offsets::SetValue<BYTE>({ 0x08, 0xD10, 0x20, 0x58C }, FALSE);
		}
		g_LastAllowParachute = enable;
	}

	void VehicleFeatures::InfiniteMissiles(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_AKULA_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_APC_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_CHERNO_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_DELUXO_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_OPPRESSOR2_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_OPPRESSOR_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_ROGUE_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_SCRAMJET_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_VIGILANTE_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_STRIKEFORCE_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_TAMPA_MISSILE, -1);
			VEHICLE::SET_VEHICLE_WEAPON_RESTRICTED_AMMO(Vehicle, VEHICLE_WEAPON_TAMPA_MORTAR, -1);
		}
	}

	void VehicleFeatures::InfiniteBombs(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_BOMB_AMMO(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 1000);
	}

	void VehicleFeatures::InfiniteCounterMeasures(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
			VEHICLE::SET_VEHICLE_COUNTERMEASURE_AMMO(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 1000);
	}

	void VehicleFeatures::IndicatorLights(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			static bool Left = false;
			static bool Right = false;
			static bool Both = false;
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_FRONTEND_LEFT, TRUE);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_FRONTEND_RIGHT, TRUE);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_FRONTEND_DOWN, TRUE);

			if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_LEFT))
			{
				Left = !Left;
				if (Left) {
					Right = false;
					Both = false;
				}
			}
			else if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_RIGHT))
			{
				Right = !Right;
				if (Right) {
					Left = false;
					Both = false;
				}
			}
			else if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_DOWN))
			{
				Both = !Both;
				if (Both) {
					Left = false;
					Right = false;
				}
			}
			else
			{
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 1, FALSE);
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 0, FALSE);
			}

			if (Left)
			{
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 1, TRUE);
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 0, FALSE);
			}
			else if (Right)
			{
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 1, FALSE);
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 0, TRUE);
			}
			else if (Both)
			{
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 1, TRUE);
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(Vehicle, 0, TRUE);
			}
		}
	}

	bool g_LastMuteSiren = false;
	void VehicleFeatures::MuteSiren(bool enable)
	{
		if (enable)
			VEHICLE::SET_VEHICLE_HAS_MUTED_SIRENS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE);
		else if (enable != g_LastMuteSiren)
			VEHICLE::SET_VEHICLE_HAS_MUTED_SIRENS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);

		g_LastMuteSiren = enable;
	}

	bool g_LastPlateSpeedo = false;
	void VehicleFeatures::PlateSpeedo(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			std::stringstream StringStream; StringStream.precision(0);
			m_SpeedoMeterMetricPos == 0 ? StringStream << std::fixed << ENTITY::GET_ENTITY_SPEED(Vehicle) * 2.236936f << " MPH" : StringStream << std::fixed << ENTITY::GET_ENTITY_SPEED(Vehicle) * 3.6f << " KMH";
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, StringStream.str().c_str());
		}
		else if (enable != g_LastPlateSpeedo)
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), "SENTINEL");

		g_LastPlateSpeedo = enable;
	}

	void VehicleFeatures::ScreenSpeedo(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			std::stringstream StringStream; StringStream.precision(0);
			m_SpeedoMeterMetricPos == 0 ? StringStream << std::fixed << ENTITY::GET_ENTITY_SPEED(Vehicle) * 2.236936f << " MPH" : StringStream << std::fixed << ENTITY::GET_ENTITY_SPEED(Vehicle) * 3.6f << " KMH";

			// Draw Native Text
			HUD::SET_TEXT_COLOUR(99, 66, 245, 255);
			HUD::SET_TEXT_FONT(6);
			HUD::SET_TEXT_SCALE(0.75f, 0.75f);
			HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)StringStream.str().c_str());
			HUD::END_TEXT_COMMAND_DISPLAY_TEXT(0.815f, 0.870f, NULL);
		}
	}

	void VehicleFeatures::EnableVehicleAlpha(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
			m_VehicleAlphaLevelPos == 5 ? ENTITY::RESET_ENTITY_ALPHA(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE)) : ENTITY::SET_ENTITY_ALPHA(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), m_VehicleAlphaLevelPos * 51, TRUE);
		}
	}

	bool g_LastVehicleInvisibility = false;
	void VehicleFeatures::VehicleInvisibility(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE);
			ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE, FALSE);
		}
		else if (enable != g_LastVehicleInvisibility)
		{
			NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
			ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE, FALSE);
		}

		g_LastVehicleInvisibility = enable;
	}

	void VehicleFeatures::EnableRainbowPaint(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			static int R, G, B = 0;

			if (m_RainbowPaintModePos == 1)
			{
				R = System::g_SlowRainbowColor.r; G = System::g_SlowRainbowColor.g; B = System::g_SlowRainbowColor.b;
			}
			else
			{
				R = Math::RandomNumberInRange(0, 255); G = Math::RandomNumberInRange(0, 255); B = Math::RandomNumberInRange(0, 255);
			}

			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), R, G, B);
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), R, G, B);
		}
	}

	void VehicleFeatures::EnableRainbowTireSmoke(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			static int R, G, B = 0;

			if (m_RainbowTireSmokeModePos == 1)
			{
				R = System::g_SlowRainbowColor.r; G = System::g_SlowRainbowColor.g; B = System::g_SlowRainbowColor.b;
			}
			else
			{
				R = Math::RandomNumberInRange(0, 255); G = Math::RandomNumberInRange(0, 255); B = Math::RandomNumberInRange(0, 255);
			}

			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), R, G, B);
		}
	}

	void VehicleFeatures::EnableRainbowNeons(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			static int R = 255;
			static int G, B = 0;

			if (m_RainbowNeonsModePos == 1)
			{
				R = System::g_SlowRainbowColor.r; G = System::g_SlowRainbowColor.g; B = System::g_SlowRainbowColor.b;
			}
			else
			{
				R = Math::RandomNumberInRange(0, 255); G = Math::RandomNumberInRange(0, 255); B = Math::RandomNumberInRange(0, 255);
			}

			for (int Iterator = 0; Iterator < 4; Iterator++) {
				VEHICLE::SET_VEHICLE_NEON_ENABLED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), Iterator, TRUE);
			}

			VEHICLE::SET_VEHICLE_NEON_COLOUR(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), R, G, B);
		}
	}

	void VehicleFeatures::RainbowHeadlights(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);

			VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
			if (VEHICLE::GET_VEHICLE_MOD(Vehicle, MOD_XENONLIGHTS) == -1)
				VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, MOD_XENONLIGHTS, TRUE);

			static int Timer = 0;
			Math::RunTimed(&Timer, 750, [=] {
				VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(Vehicle, Math::RandomNumberInRange(0, 12));
			});
		}
	}

	void VehicleFeatures::RandomMods(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
			const auto Class = Math::RandomNumberInRange(MOD_SPOILER - 1, MOD_LIVERY - 1);
			const auto Max = VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, Class);
			const auto Mod = Math::RandomNumberInRange(-2, Max + 1);
			VEHICLE::SET_VEHICLE_MOD(Vehicle, Class, Mod, FALSE);
		}
	}

	void VehicleFeatures::RandomAcrobatics(bool enable)
	{
		if (enable && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			static int Timer = 0;
			Math::RunTimed(&Timer, 5000, [=] {
				const auto Number = Math::RandomNumberInRange(0, std::size(Lists::g_VehicleAcrobatics));
				const auto FirstVector = std::get<0>(Lists::g_VehicleAcrobatics[Number]);
				const auto SecondVector = std::get<1>(Lists::g_VehicleAcrobatics[Number]);
				ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 1, FirstVector.x, FirstVector.y, FirstVector.z, SecondVector.x, SecondVector.y, SecondVector.z, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
			});
		}
	}

	void VehicleFeatures::EnableTireWidth(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed()->m_vehicle)
				return;

			const auto CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (m_PreviousVehicleHash != CurrentVehicle)
			{
				if (!IsTireWidthValueChanged(CurrentVehicle))
					m_OriginalTireWidthValues.push_back({ CurrentVehicle, Local::TireScaling::GetTireScale().x });

				Local::TireScaling::SetTireWidth(GetTireWidthValue(CurrentVehicle));
			}
		}
	}

	void VehicleFeatures::EnableTireHeight(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed()->m_vehicle)
				return;

			const auto CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (m_PreviousVehicleHash != CurrentVehicle)
			{
				if (!IsTireHeightValueChanged(CurrentVehicle))
					m_OriginalTireHeightValues.push_back({ CurrentVehicle, Local::TireScaling::GetTireScale().y });

				Local::TireScaling::SetTireHeight(GetTireHeightValue(CurrentVehicle));
			}
		}
	}

	void VehicleFeatures::EnableTireScale(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed()->m_vehicle)
				return;

			const auto CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
			if (m_PreviousVehicleHash != CurrentVehicle)
			{
				if (!IsTireWidthValueChanged(CurrentVehicle))
					m_OriginalTireWidthValues.push_back({ CurrentVehicle, Local::TireScaling::GetTireScale().x });

				if (!IsTireHeightValueChanged(CurrentVehicle))
					m_OriginalTireHeightValues.push_back({ CurrentVehicle, Local::TireScaling::GetTireScale().y });

				Local::TireScaling::SetTireWidth(GetTireScaleValue(CurrentVehicle).x);
				Local::TireScaling::SetTireHeight(GetTireScaleValue(CurrentVehicle).y);
			}
		}
	}

	bool g_LastEnableTireTracks = false;
	void VehicleFeatures::EnableTireTracks(bool enable)
	{
		CWheelVfx* VFXWheel = Pointers::pVFXWheel.first;
		CWheelVfx* VFXWheelCache = Local::TireTracks::m_Cache;

		if (enable)
		{
			for (std::uint32_t Iterator = 0; Iterator < (std::uint32_t)(Pointers::pVFXWheel.second * 4); Iterator++) {
				// Persistent
				VFXWheel[Iterator].m_type1 = 53;
				VFXWheel[Iterator].m_type2 = 53;
				VFXWheel[Iterator].m_type3 = 64;
				VFXWheel[Iterator].m_type4 = 255;
				VFXWheel[Iterator].m_unk1 = TRUE;
				VFXWheel[Iterator].m_unk2 = TRUE;
				VFXWheel[Iterator].m_unk3 = TRUE;
				VFXWheel[Iterator].m_unk4 = TRUE;
				VFXWheel[Iterator].m_pressure_min = 0.0f;
				VFXWheel[Iterator].m_pressure_max = 0.0f;

				// Color
				const RGB Temp = { m_TireTracksColor[0] * 255.f, m_TireTracksColor[1] * 255.f, m_TireTracksColor[2] * 255.f };
				VFXWheel[Iterator].m_r = Temp.r;
				VFXWheel[Iterator].m_g = Temp.g;
				VFXWheel[Iterator].m_b = Temp.b;
			}

			if (m_TireTracksRainbow)
			{
				m_TireTracksColor[0] = System::g_FastRainbowColor.r / 255.f;
				m_TireTracksColor[1] = System::g_FastRainbowColor.g / 255.f;
				m_TireTracksColor[2] = System::g_FastRainbowColor.b / 255.f;
			}
		}
		else if (enable != g_LastEnableTireTracks)
		{
			for (std::uint32_t Iterator = 0; Iterator < (std::uint32_t)(Pointers::pVFXWheel.second * 4); Iterator++) {
				// Persistent
				VFXWheel[Iterator].m_type1 = VFXWheelCache[Iterator].m_type1;
				VFXWheel[Iterator].m_type2 = VFXWheelCache[Iterator].m_type2;
				VFXWheel[Iterator].m_type3 = VFXWheelCache[Iterator].m_type3;
				VFXWheel[Iterator].m_type4 = VFXWheelCache[Iterator].m_type4;
				VFXWheel[Iterator].m_unk1 = VFXWheelCache[Iterator].m_unk1;
				VFXWheel[Iterator].m_unk2 = VFXWheelCache[Iterator].m_unk2;
				VFXWheel[Iterator].m_unk3 = VFXWheelCache[Iterator].m_unk3;
				VFXWheel[Iterator].m_unk4 = VFXWheelCache[Iterator].m_unk4;
				VFXWheel[Iterator].m_pressure_min = VFXWheelCache[Iterator].m_pressure_min;
				VFXWheel[Iterator].m_pressure_max = VFXWheelCache[Iterator].m_pressure_max;

				// Color
				VFXWheel[Iterator].m_r = VFXWheelCache[Iterator].m_r;
				VFXWheel[Iterator].m_g = VFXWheelCache[Iterator].m_g;
				VFXWheel[Iterator].m_b = VFXWheelCache[Iterator].m_b;
			}
		}

		g_LastEnableTireTracks = enable;
	}
}