#include "Self.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/Kicks.hpp"
#include "../GTA/enums.hpp"
#include "../Pointers/Pointers.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/Script/ScriptLocal.hpp"
#include "../Utility/Timer.hpp"
#include "../Utility/Joaat.hpp"
#include "../Utility/All.hpp"

namespace Sentinel
{
	void SelfFeatures::GodMode(bool enable)
	{ 
		if (enable)
			GTA::Offsets::SetValue<bool>({ 0x8, 0x189 }, true);
	}

	void SelfFeatures::SemiGodMode(bool enable)
	{
		if (enable && GTA::GetLocalPed()->m_health < GTA::GetLocalPed()->m_maxhealth)
			GTA::GetLocalPed()->m_health = GTA::GetLocalPed()->m_maxhealth;
	}

	void SelfFeatures::UnbreakableArmor(bool enable)
	{
		if (enable)
			GTA::GetLocalPed()->m_armor = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID());
	}

	void SelfFeatures::AutoRestore(bool enable)
	{
		if (enable)
		{
			if (m_ComponentPos != 0)
			{
				float Armor = PED::GET_PED_ARMOUR(PLAYER::PLAYER_PED_ID());
				const float MaxArmor = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID());
				float Value = 20.f;
				static Timer repairTimer(0ms);
				repairTimer.SetDelay(std::chrono::milliseconds(400));
				if (repairTimer.Update())
				{
					Value += Armor;
					if (Armor < MaxArmor)
					{
						Armor = Value;
						PED::SET_PED_ARMOUR(PLAYER::PLAYER_PED_ID(), Value);
					}
					if (Armor == MaxArmor)
						Armor = MaxArmor;
				}
			}
			if (m_ComponentPos != 1)
			{
				float Health = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID());
				const float MaxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID());
				float Value = 20.f;
				static Timer healTimer(0ms);
				healTimer.SetDelay(std::chrono::milliseconds(400));
				if (healTimer.Update())
				{
					Value += Health;
					if (Health < MaxHealth)
					{
						Health = Value;
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), Value, FALSE);
					}
					if (Health == MaxHealth)
						Health = MaxHealth;
				}
			}
		}
	}

	void SelfFeatures::RestoreInCover(bool enable)
	{
		if (enable && PED::IS_PED_IN_COVER(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			if (m_ComponentPos != 0)
			{
				float Armor = PED::GET_PED_ARMOUR(PLAYER::PLAYER_PED_ID());
				const float MaxArmor = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID());
				float Value = 20.f;
				static Timer repairTimer(0ms);
				repairTimer.SetDelay(std::chrono::milliseconds(400));
				if (repairTimer.Update())
				{
					Value += Armor;
					if (Armor < MaxArmor)
					{
						Armor = Value;
						PED::SET_PED_ARMOUR(PLAYER::PLAYER_PED_ID(), Value);
					}
					if (Armor == MaxArmor)
						Armor = MaxArmor;
				}
			}
			if (m_ComponentPos != 1)
			{
				float Health = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID());
				const float MaxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID());
				float Value = 20.f;
				static Timer healTimer(0ms);
				healTimer.SetDelay(std::chrono::milliseconds(400));
				if (healTimer.Update())
				{
					Value += Health;
					if (Health < MaxHealth)
					{
						Health = Value;
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), Value, FALSE);
					}
					if (Health == MaxHealth)
						Health = MaxHealth;
				}
			}
		}
	}

	static const eControllerInputs g_NoClipControls[] =
	{
		eControllerInputs::INPUT_SPRINT,
		eControllerInputs::INPUT_DUCK,
		eControllerInputs::INPUT_MOVE_UP_ONLY,
		eControllerInputs::INPUT_MOVE_DOWN_ONLY
	};

	bool g_LastNoClip = false;
	bool g_ClearTasks = true;
	void SelfFeatures::NoClip(bool enable)
	{
		if (enable)
		{
			// Disable Controls
			for (const auto& Control : g_NoClipControls)
				PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(Control), TRUE);

			if (g_ClearTasks)
			{
				Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());
				g_ClearTasks = false;
			}

			const auto Ped = PLAYER::PLAYER_PED_ID();
			auto Position = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
			auto CamCoords = CAM::GET_GAMEPLAY_CAM_COORD();
			const auto CamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			auto Distance = std::distance(&CamCoords, &Position);

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Ped, Position.x, Position.y, Position.z, TRUE, TRUE, TRUE);
			ENTITY::SET_ENTITY_ROTATION(Ped, CamRot.x, CamRot.y, CamRot.z, 2, TRUE);

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
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Ped, Coords.x, Coords.y, Coords.z, TRUE, TRUE, TRUE);
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
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Ped, Coords.x, Coords.y, Coords.z, TRUE, TRUE, TRUE);
			}
		}
		else if (enable != g_LastNoClip)
			g_ClearTasks = true;

		g_LastNoClip = enable;
	}

	void SelfFeatures::SlideRun(bool enable)
	{
		if (enable)
		{
			if (PAD::IS_CONTROL_PRESSED(0, INPUT_MOVE_UP_ONLY) && PAD::IS_CONTROL_PRESSED(0, INPUT_SPRINT) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()))
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, m_SlideRunSpeed, 0.0f, 0.0f, 0.0f, 0.0f, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE);
		}
	}

	static int g_Flip;
	int g_Speed = 6;
	void SelfFeatures::NinjaJump(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
			{
				if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()))
				{
					g_Flip -= g_Speed;
					if (g_Flip >= -(360 * m_NinjaJumps))
						ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), g_Flip, 0, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 0);
				}
				else
					g_Flip = 0;
			}
		}
	}

	void SelfFeatures::SuperMan(bool enable)
	{
		if (enable)
		{
			// Disable Controls
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_SPRINT, TRUE);
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_DUCK, TRUE);

			if (!PED::IS_PED_IN_PARACHUTE_FREE_FALL(PLAYER::PLAYER_PED_ID()))
			{
				if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(PLAYER::PLAYER_PED_ID()) > 4.0f)
					TASK::TASK_SKY_DIVE(PLAYER::PLAYER_PED_ID(), TRUE);
			}

			if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
			{
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_SPRINT))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 100.0f, 100.0f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
				}
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_DUCK))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, -100.f, -100.0f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
				}
			}
		}
	}

	bool g_LastWalkOnAir = false;
	Object g_WalkingPad = 0;
	void SelfFeatures::WalkOnAir(bool enable)
	{
		if (enable)
		{
			if (!ENTITY::DOES_ENTITY_EXIST(g_WalkingPad))
			{
				// Create the walking pad
				auto iCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
				iCoord.z -= 1.7f;
				System::ModelSpawnBypass(true);
				g_WalkingPad = OBJECT::CREATE_OBJECT(Joaat("prop_pooltable_02"), iCoord.x, iCoord.y, iCoord.z, TRUE, TRUE, FALSE);
				System::ModelSpawnBypass(false);
			}
			else
			{
				// Walk on the walking pad
				ENTITY::SET_ENTITY_VISIBLE(g_WalkingPad, FALSE, FALSE);
				if (PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) == 0)
				{
					auto iCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
					auto wCoord = ENTITY::GET_ENTITY_COORDS(g_WalkingPad, TRUE);
					if (sqrtf(powf(iCoord.x - wCoord.x, 2.f) + powf(iCoord.y - wCoord.y, 2.f) + powf(iCoord.z - wCoord.z, 2.f)) > 1.7f)
					{
						ENTITY::SET_ENTITY_HEADING(g_WalkingPad, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
						if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(g_WalkingPad, iCoord.x, iCoord.y, iCoord.z - 1.7f, FALSE, FALSE, FALSE);
						else
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(g_WalkingPad, iCoord.x, iCoord.y, wCoord.z, FALSE, FALSE, FALSE);
					}
				}
			}
		}
		else if (enable != g_LastWalkOnAir)
		{
			// Proper Cleanup
			if (ENTITY::DOES_ENTITY_EXIST(g_WalkingPad))
			{
				Entities::RequestControl(g_WalkingPad);
				Entities::DeleteEntity(g_WalkingPad);
				g_WalkingPad = 0;
			}
		}

		g_LastWalkOnAir = enable;
	}

	bool g_LastWalkOnWater = true;
	Object g_Container = 0;
	Hash g_ContainerHash = Joaat("prop_container_ld2");
	void SelfFeatures::WalkOnWater(bool enable)
	{
		if (enable)
		{
			const auto iCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			float WaterHeight = 0;
			if (WATER::GET_WATER_HEIGHT(iCoord.x, iCoord.y, iCoord.z, &WaterHeight))
			{
				WATER::SET_DEEP_OCEAN_SCALER(0.0f);
				g_Container = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(iCoord.x, iCoord.y, iCoord.z, 10.0f, g_ContainerHash, FALSE, FALSE, TRUE);
				if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
				{
					if (ENTITY::DOES_ENTITY_EXIST(g_Container) && WaterHeight > -50.0f)
					{
						ENTITY::SET_ENTITY_COORDS(g_Container, iCoord.x, iCoord.y, WaterHeight - 2.8f, 0, 0, 0, TRUE);
						ENTITY::SET_ENTITY_ROTATION(g_Container, 0.f, 0.f, ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0).z, 0, TRUE);
						auto ContainerCoord = ENTITY::GET_ENTITY_COORDS(g_Container, FALSE);
						if (iCoord.z < ContainerCoord.z)
							ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), iCoord.x, iCoord.y, ContainerCoord.z + 2.0f, 0, 0, 0, TRUE);
					}
					else
					{
						if (MISC::GET_DISTANCE_BETWEEN_COORDS(iCoord.x, iCoord.y, iCoord.z, iCoord.x, iCoord.y, WaterHeight, TRUE) < 5.0f)
						{
							STREAMING::REQUEST_MODEL(g_ContainerHash);
							while (!STREAMING::HAS_MODEL_LOADED(g_ContainerHash))
								Script::Current()->Yield();
							System::ModelSpawnBypass(true);
							g_Container = OBJECT::CREATE_OBJECT(g_ContainerHash, iCoord.x, iCoord.y, WaterHeight - 2.8f, TRUE, TRUE, FALSE);
							System::ModelSpawnBypass(false);
							ENTITY::FREEZE_ENTITY_POSITION(g_Container, TRUE);
							ENTITY::SET_ENTITY_VISIBLE(g_Container, FALSE, FALSE);
							NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(g_Container, TRUE);
						}
					}
				}
			}
			else if (enable != g_LastWalkOnWater)
			{
				g_Container = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(iCoord.x, iCoord.y, iCoord.z, 10.f, g_ContainerHash, FALSE, FALSE, TRUE);
				if (ENTITY::DOES_ENTITY_EXIST(g_Container))
				{
					Entities::RequestControl(g_Container);
					Entities::DeleteEntity(g_Container);
					WATER::RESET_DEEP_OCEAN_SCALER();
					g_Container = 0;
				}
			}

			g_LastWalkOnWater = enable;
		}
	}

	void SelfFeatures::WalkUnderWater(bool enable)
	{
		if (enable && ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()))
		{
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 65, FALSE);
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 66, FALSE);
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 168, FALSE);

			const auto iCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			GRAPHICS::DRAW_LIGHT_WITH_RANGE(iCoord.x, iCoord.y, (iCoord.z + 1.5f), 237, 233, 218, 100.f, 1.5f);
			GRAPHICS::DRAW_LIGHT_WITH_RANGE(iCoord.x, iCoord.y, (iCoord.z + 50.f), 237, 233, 218, 200.f, 1.f);

			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 0.f, 0.7f, 0.f, 0.f, 0xFF, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);

			if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(PLAYER::PLAYER_PED_ID()) > 1.2f)
			{
				PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 60, FALSE);
				PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 61, FALSE);
				PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 104, FALSE);
				PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 276, FALSE);
				PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 76, TRUE);
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 0.f, -0.7f, 0.f, 0.f, 0xFF, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
			}

			if (TASK::GET_IS_TASK_ACTIVE(PLAYER::PLAYER_PED_ID(), 281) || PED::IS_PED_SWIMMING_UNDER_WATER(PLAYER::PLAYER_PED_ID()))
				Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());
		}
	}

	void SelfFeatures::NoRagdoll(bool enable)
	{
		if (enable)
		{
			// Ragdoll Flags
			for (const auto& Flag : g_RagdollBlockingFlags)
				PED::SET_RAGDOLL_BLOCKING_FLAGS(PLAYER::PLAYER_PED_ID(), static_cast<int>(Flag));

			PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), FALSE);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), FALSE);
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 287, TRUE);

			// If by any chance you are still ragdolled, just get the fuck up?
			if (PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
				Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());

			// This speeds up the weird falling animation where you're injured
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 166, FALSE);
			PED::DISABLE_PED_INJURED_ON_GROUND_BEHAVIOUR(PLAYER::PLAYER_PED_ID());

			// Stop Injured Clipset
			PED::RESET_PED_STRAFE_CLIPSET(PLAYER::PLAYER_PED_ID());
		}
	}

	void SelfFeatures::WalkThroughWater(bool enable)
	{
		if (enable)
		{
			float Offset[] = { -4, 4 };
			for (int i = 0; i < 5; i++) {
				auto iCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
				if (i < 2) iCoord.x += Offset[(i % 2 == 0)];
				else if (i < 4) iCoord.y += Offset[(i % 2 == 0)];
				WATER::MODIFY_WATER(iCoord.x, iCoord.y, -500000.f, 0.2f);
			}
		}
	}

	static const eControllerInputs g_FreecamControls[] =
	{
		eControllerInputs::INPUT_LOOK_LR,
		eControllerInputs::INPUT_LOOK_UD,
		eControllerInputs::INPUT_LOOK_UP_ONLY,
		eControllerInputs::INPUT_LOOK_DOWN_ONLY,
		eControllerInputs::INPUT_LOOK_LEFT_ONLY,
		eControllerInputs::INPUT_LOOK_RIGHT_ONLY,
		eControllerInputs::INPUT_LOOK_LEFT,
		eControllerInputs::INPUT_LOOK_RIGHT,
		eControllerInputs::INPUT_LOOK_UP,
		eControllerInputs::INPUT_LOOK_DOWN
	};

	bool g_Initialized = false;
	bool g_LastFreecam = false;
	float g_FreecamSpeed = 0.5f;
	float g_FreecamMultiplier = 0.f;

	Cam g_FreecamCamera = -1;
	NativeVector3 g_FreeCamPosition;
	NativeVector3 g_FreeCamRotation;
	void SelfFeatures::Freecam(bool enable)
	{
		if (enable)
		{
			if (!g_Initialized)
			{
				// Initialize
				g_Initialized = true;

				g_FreecamCamera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", FALSE);

				g_FreeCamPosition = CAM::GET_GAMEPLAY_CAM_COORD();
				g_FreeCamRotation = CAM::GET_GAMEPLAY_CAM_ROT(2);

				ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), TRUE);
				ENTITY::FREEZE_ENTITY_POSITION(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), TRUE);

				CAM::SET_CAM_COORD(g_FreecamCamera, g_FreeCamPosition.x, g_FreeCamPosition.y, g_FreeCamPosition.z);
				CAM::SET_CAM_ROT(g_FreecamCamera, g_FreeCamRotation.x, g_FreeCamRotation.y, g_FreeCamRotation.z, 2);
				CAM::SET_CAM_ACTIVE(g_FreecamCamera, TRUE);
				CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 500, TRUE, TRUE, NULL);
			}

			// Tick
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

			for (const auto& Control : g_FreecamControls)
				PAD::ENABLE_CONTROL_ACTION(0, static_cast<int>(Control), TRUE);

			NativeVector3 VectorChange = { 0.f, 0.f, 0.f };

			// Left Shift
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_SPRINT))
				VectorChange.z += g_FreecamSpeed / 2.f;
			// Left Control
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_DUCK))
				VectorChange.z -= g_FreecamSpeed / 2.f;
			// Forward
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_MOVE_UP_ONLY))
				VectorChange.y += g_FreecamSpeed;
			// Backward
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_MOVE_DOWN_ONLY))
				VectorChange.y -= g_FreecamSpeed;
			// Left
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_MOVE_LEFT_ONLY))
				VectorChange.x -= g_FreecamSpeed;
			// Right
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_MOVE_RIGHT_ONLY))
				VectorChange.x += g_FreecamSpeed;

			if (VectorChange.x == 0.f && VectorChange.y == 0.f && VectorChange.z == 0.f)
				g_FreecamMultiplier = 0.f;
			else if (g_FreecamMultiplier < 10)
				g_FreecamMultiplier += 0.15f;

			const NativeVector3 Rotation = CAM::GET_CAM_ROT(g_FreecamCamera, 2);
			const auto Yaw = Math::DegreeToRadian(Rotation.z); // Horizontal

			g_FreeCamPosition.x += (VectorChange.x * cos(Yaw) - VectorChange.y * sin(Yaw)) * g_FreecamMultiplier;
			g_FreeCamPosition.y += (VectorChange.x * sin(Yaw) + VectorChange.y * cos(Yaw)) * g_FreecamMultiplier;
			g_FreeCamPosition.z += VectorChange.z * g_FreecamMultiplier;

			CAM::SET_CAM_COORD(g_FreecamCamera, g_FreeCamPosition.x, g_FreeCamPosition.y, g_FreeCamPosition.z);
			STREAMING::SET_FOCUS_POS_AND_VEL(g_FreeCamPosition.x, g_FreeCamPosition.y, g_FreeCamPosition.z, 0.f, 0.f, 0.f);

			g_FreeCamRotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
			CAM::SET_CAM_ROT(g_FreecamCamera, g_FreeCamRotation.x, g_FreeCamRotation.y, g_FreeCamRotation.z, 2);
		}
		else if (enable != g_LastFreecam)
		{
			// Cleanup
			CAM::SET_CAM_ACTIVE(g_FreecamCamera, FALSE);
			CAM::RENDER_SCRIPT_CAMS(FALSE, TRUE, 500, TRUE, TRUE, NULL);
			CAM::DESTROY_CAM(g_FreecamCamera, FALSE);
			STREAMING::CLEAR_FOCUS();

			ENTITY::FREEZE_ENTITY_POSITION(g_FreecamCamera, FALSE);
			ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), FALSE);
			ENTITY::FREEZE_ENTITY_POSITION(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), FALSE);

			g_Initialized = false;
		}

		g_LastFreecam = enable;
	}

	void SelfFeatures::EnableRunSpeed(bool enable)
	{
		if (enable)
			PED::SET_PED_MOVE_RATE_OVERRIDE(PLAYER::PLAYER_PED_ID(), m_RunSpeed);
	}

	void SelfFeatures::EnableSwimSpeed(bool enable)
	{
		if (enable)
			PED::SET_PED_MOVE_RATE_IN_WATER_OVERRIDE(PLAYER::PLAYER_PED_ID(), m_SwimSpeed);
	}

	void SelfFeatures::EnablePedWidth(bool enable)
	{
		if (enable)
			GTA::Offsets::SetValue<float>({ 0x8, 0x60 }, m_PedWidth);
	}

	void SelfFeatures::EnablePedHeight(bool enable)
	{
		if (enable)
			GTA::Offsets::SetValue<float>({ 0x8, 0x88 }, m_PedHeight);
	}

	void SelfFeatures::EnablePedLean(bool enable)
	{
		if (enable)
			GTA::Offsets::SetValue<float>({ 0x8, 0x80 }, m_PedLean);
	}

	void SelfFeatures::SwimAnywhere(bool enable)
	{
		if (enable)
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 65, TRUE);
	}

	void SelfFeatures::NoMelee(bool enable)
	{
		if (enable)
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 122, TRUE);
	}

	void SelfFeatures::CanPedFlyThroughWindshield(bool enable)
	{
		if (enable)
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 32, TRUE);
	}

	void SelfFeatures::PedDiesByRagdoll(bool enable)
	{
		if (enable)
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 33, TRUE);
	}

	void SelfFeatures::NoJumping(bool enable)
	{
		if (enable)
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_JUMP, TRUE);
	}

	void SelfFeatures::NoCollision(bool enable)
	{
		if (enable)
			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), FALSE, TRUE);
	}

	void SelfFeatures::ReducedCollision(bool enable)
	{
		if (enable)
			PED::SET_PED_CAPSULE(PLAYER::PLAYER_PED_ID(), 0.001f);
	}

	void SelfFeatures::Freeze(bool enable)
	{
		if (enable)
			ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), TRUE);
	}

	void SelfFeatures::Shrink(bool enable)
	{
		if (enable)
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, TRUE);
	}

	void SelfFeatures::Handtrails(bool enable)
	{
		if (enable)
		{
			if (m_RainbowHandtrails)
			{
				m_HandTrailsColor[0] = System::g_SlowRainbowColor.r / 255.f;
				m_HandTrailsColor[1] = System::g_SlowRainbowColor.g / 255.f;
				m_HandTrailsColor[2] = System::g_SlowRainbowColor.b / 255.f;
			}

			Local::ParticleOnBone("scr_powerplay", "sp_powerplay_beast_appear_trails", ePedBones::SKEL_L_Hand, m_HandtrailsScale, true, m_HandTrailsColor[0], m_HandTrailsColor[1], m_HandTrailsColor[2]);
			Local::ParticleOnBone("scr_powerplay", "sp_powerplay_beast_appear_trails", ePedBones::SKEL_R_Hand, m_HandtrailsScale, true, m_HandTrailsColor[0], m_HandTrailsColor[1], m_HandTrailsColor[2]);
		}
	}

	void SelfFeatures::NeverWanted(bool enable)
	{
		if (enable)
		{
			GTA::GetLocalPlayerInfo()->m_wanted_level = 0;
			GTA::GetLocalPlayerInfo()->m_wanted_level_display = 0;
			GTA::GetLocalPlayerInfo()->m_is_wanted = false;
			Pointers::pMaxWantedLevel->Apply();
			Pointers::pMaxWantedLevel2->Apply();
		}
	}

	void SelfFeatures::DisableSharing(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
				VEHICLE::SET_VEHICLE_INFLUENCES_WANTED_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), FALSE);
		}
	}

	void SelfFeatures::EnableFakeWantedLevel(bool enable)
	{
		if (enable)
			GTA::GetLocalPlayerInfo()->m_wanted_level_display = m_FakeWantedLevel;
	}

	void SelfFeatures::EnableRealWantedLevel(bool enable)
	{
		if (enable)
			GTA::GetLocalPlayerInfo()->m_wanted_level = m_RealWantedLevel;
	}

	void SelfFeatures::QuickRespawn(bool enable)
	{
		if (enable && NETWORK::NETWORK_IS_SESSION_ACTIVE())
		{
			const auto Ped = PLAYER::PLAYER_PED_ID();
			if (ENTITY::IS_ENTITY_DEAD(Ped, FALSE) || ENTITY::GET_ENTITY_HEALTH(Ped) == 0)
			{
				Entities::RequestControl(Ped);
				ENTITY::FREEZE_ENTITY_POSITION(Ped, TRUE);
				PED::RESURRECT_PED(Ped);
				ENTITY::FREEZE_ENTITY_POSITION(Ped, FALSE);
				Entities::ClearPedTasks(Ped);
				if (CAM::IS_SCREEN_FADING_IN())
					CAM::DO_SCREEN_FADE_OUT(NULL);
				if (CAM::IS_SCREEN_FADING_OUT())
					CAM::DO_SCREEN_FADE_IN(NULL);
			}
		}
	}

	void SelfFeatures::RespawnAtDeath(bool enable)
	{
		if (enable && NETWORK::NETWORK_IS_SESSION_ACTIVE())
		{
			static NativeVector3 DeathCoords;
			const auto Ped = PLAYER::PLAYER_PED_ID();
			const auto Dead = PED::IS_PED_DEAD_OR_DYING(Ped, TRUE);
			const auto Alive = !Dead;

			if (Dead)
				DeathCoords = ENTITY::GET_ENTITY_COORDS(Ped, FALSE);
			if (Alive)
			{
				if (DeathCoords.x != 0 && DeathCoords.y != 0 && DeathCoords.z != 0)
				{
					Script::Current()->Yield(50ms);
					ENTITY::SET_ENTITY_COORDS(Ped, DeathCoords.x, DeathCoords.y, DeathCoords.z, FALSE, FALSE, FALSE, FALSE);
					DeathCoords = { 0.f, 0.f, 0.f };
				}
			}
		}
	}

	void SelfFeatures::InfiniteStamina(bool enable)
	{
		const auto cPlayerInfo = GTA::GetLocalPlayerInfo();
		if (m_Stamina == 0.f)
			m_Stamina = cPlayerInfo->m_stamina;

		if (enable)
			cPlayerInfo->m_stamina = m_Stamina;
	}

	void SelfFeatures::Ignored(bool enable)
	{
		if (enable)
		{
			for (const auto Entity : Entities::GetEntities(false, true, false))
			{
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Entity, TRUE);
				PED::SET_PED_FLEE_ATTRIBUTES(Entity, 0, FALSE);
				PED::SET_PED_COMBAT_ATTRIBUTES(Entity, 17, TRUE);
			}
		}
	}

	void SelfFeatures::AntiNPC(bool enable)
	{
		if (enable)
		{
			for (const auto Entity : Entities::GetEntities(false, true, false))
			{
				if (PED::IS_PED_IN_COMBAT(Entity, PLAYER::PLAYER_PED_ID()) && !ENTITY::IS_ENTITY_DEAD(Entity, FALSE))
					PED::APPLY_DAMAGE_TO_PED(Entity, 1000.f, FALSE, NULL);
			}
		}
	}

	void SelfFeatures::Karma(bool enable)
	{
		if (enable && NETWORK::NETWORK_IS_SESSION_ACTIVE())
		{
			const auto Ped = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_DEAD_OR_DYING(Ped, TRUE))
			{
				const auto Source = PED::GET_PED_SOURCE_OF_DEATH(Ped);
				if (ENTITY::IS_ENTITY_A_PED(Source))
				{
					if (PED::IS_PED_A_PLAYER(Source))
					{
						if (!PED::IS_PED_DEAD_OR_DYING(Source, TRUE))
						{
							// Make it appear as a little suicide
							const auto Player = g_PlayerService->GetByPed(Source);
							if (!(Player->GetPed()->m_damage_bits & (1 << 8)))
							{
								Pointers::pSendNetworkDamage((CEntity*)g_PlayerService->GetSelf()->GetPed(), (CEntity*)Player->GetPed(), Player->GetPed()->m_navigation->get_position(),
									0, true, "weapon_explosion"_J, 10000.f, 2, 0, (1 << 4), 0, 0, 0, false, false, true, true, nullptr);
							}
						}
					}
				}
			}
		}
	}

	void SelfFeatures::FireBreath(bool enable)
	{
		if (enable)
		{
			if (STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("core")) {
				GRAPHICS::USE_PARTICLE_FX_ASSET("core");

				if (!GRAPHICS::DOES_PARTICLE_FX_LOOPED_EXIST(m_FireBreathHandle))
					m_FireBreathHandle = GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY_BONE("ent_sht_flame", PLAYER::PLAYER_PED_ID(), -0.02f, 0.2f, 0.f, 90.f, 100.f, 90.f, PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 31086), 1.f, FALSE, FALSE, FALSE);
			}
			else 
				STREAMING::REQUEST_NAMED_PTFX_ASSET("core");
		}
	}

	void SelfFeatures::Vomit(bool enable)
	{
		if (enable)
		{
			if (STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("core")) {
				GRAPHICS::USE_PARTICLE_FX_ASSET("core");

				if (!GRAPHICS::DOES_PARTICLE_FX_LOOPED_EXIST(m_VomitHandle))
					m_VomitHandle = GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY_BONE("ent_sht_petrol", PLAYER::PLAYER_PED_ID(), -0.02f, 0.2f, 0.f, 90.f, 100.f, 90.f, PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 31086), 1.f, FALSE, FALSE, FALSE);
			}
			else
				STREAMING::REQUEST_NAMED_PTFX_ASSET("core");
		}
	}

	void SelfFeatures::SlowMotion(bool enable)
	{
		if (enable)
			MISC::SET_TIME_SCALE(0.6f);
	}

	void SelfFeatures::MobileRadio(bool enable)
	{
		if (enable)
		{
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(TRUE);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(TRUE);
		}
	}

	void SelfFeatures::Invisibility(bool enable)
	{
		if (enable)
		{
			switch (m_InvisibilityModePos)
			{
			case 0:
				NETWORK::SET_LOCAL_PLAYER_INVISIBLE_LOCALLY(TRUE);
				break;
			case 1:
				NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PLAYER::PLAYER_PED_ID(), TRUE);
				NETWORK::SET_ENTITY_LOCALLY_VISIBLE(PLAYER::PLAYER_PED_ID());
				ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), FALSE, FALSE);
				break;
			case 2:
				NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PLAYER::PLAYER_PED_ID(), TRUE);
				NETWORK::SET_LOCAL_PLAYER_INVISIBLE_LOCALLY(TRUE);
				ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), FALSE, FALSE);
				break;
			}

		}
	}

	void SelfFeatures::EnableOpacity(bool enable)
	{
		if (enable)
			ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), m_Opacity, FALSE);
	}

	const char* g_Dict = "";
	const char* g_Particle = "";
	void SelfFeatures::ParticleMan()
	{
		switch (m_ParticleManTypesPos)
		{
		case 0:
			g_Dict = "scr_agencyheist";
			g_Particle = "scr_fbi_dd_breach_smoke";
			break;
		case 1:
			g_Dict = "scr_rcbarry1";
			g_Particle = "scr_alien_disintegrate";
			break;
		case 2:
			g_Dict = "scr_rcbarry1";
			g_Particle = "scr_alien_teleport";
			break;
		case 3:
			g_Dict = "scr_rcbarry2";
			g_Particle = "scr_clown_appears";
			break;
		case 4:
			g_Dict = "scr_rcbarry2";
			g_Particle = "scr_clown_death";
			break;
		case 5:
			g_Dict = "scr_agencyheistb";
			g_Particle = "scr_agency3b_elec_box";
			break;
		}

		if (m_ParticleManHead)
			Local::ParticleOnBone(g_Dict, g_Particle, ePedBones::SKEL_Head, m_ParticleManScale);
		if (m_ParticleManLeftHand)
			Local::ParticleOnBone(g_Dict, g_Particle, ePedBones::SKEL_L_Hand, m_ParticleManScale);
		if (m_ParticleManRightHand)
			Local::ParticleOnBone(g_Dict, g_Particle, ePedBones::SKEL_R_Hand, m_ParticleManScale);
		if (m_ParticleManLeftFoot)
			Local::ParticleOnBone(g_Dict, g_Particle, ePedBones::SKEL_L_Foot, m_ParticleManScale);
		if (m_ParticleManRightFoot)
			Local::ParticleOnBone(g_Dict, g_Particle, ePedBones::SKEL_R_Foot, m_ParticleManScale);
	}

	void SelfFeatures::LoopFireworkParticles(bool enable) 
	{
		if (enable)
			Local::ParseParticle(0, m_FireworkParticlesPos);
	}

	void SelfFeatures::LoopClownParticles(bool enable)
	{
		if (enable)
			Local::ParseParticle(1, m_ClownParticlesPos);
	}

	void SelfFeatures::LoopAlienParticles(bool enable)
	{
		if (enable)
			Local::ParseParticle(2, m_AlienParticlesPos);
	}

	void SelfFeatures::LoopConstructionParticles(bool enable)
	{
		if (enable)
			Local::ParseParticle(3, m_ConstructionParticlesPos);
	}

	void SelfFeatures::LoopGoreParticles(bool enable)
	{
		if (enable)
			Local::ParseParticle(4, m_GoreParticlesPos);
	}

	void SelfFeatures::LoopMiscParticles(bool enable)
	{
		if (enable)
			Local::ParseParticle(5, m_MiscParticlesPos);
	}

	void SelfFeatures::ModelChanger()
	{
		if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID(), FALSE))
		{
			if (m_LastModel)
				m_ReapplyModel = true;
		}
		else
		{
			if (m_ReapplyModel)
			{
				m_ReapplyModel = false;

				if (m_LastModel)
					Local::ChangeModel(m_LastModel);
			}
		}
	}

	void SelfFeatures::OffRadar(bool enable)
	{
		if (enable)
		{
			*ScriptGlobal(2657589).At(PLAYER::PLAYER_ID(), 466).Add(210).As<std::int32_t*>() = 1;
			*ScriptGlobal(2672505).Add(57).As<std::int32_t*>() = NETWORK::GET_NETWORK_TIME() + 60;
		}
	}

	void SelfFeatures::CopsTurnBlindEye(bool enable)
	{
		if (enable)
		{
			*ScriptGlobal(2793046).Add(4654).As<std::int32_t*>() = 1;
			*ScriptGlobal(2793046).Add(4657).As<std::int32_t*>() = NETWORK::GET_NETWORK_TIME() + 60;
		}
	}

	void SelfFeatures::RevealPlayers(bool enable)
	{
		if (enable)
		{
			*ScriptGlobal(2657589).At(PLAYER::PLAYER_ID(), 466).Add(213).As<std::int32_t*>() = 1;
			*ScriptGlobal(2672505).Add(58).As<std::int32_t*>() = NETWORK::GET_NETWORK_TIME() + 60;
		}
	}

	static const eControllerInputs g_YogaGodControls[] =
	{
		eControllerInputs::INPUT_MOVE_UP_ONLY,
		eControllerInputs::INPUT_AIM,
		eControllerInputs::INPUT_ATTACK,
		eControllerInputs::INPUT_SELECT_NEXT_WEAPON,
		eControllerInputs::INPUT_SELECT_PREV_WEAPON,
		eControllerInputs::INPUT_NEXT_WEAPON,
		eControllerInputs::INPUT_PREV_WEAPON,
		eControllerInputs::INPUT_CURSOR_SCROLL_UP,
		eControllerInputs::INPUT_CURSOR_SCROLL_DOWN
	};
	bool g_LastYogaGod = false;
	void SelfFeatures::YogaGod(bool enable)
	{
		if (enable)
		{
			// Variables
			const auto Ped = PLAYER::PLAYER_PED_ID();
			const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

			// Disable Controls
			for (const auto& Control : g_YogaGodControls)
				PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(Control), TRUE);

			// Start the meditation animation
			Local::PlayAnimation("rcmcollect_paperleadinout@", "meditiate_idle", false);

			// Rotate our ped
			static int Heading;
			Heading++;
			Heading++;
			Heading++;
			Heading++;
			ENTITY::SET_ENTITY_HEADING(Ped, Heading);

			// Force our ped's position
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Ped, Coords.x, Coords.y, Coords.z, TRUE, TRUE, TRUE);

			// Make our ped move forward
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_UP_ONLY))
			{
				const auto ForwardCoords = Math::Add(Coords, Math::Multiply(Math::RotationToDirection(CAM::GET_GAMEPLAY_CAM_ROT(0)), 3.f / 9));
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Ped, ForwardCoords.x, ForwardCoords.y, ForwardCoords.z, TRUE, TRUE, TRUE);
			}

			// Start the particles
			Local::ParticleOnBone("scr_rcbarry1", "scr_alien_teleport", SKEL_L_Hand, 0.1f);
			Local::ParticleOnBone("scr_rcbarry1", "scr_alien_teleport", SKEL_R_Hand, 0.1f);

			// Pull vehicles in
			static int Zoom = 19;
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_AIM))
			{
				for (const auto Entity : Entities::GetEntities(true, false, true))
				{
					if (*Pointers::pIsSessionStarted)
						Entities::ChangeNetworkObjectOwner(Entity, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
					const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
					const auto Direction = Math::RotationToDirection(Rotation);
					const auto SpawnCoords = Math::Add(CAM::GET_GAMEPLAY_CAM_COORD(), Math::Multiply(Direction, Zoom));

					const auto VehicleCoords = ENTITY::GET_ENTITY_COORDS(Entity, FALSE);
					const Vector3 Velocity = { Math::SmoothGlide(SpawnCoords.x, VehicleCoords.x), Math::SmoothGlide(SpawnCoords.y, VehicleCoords.y), Math::SmoothGlide(SpawnCoords.z, VehicleCoords.z) };

					ENTITY::SET_ENTITY_VELOCITY(Entity, Velocity.x * 2.f, Velocity.y * 2.f, Velocity.z * 2.f);

					// Draw our little marker
					GRAPHICS::DRAW_MARKER_SPHERE(SpawnCoords.x, SpawnCoords.y, SpawnCoords.z, 1.f, m_SuperPowerMarkers.r, m_SuperPowerMarkers.g, m_SuperPowerMarkers.b, m_SuperPowerMarkers.a / 255.f);

					// Push them away
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_ATTACK))
						ENTITY::APPLY_FORCE_TO_ENTITY(Entity, 1, Direction.x * 25000.f, Direction.y * 25000.f, Direction.z * 25000.f, 0.f, 0.f, 0.f, FALSE, FALSE, TRUE, TRUE, FALSE, TRUE);

					// Move them
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CURSOR_SCROLL_UP))
						Zoom += 1;
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CURSOR_SCROLL_DOWN))
						Zoom -= 1;
				}
			}
			else
				Zoom = 18;
		}
		else if (g_LastYogaGod != enable)
			Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());

		g_LastYogaGod = enable;
	}

	auto g_CamSpeed = 8.5f;
	void SelfFeatures::ElectricSprint(bool enable)
	{
		static Cam Freecam;
		const auto CamCoords = CAM::GET_CAM_COORD(Freecam);
		const auto iCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
		if (enable)
		{
			// Variables
			const auto GameplayRotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
			const auto GameplayCoord = CAM::GET_GAMEPLAY_CAM_COORD();
			const auto Direction = Math::RotationToDirection(GameplayRotation);
			const auto Calculation = Math::Multiply(Direction, g_CamSpeed);
			const auto FinalResult = Math::Add(CamCoords, Calculation);
			const auto Dict = "scr_martin1";
			const auto Particle = "ent_amb_elec_crackle_sp";

			// Create New Cam
			if (!CAM::DOES_CAM_EXIST(Freecam))
			{
				Freecam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", TRUE);
				CAM::SET_CAM_ROT(Freecam, GameplayRotation.x, GameplayRotation.y, GameplayRotation.z, 0);
				CAM::SET_CAM_COORD(Freecam, iCoord.x, iCoord.y, iCoord.z);
			}

			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), TRUE, TRUE);
			ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), CamCoords.x, CamCoords.y, CamCoords.z, 0, 0, 0, TRUE);

			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), FALSE, FALSE);
			CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 700, TRUE, TRUE, NULL);
			CAM::SET_CAM_ACTIVE(Freecam, TRUE);
			CAM::SET_CAM_ROT(Freecam, GameplayRotation.x, GameplayRotation.y, GameplayRotation.z, 0);

			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), TRUE);
			PED::SET_PED_CAN_SWITCH_WEAPON(PLAYER::PLAYER_PED_ID(), FALSE);
			WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), Joaat("weapon_unarmed"), TRUE);
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();

			while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(Dict))
			{
				STREAMING::REQUEST_NAMED_PTFX_ASSET(Dict);
				Script::Current()->Yield(5ms);
			}

			GRAPHICS::USE_PARTICLE_FX_ASSET(Dict);
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD(Particle, FinalResult.x, FinalResult.y, FinalResult.z, 0.f, 0.f, 0.f, 3.f, 0, 0, 0);

			// Faster
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_SPRINT, TRUE);
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_SPRINT))
				g_CamSpeed = 10.f;
			else
				g_CamSpeed = 5.f;

			// Forward
			if (PAD::IS_CONTROL_PRESSED(0, INPUT_MOVE_UP_ONLY))
				CAM::SET_CAM_COORD(Freecam, FinalResult.x, FinalResult.y, FinalResult.z);

			// Backward
			if (PAD::IS_CONTROL_PRESSED(0, INPUT_MOVE_DOWN_ONLY))
			{
				g_CamSpeed /= -1;
				CAM::SET_CAM_COORD(Freecam, FinalResult.x, FinalResult.y, FinalResult.z);
			}
		}
		// Proper Cleanup
		else if (CAM::DOES_CAM_EXIST(Freecam))
		{
			ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), CamCoords.x, CamCoords.y, CamCoords.z, iCoord.x, iCoord.y, iCoord.z, TRUE);
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), FALSE);
			PED::SET_PED_CAN_SWITCH_WEAPON(PLAYER::PLAYER_PED_ID(), TRUE);
			CAM::SET_CAM_ACTIVE(Freecam, FALSE);
			CAM::DESTROY_ALL_CAMS(Freecam);
			CAM::RENDER_SCRIPT_CAMS(FALSE, FALSE, 3000, TRUE, FALSE, NULL);
		}
	}

	void SelfFeatures::LazerEyes(bool enable)
	{
		if (enable)
		{
			const auto Coords = Weapons::GetAimCoords();
			const auto MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);

			PAD::DISABLE_CONTROL_ACTION(0, INPUT_ATTACK, TRUE);
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_ATTACK) && !Math::Empty(Coords))
			{
				if (Math::DistanceBetweenVectors(Coords, MyCoords) < 500.f && Math::DistanceBetweenVectors(Coords, MyCoords) >= 1.f)
				{
					FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, eExplosionTag::BULLET, 1000.f, TRUE, FALSE, 0.f, FALSE);
				}
			}
		}
	}

	void PlayForcePushSound()
	{
		switch (Math::RandomNumberInRange(1, 5))
		{
		case 1:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Push\\ForcePush1.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 2:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Push\\ForcePush2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 3:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Push\\ForcePush3.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 4:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Push\\ForcePush4.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 5:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Push\\ForcePush5.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		}
	}

	void PlayForcePullSound()
	{
		switch (Math::RandomNumberInRange(1, 6))
		{
		case 1:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Pull\\ForcePull1.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 2:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Pull\\ForcePull2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 3:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Pull\\ForcePull3.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 4:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Pull\\ForcePull4.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 5:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Pull\\ForcePull5.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 6:
			PlaySound(TEXT("C:\\Users\\matte\\Documents\\Sentinel\\FX\\Force_Pull\\ForcePull6.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		}
	}

	void SelfFeatures::ForcePush(bool enable)
	{
		if (enable)
		{
			const auto MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
			const auto Pitch = Math::DegreeToRadian(Rotation.x);
			const auto Yaw = Math::DegreeToRadian(Rotation.z + 90.f);
			static Timer pushTimer(0ms);
			pushTimer.SetDelay(std::chrono::seconds(2));
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_ATTACK, TRUE);
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_ATTACK) && pushTimer.Update())
			{
				for (const auto Entity : Entities::GetEntities(true, true, true))
				{
					const auto Coords = ENTITY::GET_ENTITY_COORDS(Entity, TRUE);
					if (Math::DistanceBetweenVectors(Coords, MyCoords) < 500.f && Math::DistanceBetweenVectors(Coords, MyCoords) >= 1.f)
					{
						if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(PLAYER::PLAYER_PED_ID(), Entity))
						{
							NativeVector3 Velocity{};

							Velocity.x = MyCoords.x + (1000 * cos(Pitch) * cos(Yaw)) - Coords.x;
							Velocity.y = MyCoords.y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.y;
							Velocity.z = MyCoords.z + (1000 * sin(Pitch)) - Coords.z;

							//PlayForcePushSound();
							ENTITY::SET_ENTITY_VELOCITY(Entity, Velocity.x, Velocity.y, Velocity.z);
						}
					}
				}
			}
		}
	}

	void SelfFeatures::ForcePull(bool enable)
	{
		if (enable)
		{
			const auto MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
			const auto Pitch = Math::DegreeToRadian(Rotation.x);
			const auto Yaw = Math::DegreeToRadian(Rotation.z + 90.f);
			static Timer pullTimer(0ms);
			pullTimer.SetDelay(std::chrono::seconds(2));
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_ATTACK, TRUE);
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_ATTACK) && pullTimer.Update())
			{
				for (const auto Entity : Entities::GetEntities(true, true, true))
				{
					const auto Coords = ENTITY::GET_ENTITY_COORDS(Entity, TRUE);
					if (Math::DistanceBetweenVectors(Coords, MyCoords) < 500.f && Math::DistanceBetweenVectors(Coords, MyCoords) >= 1.f)
					{
						if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(PLAYER::PLAYER_PED_ID(), Entity))
						{
							NativeVector3 Velocity{};

							Velocity.x = MyCoords.x + (1000 * cos(Pitch) * cos(Yaw)) - Coords.x;
							Velocity.y = MyCoords.y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.y;
							Velocity.z = MyCoords.z + (1000 * sin(Pitch)) - Coords.z;

							//PlayForcePullSound();
							ENTITY::SET_ENTITY_VELOCITY(Entity, -(Velocity.x), -(Velocity.y), -(Velocity.z));
						}
					}
				}
			}
		}
	}

	void SelfFeatures::Forcefield(bool enable)
	{
		if (enable)
		{
			const auto MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
			const auto Pitch = Math::DegreeToRadian(Rotation.x);
			const auto Yaw = Math::DegreeToRadian(Rotation.z + 90.f);
			if (m_ForcefieldSphere)
				GRAPHICS::DRAW_MARKER_SPHERE(MyCoords.x, MyCoords.y, MyCoords.z, m_ForcefieldRange, 99, 66, 245, 100 / 255.f);
			for (const auto Entity : Entities::GetEntities(m_ForcefieldVehicles, m_ForcefieldPeds, false))
			{
				const auto Coords = ENTITY::GET_ENTITY_COORDS(Entity, TRUE);
				if (Math::DistanceBetweenVectors(Coords, MyCoords) < m_ForcefieldRange && Math::DistanceBetweenVectors(Coords, MyCoords) >= 1.f)
				{
					NativeVector3 Velocity{};

					Velocity.x = MyCoords.x + (1000 * cos(Pitch) * cos(Yaw)) - Coords.x;
					Velocity.y = MyCoords.y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.y;
					Velocity.z = MyCoords.z + (1000 * sin(Pitch)) - Coords.z;

					if (*Pointers::pIsSessionStarted)
						ENTITY::IS_ENTITY_A_PED(Entity) ? Entities::RequestControl(Entity) : Entities::ChangeNetworkObjectOwner(Entity, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
					ENTITY::SET_ENTITY_VELOCITY(Entity, Velocity.x, Velocity.y, Velocity.z);
				}
			}
		}
	}

	void SelfFeatures::Blackout(bool enable)
	{
		if (enable) {
			GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(TRUE);
			GRAPHICS::SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(TRUE);
		}
	}

	void SelfFeatures::ThermalVision(bool enable)
	{
		if (enable) {
			*ScriptGlobal(1853910).At(PLAYER::PLAYER_ID(), 862).Add(821).Add(11).As<int*>() = -1;
			GRAPHICS::SET_SEETHROUGH(TRUE);
		}
	}

	void SelfFeatures::NightVision(bool enable)
	{
		if (enable) {
			*ScriptGlobal(1853910).At(PLAYER::PLAYER_ID(), 862).Add(821).Add(11).As<int*>() = -1;
			GRAPHICS::SET_NIGHTVISION(TRUE);
		}
	}
}