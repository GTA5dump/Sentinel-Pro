#include "Weapon.hpp"
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
#include <GTAV-Classes/entities/fwEntity.hpp>
#include <GTAV-Classes/weapon/CWeaponInfo.hpp>

namespace Sentinel
{
	// Usually, I'd do these two features with BytePatches but they're integrity checked now :/
	// This method is a mere fragment of the BytePatch method
	void WeaponFeatures::InfiniteAmmo(bool enable) 
	{
		if (enable)
			WEAPON::SET_PED_INFINITE_AMMO(PLAYER::PLAYER_PED_ID(), TRUE, NULL);
	}

	void WeaponFeatures::InfiniteClip(bool enable)
	{
		if (enable)
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), TRUE);
	}

	void WeaponFeatures::ExplosiveAmmo(bool enable)
	{
		if (enable)
		{
			const auto BulletCoords = Weapons::g_ShapeTestDestination;
			if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()) && !Math::Empty(BulletCoords)) 
				FIRE::ADD_EXPLOSION(BulletCoords.x, BulletCoords.y, BulletCoords.z, eExplosionTag::GRENADE, 9999.f, TRUE, FALSE, 0.f, FALSE);
		}
	}

	void WeaponFeatures::FireAmmo(bool enable)
	{
		if (enable)
		{
			const auto BulletCoords = Weapons::g_ShapeTestDestination;
			if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()) && !Math::Empty(BulletCoords)) 
				FIRE::START_SCRIPT_FIRE(BulletCoords.x, BulletCoords.y, BulletCoords.z, 24, TRUE);
		}
	}

	void WeaponFeatures::NoRecoil(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed())
				return;

			const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
			if (WeaponManager)
			{
				const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
				if (m_PreviousWeaponHash != CurrentWeapon)
				{
					if (!IsRecoilValueCached(CurrentWeapon))
						m_OriginalRecoilValues.push_back({ CurrentWeapon, WeaponManager->m_weapon_info->m_explosion_shake_amplitude });

					WeaponManager->m_weapon_info->m_explosion_shake_amplitude = GetRecoilValue(CurrentWeapon); // m_explosion_shake_amplitude is the right offset in https://github.com/Yimura/GTAV-Classes
				}
			}
		}
	}

	void WeaponFeatures::NoSpread(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed())
				return;

			const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
			if (WeaponManager)
			{
				const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
				if (m_PreviousWeaponHash != CurrentWeapon)
				{
					if (!IsSpreadValueCached(CurrentWeapon))
						m_OriginalSpreadValues.push_back({ CurrentWeapon, WeaponManager->m_weapon_info->m_accuracy_spread });

					WeaponManager->m_weapon_info->m_accuracy_spread = GetSpreadValue(CurrentWeapon);
				}
			}
		}
	}

	void WeaponFeatures::ShotGunMode(bool enable)
	{
		if (enable)
		{
			const auto WeaponInfo = GTA::GetLocalPed()->m_weapon_manager->m_weapon_info;
			WeaponInfo->m_batch_spread = 0.5f;
			WeaponInfo->m_bullets_in_batch = 30;
			WeaponInfo->m_force = 200.0f;
			WeaponInfo->m_speed = 5000.0f;
		}
	}

	void WeaponFeatures::OneShotKill(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed())
				return;

			const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
			if (WeaponManager)
			{
				const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
				if (m_PreviousWeaponHash != CurrentWeapon)
				{
					if (!IsDamageValueCached(CurrentWeapon))
						m_OriginalDamageValues.push_back({ CurrentWeapon, WeaponManager->m_weapon_info->m_damage });

					WeaponManager->m_weapon_info->m_damage = GetDamageValue(CurrentWeapon);
				}
			}
		}
	}

	void WeaponFeatures::NoCooldown(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed())
				return;

			const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
			if (WeaponManager)
			{
				const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
				if (m_PreviousWeaponHash != CurrentWeapon)
				{
					if (!IsDamageValueCached(CurrentWeapon))
						m_OriginalCooldownValues.push_back({ CurrentWeapon, WeaponManager->m_weapon_info->m_time_between_shots });

					WeaponManager->m_weapon_info->m_time_between_shots = GetCooldownValue(CurrentWeapon);
				}
			}
		}
	}

	void WeaponFeatures::NoSpinUp(bool enable)
	{
		if (enable)
		{
			if (!GTA::GetLocalPed())
				return;

			const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
			if (WeaponManager)
			{
				const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
				if (m_PreviousWeaponHash != CurrentWeapon)
				{
					if (!IsSpinUpValueCached(CurrentWeapon))
						m_OriginalSpinUpValues.push_back({ CurrentWeapon, WeaponManager->m_weapon_info->m_spinup_time });

					WeaponManager->m_weapon_info->m_spinup_time = GetOriginalSpinUpValue(CurrentWeapon);
				}
			}
		}
	}

	bool g_LastPortalGun = false;
	int g_FirstPortal = 0;
	NativeVector3 g_FirstPortalCoords;
	int g_SecondPortal = 0;
	NativeVector3 g_SecondPortalCoords;
	bool g_Transition = false;
	void WeaponFeatures::PortalGun(bool enable)
	{
		if (enable)
		{
			const auto Ped = PLAYER::PLAYER_PED_ID();
			const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
			const auto Shooting = Weapons::IsPedShooting(Ped);
			auto BulletCoords = Weapons::g_ShapeTestDestination;

			if (Shooting && g_FirstPortal && g_SecondPortal)
			{
				// Cleanup, so the user can create new portals
				GRAPHICS::DELETE_CHECKPOINT(g_FirstPortal);
				g_FirstPortal = 0;
				GRAPHICS::DELETE_CHECKPOINT(g_SecondPortal);
				g_SecondPortal = 0;
			}

			// Checkpoints are handled by the game, so you may never put those in a tick
			if (Shooting && !Math::Empty(BulletCoords))
			{
				if (!g_FirstPortal && !g_SecondPortal)
				{
					// Create the first portal checkpoint at the given BulletCoords and set its properties
					g_FirstPortal = GRAPHICS::CREATE_CHECKPOINT(47, BulletCoords.x, BulletCoords.y, BulletCoords.z, 0.f, 0.f, 0.f, 2.f, 50, 109, 168, 255, 0);
					GRAPHICS::SET_CHECKPOINT_CYLINDER_HEIGHT(g_FirstPortal, 1.5f, 1.5f, 2.f);
					g_FirstPortalCoords = BulletCoords;
				}
				else if (g_FirstPortal && !g_SecondPortal)
				{
					// Create the second portal checkpoint at the given BulletCoords and set its properties
					g_SecondPortal = GRAPHICS::CREATE_CHECKPOINT(47, BulletCoords.x, BulletCoords.y, BulletCoords.z, 0.f, 0.f, 0.f, 2.f, 240, 95, 50, 255, 0);
					GRAPHICS::SET_CHECKPOINT_CYLINDER_HEIGHT(g_SecondPortal, 1.5f, 1.5f, 2.f);
					g_SecondPortalCoords = BulletCoords;
				}
			}

			// Update
			if (!Math::Empty(g_FirstPortalCoords) && !Math::Empty(g_SecondPortalCoords))
			{
				// Handle teleportation between elements
				if (Math::DistanceBetweenVectors(Coords, g_FirstPortalCoords) <= 1.5f && !g_Transition)
				{
					// First portal to the second 
					ENTITY::SET_ENTITY_COORDS(Ped, g_SecondPortalCoords.x, g_SecondPortalCoords.y, g_SecondPortalCoords.z, FALSE, FALSE, FALSE, FALSE);
					g_Transition = true;
				}


				// Handle teleportation between elements
				if (Math::DistanceBetweenVectors(Coords, g_SecondPortalCoords) <= 1.5f && !g_Transition)
				{
					// Second portal to the first 
					ENTITY::SET_ENTITY_COORDS(Ped, g_FirstPortalCoords.x, g_FirstPortalCoords.y, g_FirstPortalCoords.z, FALSE, FALSE, FALSE, FALSE);
					g_Transition = true;
				}
			}

			// Stop transitioning when out of the element circle
			if ((Math::DistanceBetweenVectors(Coords, g_SecondPortalCoords) >= 1.5f && Math::DistanceBetweenVectors(Coords, g_FirstPortalCoords) >= 1.5f))
				g_Transition = false;
		}
		else if (g_LastPortalGun != enable)
		{
			// Cleanup
			GRAPHICS::DELETE_CHECKPOINT(g_FirstPortal);
			g_FirstPortal = 0;
			GRAPHICS::DELETE_CHECKPOINT(g_SecondPortal);
			g_SecondPortal = 0;
		}

		g_LastPortalGun = enable;
	}

	void WeaponFeatures::TeleportGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			const auto BulletCoord = Weapons::g_ShapeTestDestination;
			if (!Math::Empty(BulletCoord))
				ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), BulletCoord.x, BulletCoord.y, BulletCoord.z, FALSE, TRUE, TRUE, FALSE);
		}
	}

	bool g_FlyGunActivated = false;
	NativeVector3 g_FlyGunCoords;
	void WeaponFeatures::SkydiveGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			auto BulletCoord = Weapons::g_ShapeTestDestination;
			if (!Math::Empty(BulletCoord))
			{
				g_FlyGunCoords = { BulletCoord.x, BulletCoord.y, BulletCoord.z };
				g_FlyGunActivated = true;
			}
		}

		if (g_FlyGunActivated)
		{
			const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			const auto Distance = SYSTEM::VDIST(g_FlyGunCoords.x, g_FlyGunCoords.y, g_FlyGunCoords.z, Coords.x, Coords.y, Coords.z);
			if (Distance < 3.0f)
			{
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				TASK::CLEAR_PED_SECONDARY_TASK(PLAYER::PLAYER_PED_ID());
				g_FlyGunActivated = false;
			}
			else
			{
				NativeVector3 Velocity{};
				Velocity.x = (g_FlyGunCoords.x - Coords.x) * 3.0f;
				Velocity.y = (g_FlyGunCoords.y - Coords.y) * 3.0f;
				Velocity.z = (g_FlyGunCoords.z - Coords.z) * 3.0f;
				TASK::TASK_SKY_DIVE(PLAYER::PLAYER_PED_ID(), NULL);
				ENTITY::SET_ENTITY_VELOCITY(PLAYER::PLAYER_PED_ID(), Velocity.x, Velocity.y, Velocity.z);
			}
		}
	}

	void WeaponFeatures::AirstrikeGun(bool enable)
	{
		if (enable)
		{
			if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
			{
				const auto BulletCoord = Weapons::g_ShapeTestDestination;
				if (!Math::Empty(BulletCoord))
				{
					const auto Airstrike = Joaat("WEAPON_AIRSTRIKE_ROCKET");
					WEAPON::REQUEST_WEAPON_ASSET(Airstrike, 31, FALSE);
					while (!WEAPON::HAS_WEAPON_ASSET_LOADED(Airstrike))
						Script::Current()->Yield(5ms);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(BulletCoord.x, BulletCoord.y, BulletCoord.z + 50.f, BulletCoord.x, BulletCoord.y, BulletCoord.z, 250, 1, Airstrike, PLAYER::PLAYER_PED_ID(), 1, 0, -1.0);
					WEAPON::REMOVE_WEAPON_ASSET(Airstrike);
				}
			}
		}
	}

	void WeaponFeatures::PushGun(bool enable)
	{
		const auto Ped = PLAYER::PLAYER_PED_ID();
		if (enable && Weapons::IsPedShooting(Ped))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (PED::IS_PED_A_PLAYER(Weapons::g_ShapeTestEntity))
					return;

				if (Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity))
					return;

				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());

				const auto MyCoords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
				const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
				const auto Pitch = Math::DegreeToRadian(Rotation.x);
				const auto Yaw = Math::DegreeToRadian(Rotation.z + 90.f);

				const auto Coords = ENTITY::GET_ENTITY_COORDS(Weapons::g_ShapeTestEntity, FALSE);
				NativeVector3 Velocity{};

				Velocity.x = MyCoords.x + (1000 * cos(Pitch) * cos(Yaw)) - Coords.x;
				Velocity.y = MyCoords.y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.y;
				Velocity.z = MyCoords.z + (1000 * sin(Pitch)) - Coords.z;

				ENTITY::SET_ENTITY_VELOCITY(Weapons::g_ShapeTestEntity, Velocity.x, Velocity.y, Velocity.z);
			}
		}
	}

	void WeaponFeatures::PullGun(bool enable)
	{
		const auto Ped = PLAYER::PLAYER_PED_ID();
		if (enable && Weapons::IsPedShooting(Ped))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (PED::IS_PED_A_PLAYER(Weapons::g_ShapeTestEntity))
					return;

				if (Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity))
					return;

				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());

				const auto MyCoords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
				const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
				const auto Pitch = Math::DegreeToRadian(Rotation.x);
				const auto Yaw = Math::DegreeToRadian(Rotation.z + 90.f);

				const auto Coords = ENTITY::GET_ENTITY_COORDS(Weapons::g_ShapeTestEntity, FALSE);
				NativeVector3 Velocity{};

				Velocity.x = MyCoords.x + (1000 * cos(Pitch) * cos(Yaw)) - Coords.x;
				Velocity.y = MyCoords.y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.y;
				Velocity.z = MyCoords.z + (1000 * sin(Pitch)) - Coords.z;

				ENTITY::SET_ENTITY_VELOCITY(Weapons::g_ShapeTestEntity, -(Velocity.x), -(Velocity.y), -(Velocity.z));
			}
		}
	}

	void WeaponFeatures::PedGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			const auto BulletCoord = Weapons::g_ShapeTestDestination;
			if (!Math::Empty(BulletCoord))
				PED::CREATE_RANDOM_PED(BulletCoord.x, BulletCoord.y, BulletCoord.z);
		}
	}

	void WeaponFeatures::RapidFire(bool enable)
	{
		if (enable)
		{
			Hash WeaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &WeaponHash, FALSE);

			if (WeaponHash == 0x42BF8A85 || WeaponHash == 0xB62D1F67)
				return;

			if (PAD::IS_CONTROL_PRESSED(1, (int)eControllerInputs::INPUT_ATTACK))
			{
				const auto Direction = Math::RotationToDirection(CAM::GET_GAMEPLAY_CAM_ROT(0));
				const auto EndCoord = Math::Add(CAM::GET_FINAL_RENDERED_CAM_COORD(), (Math::Multiply(Direction, 100.f)));
				PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), EndCoord.x, EndCoord.y, EndCoord.z, true);
				WEAPON::REFILL_AMMO_INSTANTLY(PLAYER::PLAYER_PED_ID());
			}
		}
	}

	void WeaponFeatures::DeleteGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity) || PED::IS_PED_A_PLAYER(Weapons::g_ShapeTestEntity))
					return;

				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());

				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Weapons::g_ShapeTestEntity, TRUE, TRUE);
				ENTITY::DELETE_ENTITY(&Weapons::g_ShapeTestEntity);
			}
		}
	}

	void WeaponFeatures::HijackGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (!ENTITY::IS_ENTITY_A_VEHICLE(Weapons::g_ShapeTestEntity))
					return;

				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());

				auto Driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Weapons::g_ShapeTestEntity, -1, FALSE);
				if (Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity))
				{
					Pointers::pClearPedTasksNetworked((CPed*)Pointers::pHandleToPointer(Driver), true);
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Weapons::g_ShapeTestEntity, -1);
				}
				else
				{
					Entities::ClearPedTasks(Driver);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Driver, TRUE, TRUE);
					PED::DELETE_PED(&Driver);

					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Weapons::g_ShapeTestEntity, -1);
				}
			}
		}
	}

	Entity g_RopeGunEntities[2];
	int g_RopeGunID;
	void WeaponFeatures::RopeGun(bool enable)
	{
		if (enable) 
		{
			Entity Target = Weapons::g_ShapeTestEntity;
			const auto MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);

			if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
			{
				if (!ENTITY::DOES_ENTITY_EXIST(g_RopeGunEntities[0]) && !ENTITY::DOES_ENTITY_EXIST(g_RopeGunEntities[1]))
				{
					if (ENTITY::DOES_ENTITY_EXIST(Target))
					{
						g_RopeGunEntities[0] = PED::IS_PED_IN_ANY_VEHICLE(Target, TRUE) ? Target = PED::GET_VEHICLE_PED_IS_IN(Target, FALSE) : Target;
						const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(g_RopeGunEntities[0], TRUE);
						const auto Length = Math::DistanceBetweenVectors(MyCoords, EntityCoords);
						g_RopeGunID = PHYSICS::ADD_ROPE(MyCoords.x, MyCoords.y, MyCoords.z, 0.f, 0.f, 0.f, Length, 4, Length, 0.5f, 0.5f, FALSE, FALSE, TRUE, 1.f, FALSE, NULL);
						PHYSICS::ROPE_LOAD_TEXTURES();
						PHYSICS::ACTIVATE_PHYSICS(g_RopeGunID);
					}
				}
				else if (ENTITY::DOES_ENTITY_EXIST(g_RopeGunEntities[0]) && !ENTITY::DOES_ENTITY_EXIST(g_RopeGunEntities[1]))
				{
					if (ENTITY::DOES_ENTITY_EXIST(Target))
					{
						g_RopeGunEntities[1] = PED::IS_PED_IN_ANY_VEHICLE(Target, TRUE) ? Target = PED::GET_VEHICLE_PED_IS_IN(Target, FALSE) : Target;
						const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(g_RopeGunEntities[0], TRUE);

						const auto BulletCoords = Weapons::g_ShapeTestDestination;
						const auto Length = Math::DistanceBetweenVectors(EntityCoords, BulletCoords);
						if (!Math::Empty(BulletCoords))
						{
							PHYSICS::ATTACH_ENTITIES_TO_ROPE(g_RopeGunID, g_RopeGunEntities[0], g_RopeGunEntities[1], EntityCoords.x, EntityCoords.y, EntityCoords.z, BulletCoords.x, BulletCoords.y, BulletCoords.z, Length, FALSE, FALSE, NULL, NULL);
							PHYSICS::PIN_ROPE_VERTEX(g_RopeGunID, PHYSICS::GET_ROPE_VERTEX_COUNT(g_RopeGunID) - 1, BulletCoords.x, BulletCoords.y, BulletCoords.z);
							g_RopeGunEntities[0] = NULL;
							g_RopeGunEntities[1] = NULL;
						}
					}
				}
			}

			if (ENTITY::DOES_ENTITY_EXIST(g_RopeGunEntities[0]) && !ENTITY::DOES_ENTITY_EXIST(g_RopeGunEntities[1]))
			{
				const auto RightHand = PED::GET_PED_BONE_COORDS(PLAYER::PLAYER_PED_ID(), 6286, 0.f, 0.f, 0.f);
				const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(g_RopeGunEntities[0], TRUE);
				PHYSICS::PIN_ROPE_VERTEX(g_RopeGunID, 0, RightHand.x, RightHand.y, RightHand.z);
				PHYSICS::PIN_ROPE_VERTEX(g_RopeGunID, PHYSICS::GET_ROPE_VERTEX_COUNT(g_RopeGunID) - 1, EntityCoords.x, EntityCoords.y, EntityCoords.z);
			}
		}
	}

	// This code looks familiar to you right?
	// That's because I was the one who first had it, then my source got leaked
	// Now every skidded menu has it, but fortunately the code was very shit
	// I've adapted it a bit to my own personal preference
	bool g_LastValkyrieGun = false;
	void WeaponFeatures::ValkyrieGun(bool enable)
	{
		static std::int32_t Rocket, Cam;
		static std::uint8_t YPos;
		static float Meter;
		static bool Initialized;
		const auto Ped = PLAYER::PLAYER_PED_ID();

		if (enable)
		{
			if (Weapons::IsPedShooting(Ped))
			{
				if (!Initialized)
					Initialized = true;
			}

			if (Initialized)
			{
				if (ENTITY::DOES_ENTITY_EXIST(Rocket))
				{
					const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
					CAM::SET_CAM_ROT(Cam, Rotation.x, Rotation.y, Rotation.z, 0);
					ENTITY::SET_ENTITY_ROTATION(Rocket, Rotation.x, Rotation.y, Rotation.z, 0, TRUE);

					const auto RocketPos = ENTITY::GET_ENTITY_COORDS(Rocket, FALSE);
					const auto Coords = Math::Add(RocketPos, Math::Multiply(Math::RotationToDirection(Rotation), .8f));
					ENTITY::SET_ENTITY_COORDS(Rocket, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);

					HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
					PLAYER::DISABLE_PLAYER_FIRING(Ped, TRUE);
					ENTITY::FREEZE_ENTITY_POSITION(Ped, TRUE);
					HUD::HUD_SUPPRESS_WEAPON_WHEEL_RESULTS_THIS_FRAME();

					GRAPHICS::DRAW_RECT(0.5f, 0.5f - 0.025f, 0.050f, 0.002f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f, 0.5f + 0.025f, 0.050f, 0.002f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f - 0.025f, 0.5f, 0.002f, 0.050f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f + 0.025f, 0.5f, 0.002f, 0.050f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f + 0.05f, 0.5f, 0.050f, 0.002f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f - 0.05f, 0.5f, 0.050f, 0.002f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f, 0.5f + 0.05f, 0.002f, 0.050f, 255, 255, 255, 255, FALSE);
					GRAPHICS::DRAW_RECT(0.5f, 0.5f - 0.05f, 0.002f, 0.050f, 255, 255, 255, 255, FALSE);

					GRAPHICS::DRAW_RECT(0.25f, 0.5f, 0.01f, 0.5f, 255, 255, 255, 150, FALSE);
					static auto Ticker = GetTickCount();
					if (GetTickCount() - Ticker >= 250)
					{
						Meter -= .01f; YPos -= 4;
						Ticker = GetTickCount();
					}
					GRAPHICS::DRAW_RECT(0.25f, 0.75f - (Meter / 2.f), 0.01f, Meter, 255, YPos, 0, 255, FALSE);

					float GroundZ;
					MISC::GET_GROUND_Z_FOR_3D_COORD(RocketPos.x, RocketPos.y, RocketPos.z, &GroundZ, FALSE, FALSE);
					if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(Rocket) ||
						(std::abs(RocketPos.z - GroundZ) < .5f) ||
						Meter <= 0.01)
					{
						FIRE::ADD_EXPLOSION(RocketPos.x, RocketPos.y, RocketPos.z, eExplosionTag::EXP_TAG_VALKYRIE_CANNON, 1000.f, TRUE, FALSE, .4f, FALSE);
						Entities::DeleteEntity(Rocket);
						Rocket = 0;
						PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 0);
						CAM::RENDER_SCRIPT_CAMS(FALSE, TRUE, 700, TRUE, TRUE, NULL);
						CAM::DESTROY_CAM(Cam, TRUE);
						ENTITY::FREEZE_ENTITY_POSITION(Ped, FALSE);
						Initialized = false;
					}
				}
				else
				{
					const auto Weapon = Weapons::GetCurrent(PLAYER::PLAYER_PED_ID());
					const auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Weapon, 0.f, 1.f, 0.f);
					System::ModelSpawnBypass(true);
					Rocket = OBJECT::CREATE_OBJECT(Joaat("w_lr_rpg_rocket"), Coords.x, Coords.y, Coords.z, TRUE, TRUE, FALSE);
					System::ModelSpawnBypass(false);
					CAM::DESTROY_ALL_CAMS(TRUE);
					Cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", TRUE);
					CAM::ATTACH_CAM_TO_ENTITY(Cam, Rocket, 0.f, 0.f, 0.f, TRUE);
					CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 700, TRUE, TRUE, NULL);
					CAM::SET_CAM_ACTIVE(Cam, TRUE);
					ENTITY::SET_ENTITY_VISIBLE(Rocket, FALSE, FALSE);
					YPos = 255; Meter = .5f;
				}
			}
		}
		else if (enable != g_LastValkyrieGun)
		{
			CAM::DESTROY_CAM(Cam, TRUE);
			PLAYER::DISABLE_PLAYER_FIRING(Ped, FALSE);
			Rocket = 0;
			YPos = 255;
			Meter = .5f;
			ENTITY::FREEZE_ENTITY_POSITION(Ped, FALSE);
		}

		g_LastValkyrieGun = enable;
	}

	void WeaponFeatures::DeadEyes(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()))
			{
				if (PAD::IS_CONTROL_PRESSED(0, INPUT_AIM))
				{
					MISC::SET_TIME_SCALE(0.2f);
					GRAPHICS::ANIMPOSTFX_PLAY("HeistLocate", 0, FALSE);
				}
				else if (PAD::IS_CONTROL_RELEASED(0, INPUT_AIM))
				{
					MISC::SET_TIME_SCALE(1.f);
					GRAPHICS::ANIMPOSTFX_STOP("HeistLocate");
				}
			}
		}
	}


	// GPS Satellites
	// Unmanned Drones
	// FOOKIN' LASER SIGHTS
	void WeaponFeatures::LaserSight(bool enable)
	{
		if (enable && Weapons::IsPedAiming(PLAYER::PLAYER_PED_ID()))
		{
			const auto Start = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(PLAYER::PLAYER_PED_ID(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 0x6F06));
			const NativeVector3 Updated = { 0.0f, 200.0f, (MISC::TAN(CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH()) * 200.0f) };
			const auto End = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), Updated.x, Updated.y, Updated.z);
			GRAPHICS::DRAW_LINE(Start.x, Start.y, Start.z, End.x, End.y, End.z, 99, 66, 245, 255);
		}
	}

	void WeaponFeatures::DefibrillatorGun(bool enable)
	{
		if (enable)
		{
			Entity Ped;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &Ped))
			{
				if (ENTITY::IS_ENTITY_A_PED(Ped) && ENTITY::IS_ENTITY_DEAD(Ped, FALSE)) {
					PED::RESURRECT_PED(Ped);
					PED::REVIVE_INJURED_PED(Ped);
					ENTITY::SET_ENTITY_HEALTH(Ped, 100, 0);
					Entities::ClearPedTasks(Ped);
				}
			}
		}
	}

	void WeaponFeatures::HashGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (!ENTITY::IS_ENTITY_AN_OBJECT(Weapons::g_ShapeTestEntity) && !ENTITY::IS_ENTITY_A_PED(Weapons::g_ShapeTestEntity) && !ENTITY::IS_ENTITY_A_VEHICLE(Weapons::g_ShapeTestEntity))
					return;

				std::uint32_t Hash = ENTITY::GET_ENTITY_MODEL(Weapons::g_ShapeTestEntity);
				Overlays::PushInfo("Hash Gun", std::format("Entity Hash: 0x{:X} ({})", Hash, Hash), 5000);
				LOG(INFO) << "[Hash Gun] " << std::format("Entity Hash: 0x{:X} ({})", Hash, Hash);
			}
		}
	}

	void WeaponFeatures::ShrinkGun(bool enable)
	{
		if (enable)
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (ENTITY::IS_ENTITY_A_PED(Weapons::g_ShapeTestEntity)) {
					PED::SET_PED_CONFIG_FLAG(Weapons::g_ShapeTestEntity, 223, TRUE);
				}
			}
		}
	}

	void WeaponFeatures::ClownGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_rcbarry2"))
				Script::Current()->Yield(5ms);
			NativeVector3 v0, v1;
			MISC::GET_MODEL_DIMENSIONS(WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), &v0, &v1);
			GRAPHICS::USE_PARTICLE_FX_ASSET("scr_rcbarry2");
			GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_clown", Weapons::GetCurrent(PLAYER::PLAYER_PED_ID()), (v0.x - v1.x) / 2.0f + 0.7f, 0.f, 0.f, 0.f, 180.f, 0.f, 1.f, 1, 1, 1);
		}
	}

	void WeaponFeatures::RepairGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (!ENTITY::IS_ENTITY_A_VEHICLE(Weapons::g_ShapeTestEntity) || Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity))
					return;

				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());

				VEHICLE::SET_VEHICLE_FIXED(Weapons::g_ShapeTestEntity);
				VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Weapons::g_ShapeTestEntity);
				VEHICLE::SET_VEHICLE_DIRT_LEVEL(Weapons::g_ShapeTestEntity, 0.f);
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Weapons::g_ShapeTestEntity, "BALLER");
			}
		}
	}

	void WeaponFeatures::RecolorGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				if (!ENTITY::IS_ENTITY_A_VEHICLE(Weapons::g_ShapeTestEntity) || Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity))
					return;

				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());

				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Weapons::g_ShapeTestEntity, Math::RandomNumberInRange(0, 255), Math::RandomNumberInRange(0, 255), Math::RandomNumberInRange(0, 255));
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Weapons::g_ShapeTestEntity, Math::RandomNumberInRange(0, 255), Math::RandomNumberInRange(0, 255), Math::RandomNumberInRange(0, 255));
			}
		}
	}

	static Entity g_InstrumentOfDeathHandle;
	void WeaponFeatures::InstrumentOfDeath(bool enable)
	{
		if (enable && GTA::GetLocalPed()->m_weapon_manager->m_selected_weapon_hash == Joaat("WEAPON_UNARMED"))
		{
			const auto SpawnCoords = Math::CoordsInFrontOfCam(10);
			const auto TraceCoords = Math::CoordsInFrontOfCam(50);
			const auto BulletCoords = ENTITY::GET_ENTITY_COORDS(g_InstrumentOfDeathHandle, FALSE);

			PAD::DISABLE_CONTROL_ACTION(0, INPUT_AIM, TRUE);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_ATTACK, TRUE);

			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_AIM))
			{
				const auto Hash = Joaat("w_lr_homing_rocket");
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_ATTACK) && !ENTITY::DOES_ENTITY_EXIST(g_InstrumentOfDeathHandle))
				{
					while (!STREAMING::HAS_MODEL_LOADED(Hash))
					{
						STREAMING::REQUEST_MODEL(Hash);
						Script::Current()->Yield(5ms);
					}
					if (!g_InstrumentOfDeathHandle) {
						System::ModelSpawnBypass(true);
						g_InstrumentOfDeathHandle = OBJECT::CREATE_OBJECT(Hash, SpawnCoords.x, SpawnCoords.y, SpawnCoords.z, TRUE, FALSE, FALSE);
						System::ModelSpawnBypass(false);
					}
				}

				if (ENTITY::DOES_ENTITY_EXIST(g_InstrumentOfDeathHandle))
				{
					const auto NewTrace = Math::Subtract(TraceCoords, BulletCoords);
					ENTITY::APPLY_FORCE_TO_ENTITY(g_InstrumentOfDeathHandle, 3, NewTrace.x * 0.5f, NewTrace.y * 0.5f, NewTrace.z * 0.5f, 0.f, 0.f, 0.f, 0, FALSE, TRUE, TRUE, FALSE, TRUE);
					Local::ParticleOnBone("scr_agencyheistb", "scr_agency3b_heli_expl", (ePedBones)0, 0.1f, false, 0.f, 0.f, 0.f, false, g_InstrumentOfDeathHandle);

					if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(g_InstrumentOfDeathHandle))
					{
						FIRE::ADD_EXPLOSION(BulletCoords.x, BulletCoords.y, BulletCoords.z, (int)eExplosionTag::EXP_TAG_SUBMARINE_BIG, 9999.f, TRUE, FALSE, 10.f, FALSE);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(g_InstrumentOfDeathHandle, TRUE, TRUE);
						ENTITY::DELETE_ENTITY(&g_InstrumentOfDeathHandle);
						g_InstrumentOfDeathHandle = 0;
					}
				}
				else
					g_InstrumentOfDeathHandle = 0;
			}
			else if (!PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_AIM) && ENTITY::DOES_ENTITY_EXIST(g_InstrumentOfDeathHandle))
			{
				Local::ParticleOnBone("scr_agencyheistb", "scr_agency3b_heli_expl", ePedBones::SKEL_ROOT, 0.1f, false, 0.f, 0.f, 0.f, false, g_InstrumentOfDeathHandle);

				if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(g_InstrumentOfDeathHandle))
				{
					FIRE::ADD_EXPLOSION(BulletCoords.x, BulletCoords.y, BulletCoords.z, (int)eExplosionTag::EXP_TAG_SUBMARINE_BIG, 9999.f, TRUE, FALSE, 10.f, FALSE);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(g_InstrumentOfDeathHandle, TRUE, TRUE);
					ENTITY::DELETE_ENTITY(&g_InstrumentOfDeathHandle);
					g_InstrumentOfDeathHandle = 0;
				}
			}
		}
		else
			g_InstrumentOfDeathHandle = 0;
	}

	void WeaponFeatures::VortexGunEnable(bool enable)
	{
		// Nesting is a great thing bro
		if (enable) 
		{
			if (Weapons::IsPedAiming(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()))
			{
				if (!m_VortexGunCooldown)
				{
					const auto Direction = Math::GetGameplayCameraDirection();
					const auto VortexPosition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.4f, 0.f, 0.6f);
					const auto ReplayInterface = *Pointers::pReplayInterface;

					if (m_VortexGunSphere)
						GRAPHICS::DRAW_MARKER_SPHERE((VortexPosition.x + (Direction.x * m_VortexGunDistance)), (VortexPosition.y + (Direction.y * m_VortexGunDistance)), (VortexPosition.z + (Direction.z * m_VortexGunDistance)), 2.5f, 99, 66, 245, 100 / 255.f);

					// Move them
					if (m_VortexGunDistance > 100.f)
						m_VortexGunDistance = 100.f;

					if (m_VortexGunDistance < 10.f)
						m_VortexGunDistance = 10.f;

					PAD::DISABLE_CONTROL_ACTION(0, INPUT_CURSOR_SCROLL_UP, TRUE);
					PAD::DISABLE_CONTROL_ACTION(0, INPUT_CURSOR_SCROLL_DOWN, TRUE);
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CURSOR_SCROLL_UP)) 
						m_VortexGunDistance += 5.f;
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CURSOR_SCROLL_DOWN))
						m_VortexGunDistance -= 5.f;

					if (m_VortexGunPeds)
					{
						std::uint32_t FoundPeds = NULL;
						const auto PedInteface = ReplayInterface->m_ped_interface;
						for (int i = 0; i < PedInteface->m_max_peds; i++)
						{
							const auto PedPointer = PedInteface->get_ped(i);
							if (!PedPointer)
								continue;

							// Make sure we don't include ourselves
							if (PedPointer == GTA::GetLocalPed())
								continue;

							const auto Ped = Pointers::pPointerToHandle(PedPointer);
							if (!Ped)
								break;

							if (!ENTITY::DOES_ENTITY_EXIST(Ped))
								continue;

							if (PED::IS_PED_A_PLAYER(Ped))
								continue;

							if (FoundPeds++ == m_VortexGunPedCapacity)
								break;

							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Ped);
							if (*Pointers::pIsSessionStarted)
								Entities::ChangeNetworkObjectOwner(Ped, g_PlayerService->GetSelf()->GetNetPlayer());

							if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Ped)) {
								const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
								ENTITY::SET_ENTITY_VELOCITY(Ped,
									((VortexPosition.x + (Direction.x * m_VortexGunDistance)) - EntityCoords.x) * 4.f,
									((VortexPosition.y + (Direction.y * m_VortexGunDistance)) - EntityCoords.y) * 4.f,
									((VortexPosition.z + (Direction.z * m_VortexGunDistance)) - EntityCoords.z) * 4.f);

								if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID())) {
									const auto ForcePos = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(Ped,
										(VortexPosition.x + (Direction.x * m_VortexGunForce)),
										(VortexPosition.y + (Direction.y * m_VortexGunForce)),
										(VortexPosition.z + (Direction.z * m_VortexGunForce)));

									ENTITY::APPLY_FORCE_TO_ENTITY(Ped, 1, ForcePos.x, ForcePos.y, ForcePos.z, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE);
									m_VortexGunCooldown = true;
								}
							}
						}
					}

					if (m_VortexGunVehicles)
					{
						std::uint32_t FoundVehicles = NULL;
						const auto VehicleInterface = ReplayInterface->m_vehicle_interface;
						for (int i = 0; i < VehicleInterface->m_max_vehicles; i++)
						{
							const auto VehiclePointer = VehicleInterface->get_vehicle(i);
							if (!VehiclePointer)
								continue;

							// Make sure we don't include ourselves
							if (VehiclePointer == GTA::GetLocalPed()->m_vehicle)
								continue;

							const auto Vehicle = Pointers::pPointerToHandle(VehiclePointer);
							if (!Vehicle)
								break;

							if (!ENTITY::DOES_ENTITY_EXIST(Vehicle))
								continue;

							if (!ENTITY::IS_ENTITY_A_VEHICLE(Vehicle))
								continue;

							if (Target::IsPlayerVehicle(Vehicle))
								continue;

							if (FoundVehicles++ == m_VortexGunVehicleCapacity)
								break;

							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Vehicle);
							if (*Pointers::pIsSessionStarted)
								Entities::ChangeNetworkObjectOwner(Vehicle, g_PlayerService->GetSelf()->GetNetPlayer());
							if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Vehicle)) {
								const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(Vehicle, TRUE);
								ENTITY::SET_ENTITY_VELOCITY(Vehicle,
									((VortexPosition.x + (Direction.x * m_VortexGunDistance)) - EntityCoords.x) * 4.f,
									((VortexPosition.y + (Direction.y * m_VortexGunDistance)) - EntityCoords.y) * 4.f,
									((VortexPosition.z + (Direction.z * m_VortexGunDistance)) - EntityCoords.z) * 4.f);

								if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID())) {
									const auto ForcePos = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(Vehicle,
										(VortexPosition.x + (Direction.x * m_VortexGunForce)),
										(VortexPosition.y + (Direction.y * m_VortexGunForce)),
										(VortexPosition.z + (Direction.z * m_VortexGunForce)));

									ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, 1, ForcePos.x, ForcePos.y, ForcePos.z, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE);
									m_VortexGunCooldown = true;
								}
							}
						}
					}

					if (m_VortexGunObjects)
					{
						std::uint32_t FoundObjects = NULL;
						const auto ObjectInterface = ReplayInterface->m_object_interface;
						for (int i = 0; i < ObjectInterface->m_max_objects; i++)
						{
							const auto ObjectPointer = ObjectInterface->get_object(i);
							if (!ObjectPointer)
								continue;

							const auto Object = Pointers::pPointerToHandle(ObjectPointer);
							if (!Object)
								break;

							if (!ENTITY::DOES_ENTITY_EXIST(Object))
								continue;

							if (!ENTITY::IS_ENTITY_AN_OBJECT(Object))
								continue;

							if (Object == PLAYER::PLAYER_PED_ID())
								continue;

							if (FoundObjects++ == m_VortexGunObjectCapacity)
								break;

							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Object);
							if (*Pointers::pIsSessionStarted)
								Entities::ChangeNetworkObjectOwner(Object, g_PlayerService->GetSelf()->GetNetPlayer());
							if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Object)) {
								const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(Object, TRUE);
								ENTITY::SET_ENTITY_VELOCITY(Object,
									((VortexPosition.x + (Direction.x * m_VortexGunDistance)) - EntityCoords.x) * 4.f,
									((VortexPosition.y + (Direction.y * m_VortexGunDistance)) - EntityCoords.y) * 4.f,
									((VortexPosition.z + (Direction.z * m_VortexGunDistance)) - EntityCoords.z) * 4.f);

								if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID())) {
									const auto ForcePos = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(Object,
										(VortexPosition.x + (Direction.x * m_VortexGunForce)),
										(VortexPosition.y + (Direction.y * m_VortexGunForce)),
										(VortexPosition.z + (Direction.z * m_VortexGunForce)));

									ENTITY::APPLY_FORCE_TO_ENTITY(Object, 1, ForcePos.x, ForcePos.y, ForcePos.z, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE);
									m_VortexGunCooldown = true;
								}
							}
						}
					}
				}
			}
			else
				m_VortexGunCooldown = false;
		}
	}

	void WeaponFeatures::EnableTotemGun(bool enable)
	{
		if (enable)
		{
			if (Weapons::IsPedAiming(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()))
			{
				const auto VehicleDirection = Math::GetGameplayCameraDirection();
				const auto VehiclePosition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Weapons::GetCurrent(PLAYER::PLAYER_PED_ID()), 5.0f, 0.0f, 0.0f);

				if (m_TotemGunDistance > 100.f)
					m_TotemGunDistance = 100.f;

				if (m_TotemGunDistance < 1.f)
					m_TotemGunDistance = 1.f;

				PAD::DISABLE_CONTROL_ACTION(0, INPUT_CURSOR_SCROLL_UP, TRUE);
				PAD::DISABLE_CONTROL_ACTION(0, INPUT_CURSOR_SCROLL_DOWN, TRUE);
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CURSOR_SCROLL_UP)) 
					m_TotemGunDistance += 5.f;
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CURSOR_SCROLL_DOWN))
					m_TotemGunDistance -= 5.f;

				std::uint32_t FoundVehicles = NULL;
				const auto ReplayInterface = *Pointers::pReplayInterface;
				const auto VehicleInterface = ReplayInterface->m_vehicle_interface;
				for (int i = 0; i < VehicleInterface->m_max_vehicles; i++)
				{
					const auto VehiclePointer = VehicleInterface->get_vehicle(i);
					if (!VehiclePointer)
						continue;

					// Make sure we don't include ourselves
					if (VehiclePointer == GTA::GetLocalPed()->m_vehicle)
						continue;

					const auto Vehicle = Pointers::pPointerToHandle(VehiclePointer);
					if (!Vehicle)
						break;

					if (!ENTITY::DOES_ENTITY_EXIST(Vehicle))
						continue;

					if (!ENTITY::IS_ENTITY_A_VEHICLE(Vehicle))
						continue;

					if (Target::IsPlayerVehicle(Vehicle))
						continue;

					NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Vehicle);
					if (*Pointers::pIsSessionStarted)
						Entities::ChangeNetworkObjectOwner(Vehicle, g_PlayerService->GetSelf()->GetNetPlayer());
					if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Vehicle)) {
						ENTITY::SET_ENTITY_COORDS(Vehicle, VehiclePosition.x + (VehicleDirection.x * m_TotemGunDistance), VehiclePosition.y + (VehicleDirection.y * m_TotemGunDistance), VehiclePosition.z + (VehicleDirection.z * m_TotemGunDistance) + ((i - 1) * m_TotemGunSpacing), TRUE, TRUE, TRUE, FALSE);
					}
				}
			}
		}
	}

	struct BulletTracer
	{
		NativeVector3 m_Start{};
		NativeVector3 m_End{};
		RGB m_Color{};
		DWORD m_Time{};
		int m_Alpha{};
	}; std::vector<BulletTracer> m_BulletTracers;
	void WeaponFeatures::EnableBulletTracers(bool enable)
	{
		if (enable)
		{
			if (m_RainbowBulletTracers)
			{
				m_BulletTracerColor[0] = System::g_SlowRainbowColor.r / 255.f;
				m_BulletTracerColor[1] = System::g_SlowRainbowColor.g / 255.f;
				m_BulletTracerColor[2] = System::g_SlowRainbowColor.b / 255.f;
			}

			if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID())) 
			{
				RGB BulletTracerColor =
				{
					m_BulletTracerColor[0] * 255.f,
					m_BulletTracerColor[1] * 255.f,
					m_BulletTracerColor[2] * 255.f
				};

				const auto Direction = Math::RotationToDirection(CAM::GET_GAMEPLAY_CAM_ROT(0));
				const auto TracerStart = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(PLAYER::PLAYER_PED_ID(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 0x6F06));
				const auto TracerEnd = Math::Add(CAM::GET_FINAL_RENDERED_CAM_COORD(), (Math::Multiply(Direction, 100.f)));

				m_BulletTracers.push_back({ TracerStart, TracerEnd, BulletTracerColor, GetTickCount(), 0 });
			}

			if (m_BulletTracers.empty())
				return;

			for (auto& Tracer : m_BulletTracers) 
			{
				if (GetTickCount() - Tracer.m_Time < m_BulletTracerDuration) {
					if (Tracer.m_Alpha != 255)
						Tracer.m_Alpha += 5;

					GRAPHICS::DRAW_LINE(Tracer.m_Start.x, Tracer.m_Start.y, Tracer.m_Start.z, Tracer.m_End.x, Tracer.m_End.y, Tracer.m_End.z, Tracer.m_Color.r, Tracer.m_Color.g, Tracer.m_Color.b, Tracer.m_Alpha);
				}
				if (GetTickCount() - Tracer.m_Time > m_BulletTracerDuration - 100) {
					if (Tracer.m_Alpha != 0)
						Tracer.m_Alpha -= 5;

					GRAPHICS::DRAW_LINE(Tracer.m_Start.x, Tracer.m_Start.y, Tracer.m_Start.z, Tracer.m_End.x, Tracer.m_End.y, Tracer.m_End.z, Tracer.m_Color.r, Tracer.m_Color.g, Tracer.m_Color.b, Tracer.m_Alpha);
				}

				if (GetTickCount() - Tracer.m_Time > m_BulletTracerDuration + 1000) {
					m_BulletTracers.erase(m_BulletTracers.begin());
				}
			}
		}
	}

	Entity g_TargetEntity;
	NativeVector3 g_Rotation = {};
	void WeaponFeatures::EnableGravityGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()) && !ENTITY::DOES_ENTITY_EXIST(g_TargetEntity))
		{
			if (ENTITY::DOES_ENTITY_EXIST(Weapons::g_ShapeTestEntity))
			{
				// Safety checks
				if (!ENTITY::IS_ENTITY_AN_OBJECT(Weapons::g_ShapeTestEntity) && !ENTITY::IS_ENTITY_A_PED(Weapons::g_ShapeTestEntity) && !ENTITY::IS_ENTITY_A_VEHICLE(Weapons::g_ShapeTestEntity) || Target::IsPlayerVehicle(Weapons::g_ShapeTestEntity) || PED::IS_PED_A_PLAYER(Weapons::g_ShapeTestEntity)) {
					g_TargetEntity = 0;
					return;
				}

				g_TargetEntity = Weapons::g_ShapeTestEntity;
				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Weapons::g_ShapeTestEntity, g_PlayerService->GetSelf()->GetNetPlayer());
				g_Rotation = ENTITY::GET_ENTITY_ROTATION(Weapons::g_ShapeTestEntity, 2);
				ENTITY::SET_ENTITY_COLLISION(Weapons::g_ShapeTestEntity, FALSE, FALSE);
			}
		}
		else if (enable && ENTITY::DOES_ENTITY_EXIST(g_TargetEntity))
		{
			if (!Weapons::IsPedAiming(PLAYER::PLAYER_PED_ID()))
				g_TargetEntity = 0;

			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(g_TargetEntity);
			if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(g_TargetEntity)) {
				const auto Direction = Math::GetGameplayCameraDirection();
				const auto GravityPosition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.4f, 0.f, 0.6f);
				const auto EntityCoords = ENTITY::GET_ENTITY_COORDS(g_TargetEntity, TRUE);
				ENTITY::SET_ENTITY_VELOCITY(g_TargetEntity,
					((GravityPosition.x + (Direction.x * m_GravityGunDistance)) - EntityCoords.x) * 4.f,
					((GravityPosition.y + (Direction.y * m_GravityGunDistance)) - EntityCoords.y) * 4.f,
					((GravityPosition.z + (Direction.z * m_GravityGunDistance)) - EntityCoords.z) * 4.f);
				ENTITY::SET_ENTITY_ROTATION(g_TargetEntity, g_Rotation.x, g_Rotation.y, g_Rotation.z, 2, FALSE);

				if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID())) {
					const auto ForcePos = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(g_TargetEntity,
						(GravityPosition.x + (Direction.x * m_GravityGunForce)),
						(GravityPosition.y + (Direction.y * m_GravityGunForce)),
						(GravityPosition.z + (Direction.z * m_GravityGunForce)));

					ENTITY::APPLY_FORCE_TO_ENTITY(g_TargetEntity, 1, ForcePos.x, ForcePos.y, ForcePos.z, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE);
					ENTITY::SET_ENTITY_COLLISION(g_TargetEntity, TRUE, TRUE);
					g_TargetEntity = 0;
				}
			}
		}
	}

	void WeaponFeatures::EnableBulletChanger(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID())) {
			Hash WeaponHash = Lists::g_BulletChangerHashes[m_BulletChangerTypePos];

			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(WeaponHash))
			{
				WEAPON::REQUEST_WEAPON_ASSET(WeaponHash, 31, 0);
				Script::Current()->Yield(5ms);
			}

			const auto AimCoords = Math::CoordsInFrontOfCam(500.f);
			const auto WeaponCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Weapons::GetCurrent(PLAYER::PLAYER_PED_ID()), 1.5f, 0.f, 0.f);
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(WeaponCoords.x, WeaponCoords.y, WeaponCoords.z, AimCoords.x, AimCoords.y, AimCoords.z, 250, FALSE, WeaponHash, PLAYER::PLAYER_PED_ID(), m_BulletChangerAudible, !m_BulletChangerVisible, m_BulletChangerSpeed);
			WEAPON::REMOVE_WEAPON_ASSET(WeaponHash);
		}
	}

	void WeaponFeatures::EnableImpactGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			const auto BulletCoord = Weapons::g_ShapeTestDestination;
			if (!Math::Empty(BulletCoord))
				FIRE::ADD_EXPLOSION(BulletCoord.x, BulletCoord.y, BulletCoord.z, m_ImpactGunTypePos, m_ImpactGunDamageScale, m_ImpactGunAudible, !m_ImpactGunVisible, m_ImpactGunCameraShake, FALSE);
		}
	}

	struct PaintGun
	{
		NativeVector3 m_Coords{};
		RGB m_Color{};
		DWORD m_Time{};
		float m_Intensity{};
	}; std::vector<PaintGun> m_PaintGunVector;
	void WeaponFeatures::EnablePaintGun(bool enable)
	{
		if (enable) 
		{
			if (m_RainbowPaintGun)
			{
				m_PaintGunColor[0] = System::g_FastRainbowColor.r / 255.f;
				m_PaintGunColor[1] = System::g_FastRainbowColor.g / 255.f;
				m_PaintGunColor[2] = System::g_FastRainbowColor.b / 255.f;
			}

			if (Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
			{
				const auto BulletCoord = Weapons::g_ShapeTestDestination;
				if (!Math::Empty(BulletCoord))
				{
					RGB PaintGunColor =
					{
						m_PaintGunColor[0] * 255.f,
						m_PaintGunColor[1] * 255.f,
						m_PaintGunColor[2] * 255.f
					};

					m_PaintGunVector.push_back({ BulletCoord, PaintGunColor, GetTickCount(), 0.f });
				}
			}

			if (m_PaintGunVector.empty())
				return;

			for (auto& Paint : m_PaintGunVector)
			{
				if (GetTickCount() - Paint.m_Time < m_PaintGunDuration) {
					if (Paint.m_Intensity != m_PaintGunIntensity)
						Paint.m_Intensity += 20;

					GRAPHICS::DRAW_LIGHT_WITH_RANGE(Paint.m_Coords.x, Paint.m_Coords.y, Paint.m_Coords.z, Paint.m_Color.r, Paint.m_Color.g, Paint.m_Color.b, m_PaintGunSize, Paint.m_Intensity);
				}
				if (GetTickCount() - Paint.m_Time > m_PaintGunDuration - 100) {
					if (Paint.m_Intensity != 0.f)
						Paint.m_Intensity -= 20;

					GRAPHICS::DRAW_LIGHT_WITH_RANGE(Paint.m_Coords.x, Paint.m_Coords.y, Paint.m_Coords.z, Paint.m_Color.r, Paint.m_Color.g, Paint.m_Color.b, m_PaintGunSize, Paint.m_Intensity);
				}

				if (GetTickCount() - Paint.m_Time > m_PaintGunDuration + 1500) {
					m_PaintGunVector.erase(m_PaintGunVector.begin());
				}
			}
		}
	}

	void WeaponFeatures::EnableParticleGun(bool enable)
	{
		if (enable && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			const auto BulletCoord = Weapons::g_ShapeTestDestination;
			if (!Math::Empty(BulletCoord))
			{
				const char* Dictionary{};
				const char* Effect{};

				switch (m_ParticleGunEffectPos)
				{
				case 0:
					Dictionary = "scr_rcbarry2";
					Effect = "scr_clown_appears";
					break;
				case 1:
					Dictionary = "scr_rcbarry2";
					Effect = "scr_clown_death";
					break;
				case 2:
					Dictionary = "scr_rcbarry2";
					Effect = "scr_exp_clown";
					break;
				case 3:
					Dictionary = "scr_indep_fireworks";
					Effect = "scr_indep_firework_starburst";
					break;
				case 4:
					Dictionary = "scr_indep_fireworks";
					Effect = "scr_indep_firework_fountain";
					break;
				case 5:
					Dictionary = "scr_indep_fireworks";
					Effect = "scr_indep_firework_shotburst";
					break;
				case 6:
					Dictionary = "proj_xmas_firework";
					Effect = "scr_firework_xmas_spiral_burst_rgw";
					break;
				case 7:
					Dictionary = "scr_rcbarry1";
					Effect = "scr_alien_teleport";
					break;
				case 8:
					Dictionary = "scr_rcbarry1";
					Effect = "scr_alien_disintegrate";
					break;
				case 9:
					Dictionary = "scr_agencyheist";
					Effect = "scr_fbi_mop_drips";
					break;
				case 10:
					Dictionary = "scr_rcbarry2";
					Effect = "scr_clown_bul";
					break;
				case 11:
					Dictionary = "scr_rcpaparazzo1";
					Effect = "scr_rcpap1_camera";
					break;
				case 12:
					Dictionary = "scr_rcbarry1";
					Effect = "scr_alien_impact_bul";
					break;
				case 13:
					Dictionary = "scr_recartheft";
					Effect = "scr_wheel_burnout";
					break;
				case 14:
					Dictionary = "scr_trevor1";
					Effect = "scr_trev1_trailer_splash";
					break;
				case 15:
					Dictionary = "scr_solomon3";
					Effect = "scr_trev4_747_blood_impact";
					break;
				case 16:
					Dictionary = "scr_family4";
					Effect = "scr_fam4_trailer_sparks";
					break;
				case 17:
					Dictionary = "scr_trevor1";
					Effect = "scr_trev1_trailer_boosh";
					break;
				case 18:
					Dictionary = "scr_ornate_heist";
					Effect = "scr_heist_ornate_banknotes";
					break;
				case 19:
					Dictionary = "scr_carsteal4";
					Effect = "scr_carsteal4_wheel_burnout";
					break;
				}

				if (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(Dictionary))
				{
					STREAMING::REQUEST_NAMED_PTFX_ASSET(Dictionary);
					Script::Current()->Yield(5ms);
				}

				GRAPHICS::USE_PARTICLE_FX_ASSET(Dictionary);
				GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(Effect, BulletCoord.x, BulletCoord.y, BulletCoord.z, 0.f, 0.f, 0.f, m_ParticleGunScale, FALSE, FALSE, FALSE, FALSE);
			}
		}
	}

	Entity g_Target;
	static auto LastTime = std::chrono::steady_clock::now();
	void WeaponFeatures::EnableEntityGun(bool enable)
	{
		const auto CurrentTime = std::chrono::steady_clock::now();
		const auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - LastTime).count();

		if (enable && !GUI::IsOpen() && ElapsedTime >= m_EntityGunDelay && Weapons::IsPedShooting(PLAYER::PLAYER_PED_ID()))
		{
			if (m_EntityGunTypePos == 0) // Object
			{
				Hash ObjectHash = Lists::g_EntityGunObjects[m_EntityGunObjectPos];

				if (!STREAMING::HAS_MODEL_LOADED(ObjectHash))
				{
					STREAMING::REQUEST_MODEL(ObjectHash);
					Script::Current()->Yield(5ms);
				}

				System::ModelSpawnBypass(true);
				g_Target = OBJECT::CREATE_OBJECT(ObjectHash, 0.f, 0.f, 0.f, TRUE, TRUE, FALSE);
				System::ModelSpawnBypass(false);
			}
			else if (m_EntityGunTypePos == 1) // Vehicle 
			{
				Hash VehicleHash = Lists::g_EntityGunVehicles[m_EntityGunVehiclePos];

				if (!STREAMING::HAS_MODEL_LOADED(VehicleHash))
				{
					STREAMING::REQUEST_MODEL(VehicleHash);
					Script::Current()->Yield(5ms);
				}

				System::ModelSpawnBypass(true);
				g_Target = VEHICLE::CREATE_VEHICLE(VehicleHash, 0.f, 0.f, 0.f, 0.f, TRUE, TRUE, FALSE);
				System::ModelSpawnBypass(false);
			}
			else if (m_EntityGunTypePos == 2) // Ped
			{
				Hash PedHash = Lists::g_EntityGunPeds[m_EntityGunPedPos];

				if (!STREAMING::HAS_MODEL_LOADED(PedHash))
				{
					STREAMING::REQUEST_MODEL(PedHash);
					Script::Current()->Yield(5ms);
				}

				System::ModelSpawnBypass(true);
				g_Target = PED::CREATE_PED(21, PedHash, 0.f, 0.f, 0.f, 0.f, TRUE, TRUE);
				System::ModelSpawnBypass(false);
			}

			if (ENTITY::DOES_ENTITY_EXIST(g_Target))
			{
				const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
				auto SpawnCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
				const float Pitch = Math::DegreeToRadian(Rotation.x); // Vertical
				const float Yaw = Math::DegreeToRadian(Rotation.z + 90); // Horizontal
				SpawnCoords.x += 10.f * cos(Pitch) * cos(Yaw);
				SpawnCoords.y += 10.f * sin(Yaw) * cos(Pitch);
				SpawnCoords.z += 10.f * sin(Pitch);
				ENTITY::SET_ENTITY_COORDS(g_Target, SpawnCoords.x, SpawnCoords.y, SpawnCoords.z, FALSE, FALSE, FALSE, FALSE);
				ENTITY::SET_ENTITY_ROTATION(g_Target, Rotation.x, Rotation.y, Rotation.z, 2, TRUE);
				ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(g_Target, 1, 0.f, m_EntityGunForce, 0.f, FALSE, TRUE, TRUE, FALSE);
				if (ENTITY::IS_ENTITY_A_VEHICLE(g_Target)) 
					VEHICLE::SET_VEHICLE_OUT_OF_CONTROL(g_Target, TRUE, TRUE);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(g_Target, TRUE, TRUE);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&g_Target);

				LastTime = CurrentTime;
			}
		}
	}

	void WeaponFeatures::EnableWeaponWidth(bool enable)
	{
		// GTA::GetLocalPed()->m_weapon_manager->m_weapon_object + 0x7C
		if (enable)
			GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x7C }, m_WeaponWidth);
	}

	void WeaponFeatures::EnableWeaponHeight(bool enable)
	{
		// GTA::GetLocalPed()->m_weapon_manager->m_weapon_object + 0x8C
		if (enable)
			GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x8C }, m_WeaponHeight);
	}

	void WeaponFeatures::EnableWeaponScale(bool enable)
	{
		if (enable) {
			GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x7C }, m_WeaponScale);
			GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x8C }, m_WeaponScale);
		}
	}

	// [0] Access - Vector3
	void WeaponFeatures::EnablePositionX(bool enable)
	{
		if (enable)
			GTA::GetLocalPed()->m_transformation_matrix.data[0][7] = m_PositionX;
	}

	void WeaponFeatures::EnablePositionY(bool enable)
	{
		if (enable)
			GTA::GetLocalPed()->m_transformation_matrix.data[0][11] = m_PositionY;
	}

	std::vector<std::uint32_t> m_ConvertedWeapons;
	std::vector<std::int16_t> m_ConvertedWeaponsPos;
	void WeaponFeatures::AllowWeaponsInVehicle(bool enable)
	{
		const std::uint64_t Table = *(std::uint64_t*)Pointers::pWeaponInfoHashList;
		if (Table)
		{
			for (std::int16_t i = 0; i < (*(int16_t*)Pointers::pWeaponInfoHashListCount) - 1; i++) 
			{
				CWeaponInfo* WeaponInfo = (CWeaponInfo*)*(std::uint64_t*)(Table + (i * 8));
				if (WeaponInfo) 
				{
					if (enable) 
					{
						std::uint32_t weapon_groups[] = 
						{
							0x18D5FA97,
							0xC6E9A5C5,
							0x39D5C192,
							0x451B04BC,
							0x33431399,
							0xB7BBD827,
							0xA27A4F9F,
							0x29268262,
							0xFDBF656C
						};

						for (std::uint32_t group : weapon_groups) 
						{
							if (WeaponInfo->m_group == group) {
								m_ConvertedWeapons.push_back(WeaponInfo->m_group);
								m_ConvertedWeapons.push_back(i);
								WeaponInfo->m_group = 0x18d5fa97;
							}
						}
					}
					else 
					{
						const int Position = Math::GetIndex(m_ConvertedWeaponsPos, i);
						if (Position != -1) 
							WeaponInfo->m_group = m_ConvertedWeaponsPos.at(Position);
					}

				}
			}
			if (!enable) 
			{
				m_ConvertedWeapons.clear();
				m_ConvertedWeaponsPos.clear();
			}
		}
	}

	void WeaponFeatures::FriendlyFire(bool enable)
	{
		if (enable)
			PED::SET_CAN_ATTACK_FRIENDLY(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		else
			PED::SET_CAN_ATTACK_FRIENDLY(PLAYER::PLAYER_PED_ID(), FALSE, FALSE);
	}

	float g_LastLockOnValue = 0.f;
	bool g_LastInstantLockOn = false;
	void WeaponFeatures::InstantLockOn(bool enable)
	{
		// Whack method, doesn't even restore properly
		if (g_LastLockOnValue == 0.f)
			g_LastLockOnValue = GTA::Offsets::GetValue<float>({ 0x8, 0x10B8, 0x20, 0x60, 0x178 });

		if (enable)
		{
			Hash WeaponHash{};
			WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &WeaponHash, TRUE);
			if (WeaponHash == 0x63AB0442) // Homing Launcher
			{
				GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x20, 0x60, 0x178 }, 0.f);
			}
		}
		else if (enable != g_LastInstantLockOn)
		{
			Hash WeaponHash{};
			WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &WeaponHash, TRUE);
			if (WeaponHash == 0x63AB0442) // Homing Launcher
			{
				GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x20, 0x60, 0x178 }, g_LastLockOnValue);
			}
		}

		g_LastInstantLockOn = enable;
	}

	void WeaponFeatures::BypassC4Limit(bool enable)
	{
		if (enable)
			GTA::GetLocalPed()->fired_sticky_bombs = 0;
	}

	void WeaponFeatures::BypassFlareLimit(bool enable)
	{
		if (enable)
			GTA::GetLocalPed()->fired_flares = 0;
	}

	void WeaponFeatures::EnableTriggerBot(bool enable)
	{
		if (enable)
		{
			Entity Ped;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &Ped))
			{
				const auto Relationship = PED::GET_RELATIONSHIP_BETWEEN_PEDS(Ped, PLAYER::PLAYER_PED_ID());
				const auto Type = PED::GET_PED_TYPE(Ped);

				if (!m_TriggerBotPolice && Type == 6 || !m_TriggerBotPolice && Type == 27)
					return;

				if (((Relationship == 4) || (Relationship == 5)) && !m_TriggerBotEnemies)
					return;

				if (PED::IS_PED_A_PLAYER(Ped) && !m_TriggerBotPlayers)
					return;

				if (Relationship == 255 && !m_TriggerBotCivilians)
					return;

				if (ENTITY::IS_ENTITY_A_PED(Ped) && !ENTITY::IS_ENTITY_DEAD(Ped, FALSE)) {
					const auto Coords = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(Ped, PED::GET_PED_BONE_INDEX(Ped, Lists::g_AimAssistBones[m_TriggerBotBonePos]));
					PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z, TRUE);
				}
			}
		}
	}

	NativeVector3 g_AimLock;
	Vector2 g_MouseMovement;
	void WeaponFeatures::EnableAimBot(bool enable)
	{
		if (enable)
		{
			float FOVChange = m_AimBotFOV;
			for (const auto& Ped : Entities::GetEntities(false, true, false))
			{
				if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(PLAYER::PLAYER_PED_ID(), Ped, 17) && !ENTITY::IS_ENTITY_DEAD(Ped, NULL))
				{
					const auto Relation = PED::GET_RELATIONSHIP_BETWEEN_PEDS(Ped, PLAYER::PLAYER_PED_ID());
					const auto Type = PED::GET_PED_TYPE(Ped);
					const auto MyPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
					const auto TargetPosition = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

					if (Math::DistanceBetweenVectors(MyPosition, TargetPosition) > m_AimBotDistance)
						continue;

					if (PED::IS_PED_A_PLAYER(Ped) && m_AimBotPlayers)
					{
						goto aimbot_handler;
					}
					else if (((Relation == 4) || (Relation == 5)) && m_AimBotEnemies)
					{
						goto aimbot_handler;
					}
					else if (((Type == 6 && !PED::IS_PED_MODEL(Ped, rage::joaat("s_m_y_uscg_01"))) || Type == 27 ||
						PED::IS_PED_MODEL(Ped, rage::joaat("s_m_y_ranger_01")) || PED::IS_PED_MODEL(Ped, rage::joaat("s_f_y_ranger_01")))
						&& m_AimBotPolice)
					{
						goto aimbot_handler;
					}
					else if (m_AimBotCivilians && !PED::IS_PED_A_PLAYER(Ped))
	
					aimbot_handler:
					{
						g_AimLock = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(Ped, PED::GET_PED_BONE_INDEX(Ped, Lists::g_AimAssistBones[m_AimBotBonePos]));
						if (!Math::Empty(g_AimLock))
						{
							Vector2 ScreenDimensions, Movement;
							GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(g_AimLock.x,
								g_AimLock.y,
								g_AimLock.z,
								&ScreenDimensions.x,
								&ScreenDimensions.y);
							if ((ScreenDimensions.x >= 0.f) && (ScreenDimensions.y >= 0.f))
							{
								const auto& ImGui = ImGui::GetIO();
								ImVec2 Center(ImGui.DisplaySize.x / 2.f, ImGui.DisplaySize.y / 2.f);
								ScreenDimensions.x *= ImGui.DisplaySize.x;
								ScreenDimensions.y *= ImGui.DisplaySize.y;

								if (ScreenDimensions.x > Center.x)
								{
									Movement.x = -(Center.x - ScreenDimensions.x);
									if (Movement.x + Center.x > Center.x * 2.f)
										Movement.x = 0.f;
								}
								else
								{
									Movement.x = ScreenDimensions.x - Center.x;
									if (Movement.x + Center.x < 0.f)
										Movement.x = 0.f;
								}

								if (ScreenDimensions.y > Center.y)
								{
									Movement.y = -(Center.y - ScreenDimensions.y);
									if (Movement.y + Center.y > Center.y * 2.f)
										Movement.x = 0.f;
								}
								else
								{
									Movement.y = ScreenDimensions.y - Center.y;
									if (Movement.y + Center.y < 0.f)
										Movement.y = 0.f;
								}

								if (sqrt(pow(Movement.x, 2.f) + pow(Movement.y, 2.f)) < FOVChange)
								{
									FOVChange = sqrt(pow(Movement.x, 2.f) + pow(Movement.y, 2.f));
									g_MouseMovement.x = Movement.x;
									g_MouseMovement.y = Movement.y;
								}
							}
						}
					}
				}
			}

			if (PAD::GET_DISABLED_CONTROL_NORMAL(0, (int)eControllerInputs::INPUT_AIM))
			{
				static bool UpdateTimeNow = true;
				static std::chrono::system_clock::time_point CurrentTime;

				if (UpdateTimeNow)
				{
					CurrentTime = std::chrono::system_clock::now();
					UpdateTimeNow = false;
				}

				std::chrono::duration<double> ElapsedTime = std::chrono::system_clock::now() - CurrentTime;
				if (ElapsedTime.count() > 0.f)
				{
					INPUT MouseHandle; // MOUSEINPUT mi;
					MouseHandle.type = INPUT_MOUSE;
					MouseHandle.mi.dwFlags = MOUSEEVENTF_MOVE; // Type = Mouse movement, and the event  is emulating the mouse movement

					// Update the mouse by moving it with how much we need / smoothing speed
					MouseHandle.mi.dx = g_MouseMovement.x / (m_LegitBot ? m_AimBotLegitSpeed : 2.f);
					MouseHandle.mi.dy = g_MouseMovement.y / (m_LegitBot ? m_AimBotLegitSpeed : 2.f);
					SendInput(1, &MouseHandle, sizeof(MouseHandle));//handles the input

					// Reset our variables
					g_MouseMovement = {};
					UpdateTimeNow = true;// Reset our time
				}
			}
		}
	}

	void WeaponFeatures::RainbowRegularTint(bool enable)
	{
		if (enable)
		{
			static Timer rainbowTimer(0ms);
			rainbowTimer.SetDelay(std::chrono::milliseconds(250));
			if (rainbowTimer.Update())
				WEAPON::SET_PED_WEAPON_TINT_INDEX(PLAYER::PLAYER_PED_ID(), WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), Math::RandomNumberInRange(0, 7));
		}
	}

	void WeaponFeatures::RainbowMk2Tint(bool enable)
	{
		if (enable)
		{
			static Timer rainbowTimer(0ms);
			rainbowTimer.SetDelay(std::chrono::milliseconds(250));
			if (rainbowTimer.Update())
				WEAPON::SET_PED_WEAPON_TINT_INDEX(PLAYER::PLAYER_PED_ID(), WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), Math::RandomNumberInRange(0, 31));
		}
	}

	void WeaponFeatures::EnableCrosshair(bool enable)
	{
		if (enable && Weapons::IsPedAiming(PLAYER::PLAYER_PED_ID()))
		{
			if (m_RainbowCrosshair)
			{
				m_CrosshairColor[0] = System::g_SlowRainbowColor.r / 255.f;
				m_CrosshairColor[1] = System::g_SlowRainbowColor.g / 255.f;
				m_CrosshairColor[2] = System::g_SlowRainbowColor.b / 255.f;
			}

			const Color CrossHairColor = { m_CrosshairColor[0] * 255.f, m_CrosshairColor[1] * 255.f, m_CrosshairColor[2] * 255.f, 255 };
			switch (m_CrosshairPos)
			{
			case 0:
				System::DrawSprite("srange_gen", "hit_cross", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 1:
				System::DrawSprite("helicopterhud", "hud_target", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 2:
				System::DrawSprite("helicopterhud", "hudarrow", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 3:
				System::DrawSprite("mptattoos3", "tattoo_reach_rank_r_10", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 4:
				System::DrawSprite("darts", "dart_reticules", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 5:
				System::DrawSprite("darts", "dart_reticules_zoomed", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 6:
				System::DrawSprite("shared", "emptydot_32", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 7:
				System::DrawSprite("shared", "menuplus_32", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 8:
				System::DrawSprite("mp_freemode_mc", "mouse", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 9:
				System::DrawSprite("visualflow", "crosshair", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			case 10:
				System::DrawSprite("shared", "newstar_32", 0.5f, 0.5f, 0.02f, 0.03f, CrossHairColor, 0.f);
				break;
			}
		}
	}
}