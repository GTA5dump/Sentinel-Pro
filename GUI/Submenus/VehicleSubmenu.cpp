#include "../../Common.hpp"
#include "Submenus.hpp"
#include "../GUI.hpp"
#include "../Overlays/Overlays.hpp"
#include "../Elements.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../GTA/GTA.hpp"
#include "../../GTA/enums.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../GTA/Script/ScriptQueue.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/Script/ScriptLocal.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../Features/Vehicle.hpp"
#include "../../Utility/All.hpp"

namespace Sentinel
{
	void Submenus::VehicleSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###vehicle", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Health", ImVec2(290.f, 205.f), ImVec2(0.f, 0.f));
			if (Elements::BoolOption("God Mode", &g_VehicleFeatures->m_GodMode, ImVec2(10.f, 30.f)))
			{
				if (!g_VehicleFeatures->m_GodMode)
					GTA::Offsets::SetValue<bool>({ 0x8, 0xD10, 0x189 }, false);
			}
			if (Elements::BoolOption("Render Burnt", &g_VehicleFeatures->m_RenderBurnt, ImVec2(145.f, 30.f)))
			{
				if (!g_VehicleFeatures->m_RenderBurnt)
					ENTITY::SET_ENTITY_RENDER_SCORCHED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
			}
			Elements::BoolOption("Auto Repair", &g_VehicleFeatures->m_AutoRepair, ImVec2(10.f, 60.f));
			Elements::BoolOption("Auto Wash", &g_VehicleFeatures->m_AutoWash, ImVec2(145.f, 60.f));
			Elements::BoolOption("Auto Flip", &g_VehicleFeatures->m_AutoFlip, ImVec2(10.f, 90.f));
			Elements::BoolOption("Anti Lockon", &g_VehicleFeatures->m_AntiLockon, ImVec2(145.f, 90.f));
			Elements::BoolOption("Anti Sticky Bombs", &g_VehicleFeatures->m_AntiStickyBomb, ImVec2(10.f, 120.f));
			Elements::BoolOption("Engine Always On", &g_VehicleFeatures->m_EngineAlwaysOn, ImVec2(145.f, 120.f));
			if (Elements::RegularOption("Dirty Vehicle", ImVec2(130.f, 25.f), ImVec2(10.f, 150.f))) {
				g_ScriptQueue->Queue([] 
				{ 
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 15.f); 
				});
			}
			if (Elements::RegularOption("Wash Vehicle", ImVec2(130.f, 25.f), ImVec2(145.f, 150.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 0.f);
				});
			}
			if (Elements::RegularOption("Fix Vehicle", ImVec2(130.f, 25.f), ImVec2(10.f, 180.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						if (VEHICLE::GET_DOES_VEHICLE_HAVE_DAMAGE_DECALS(Vehicle))
							VEHICLE::SET_VEHICLE_FIXED(Vehicle);

						VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);

						if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(Vehicle) > 1000.f)
							PED::IS_PED_IN_ANY_PLANE(PLAYER::PLAYER_PED_ID()) ? VEHICLE::SET_PLANE_ENGINE_HEALTH(Vehicle, 1000.f) : VEHICLE::SET_VEHICLE_ENGINE_HEALTH(Vehicle, 1000.f);
					}
				});
			}
			if (Elements::RegularOption("Destroy Vehicle", ImVec2(130.f, 25.f), ImVec2(145.f, 180.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						PED::IS_PED_IN_ANY_PLANE(PLAYER::PLAYER_PED_ID()) ? VEHICLE::SET_PLANE_ENGINE_HEALTH(Vehicle, -2000.f) : VEHICLE::SET_VEHICLE_ENGINE_HEALTH(Vehicle, -2000.f);
					}
				});
			}

			Elements::SubOption("Collision", ImVec2(290.f, 145.f), ImVec2(300.f, 0.f));
			if (Elements::BoolOption("Disable Full Collision", &g_VehicleFeatures->m_DisableFullCollision, ImVec2(310.f, 30.f)))
			{
				if (!g_VehicleFeatures->m_DisableFullCollision) 
				{
					g_ScriptQueue->Queue([]
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
							ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE, TRUE);
					});
				}
			}
			Elements::BoolOption("Disable Collision On Vehicles", &g_VehicleFeatures->m_DisableCollisionWithVehicles, ImVec2(310.f, 60.f));
			Elements::BoolOption("Disable Collision On Peds", &g_VehicleFeatures->m_DisableCollisionWithPeds, ImVec2(310.f, 90.f));
			Elements::BoolOption("Disable Collision On Objects", &g_VehicleFeatures->m_DisableCollisionWithObjects, ImVec2(310.f, 120.f));

			Elements::SubOption("Movement", ImVec2(290.f, 295.f), ImVec2(0.f, 230.f));
			Elements::BoolOption("Noclip", &g_VehicleFeatures->m_NoClip, ImVec2(10.f, 260.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("It follows your game camera.\nKeybinds:\nForward: Go Forwards\nBackward: Go Backwards\nSprint: Go Faster\nDuck: Go Slower");
			Elements::SliderFloatOption("###noclip_speed", &g_VehicleFeatures->m_NoClipSpeed, 1.f, 10.f, 155.f, ImVec2(102.5f, 263.f));
			Elements::BoolOption("Vehicle Fly", &g_VehicleFeatures->m_VehicleFly, ImVec2(10.f, 290.f));
			Elements::BoolOption("Hydraulics", &g_VehicleFeatures->m_Hydraulics, ImVec2(145.f, 290.f));
			if (Elements::BoolOption("Drift Mode", &g_VehicleFeatures->m_DriftMode, ImVec2(10.f, 320.f)))
			{
				if (!g_VehicleFeatures->m_DriftMode)
				{
					g_ScriptQueue->Queue([]
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
							VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
					});
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Use SHIFT/SPRINT to activate");
			Elements::BoolOption("Tumble Dryer", &g_VehicleFeatures->m_TumbleDryer, ImVec2(145.f, 320.f));
			Elements::BoolOption("Sticky Wheels", &g_VehicleFeatures->m_StickyTires, ImVec2(10.f, 350.f));
			if (Elements::BoolOption("Slippy Wheels", &g_VehicleFeatures->m_SlippyWheels, ImVec2(145.f, 350.f)))
			{
				if (!g_VehicleFeatures->m_SlippyWheels)
				{
					g_ScriptQueue->Queue([]
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
							VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
					});
				}
			}
			if (Elements::BoolOption("No Gravity", &g_VehicleFeatures->m_NoGravity, ImVec2(10.f, 380.f)))
			{
				if (!g_VehicleFeatures->m_NoGravity)
				{
					g_ScriptQueue->Queue([]
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
							VEHICLE::SET_VEHICLE_GRAVITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE);
					});
				}
			}
			if (Elements::BoolOption("Super Grip", &g_VehicleFeatures->m_SuperGrip, ImVec2(145.f, 380.f)))
			{
				if (g_VehicleFeatures->m_SuperGrip)
					*(std::float_t*)(Pointers::pSuperVehicleGrip) = 0.50f;
				else
					*(std::float_t*)(Pointers::pSuperVehicleGrip) = 0.16666667f;
			}
			Elements::BoolOption("Drive On Water", &g_VehicleFeatures->m_DriveOnWater, ImVec2(10.f, 410.f));
			if (Elements::BoolOption("Drive Through Walls", &g_VehicleFeatures->m_DriveThroughWalls, ImVec2(145.f, 410.f)))
			{
				g_ScriptQueue->Queue([]
				{
					if (g_VehicleFeatures->m_DriveThroughWalls)
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
							ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE, TRUE);
					}
					else
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
							ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), TRUE, TRUE);
					}
				});
			}
			Elements::BoolOption("Drive On Air", &g_VehicleFeatures->m_DriveOnAir, ImVec2(10.f, 440.f));
			if (Elements::BoolOption("Bypass Max Speed", &g_VehicleFeatures->m_BypassMaxSpeed, ImVec2(145.f, 440.f)))
			{
				g_ScriptQueue->Queue([]
				{
					if (!g_VehicleFeatures->m_BypassMaxSpeed && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::SET_VEHICLE_MAX_SPEED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 0.f);
				});
			}
			Elements::BoolOption("No Turbulence", &g_VehicleFeatures->m_NoTurbulence, ImVec2(10.f, 470.f));
			Elements::BoolOption("Hand Brake", &g_VehicleFeatures->m_HandBrake, ImVec2(145.f, 470.f));
			Elements::BoolOption("Instantly Enter", &g_VehicleFeatures->m_InstantlyEnter, ImVec2(10.f, 500.f));
			Elements::BoolOption("Instantly Exit", &g_VehicleFeatures->m_InstantlyExit, ImVec2(145.f, 500.f));

			Elements::SubOption("Particle Trails", ImVec2(290.f, 265.f), ImVec2(300.f, 170.f));
			ImGui::SetCursorPos(ImVec2(310.f, 200.f));
			ImGui::Text("Asset");
			ImGui::SetCursorPos(ImVec2(398.f, 200.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###particle_trail_asset", &g_VehicleFeatures->m_VehicleParticlesPos, g_VehicleFeatures->m_VehicleParticles, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleParticles), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Chassis", &g_VehicleFeatures->m_ActiveVehicleBones[0], ImVec2(310.f, 230.f));
			Elements::BoolOption("Front Left Wheel", &g_VehicleFeatures->m_ActiveVehicleBones[1], ImVec2(445.f, 230.f));
			Elements::BoolOption("Front Right Wheel", &g_VehicleFeatures->m_ActiveVehicleBones[2], ImVec2(310.f, 260.f));
			Elements::BoolOption("Rear Left Wheel", &g_VehicleFeatures->m_ActiveVehicleBones[3], ImVec2(445.f, 260.f));
			Elements::BoolOption("Rear Right Wheel", &g_VehicleFeatures->m_ActiveVehicleBones[4], ImVec2(310.f, 290.f));
			Elements::BoolOption("Left Headlight", &g_VehicleFeatures->m_ActiveVehicleBones[5], ImVec2(445.f, 290.f));
			Elements::BoolOption("Right Headlight", &g_VehicleFeatures->m_ActiveVehicleBones[6], ImVec2(310.f, 320.f));
			Elements::BoolOption("Left Taillight", &g_VehicleFeatures->m_ActiveVehicleBones[7], ImVec2(445.f, 320.f));
			Elements::BoolOption("Right Taillight", &g_VehicleFeatures->m_ActiveVehicleBones[8], ImVec2(310.f, 350.f));
			ImGui::SetCursorPos(ImVec2(310.f, 380.f));
			ImGui::Text("Scale");
			Elements::SliderFloatOption("###particle_trail_scale", &g_VehicleFeatures->m_ParticleTrailScale, 0.f, 10.f, 180.f, ImVec2(390.f, 380.f));
			Elements::BoolOption("Play Stationary", &g_VehicleFeatures->m_ParticleTrailPlayStationary, ImVec2(310.f, 410.f));
			Elements::BoolOption("Enable##1", &g_VehicleFeatures->m_EnableParticleTrail, ImVec2(445.f, 410.f));

			Elements::SubOption("Boost", ImVec2(290.f, 145.f), ImVec2(0.f, 550.f));
			ImGui::SetCursorPos(ImVec2(10.f, 580.f));
			ImGui::Text("Speed");
			Elements::SliderIntOption("###boost_speed", &g_VehicleFeatures->m_BoostSpeed, 0, 5000, 180.f, ImVec2(90.f, 580.f));
			Elements::BoolOption("Screen Effect", &g_VehicleFeatures->m_BoostEffect, ImVec2(10.f, 610.f));
			Elements::BoolOption("Audio Effect", &g_VehicleFeatures->m_BoostSound, ImVec2(145.f, 610.f));
			Elements::BoolOption("Boost Forwards", &g_VehicleFeatures->m_BoostForwards, ImVec2(10.f, 640.f));
			Elements::BoolOption("Boost Backwards", &g_VehicleFeatures->m_BoostBackwards, ImVec2(145.f, 640.f));
			Elements::BoolOption("Enable##2", &g_VehicleFeatures->m_EnableBoost, ImVec2(10.f, 670.f));

			Elements::SubOption("Vehicle Weapons", ImVec2(290.f, 175.f), ImVec2(300.f, 460.f));
			ImGui::SetCursorPos(ImVec2(310.f, 490.f));
			ImGui::Text("Bullet");
			ImGui::SetCursorPos(ImVec2(398.f, 490.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###vehicle_weapon_bullet", &g_VehicleFeatures->m_VehicleWeaponBulletPos, g_VehicleFeatures->m_VehicleWeaponBullet, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleWeaponBullet), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(310.f, 520.f));
			ImGui::Text("Direction");
			ImGui::SetCursorPos(ImVec2(398.f, 520.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###vehicle_weapon_direction", &g_VehicleFeatures->m_VehicleWeaponDirectionPos, g_VehicleFeatures->m_VehicleWeaponDirection, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleWeaponDirection), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(310.f, 550.f));
			ImGui::Text("Speed");
			Elements::SliderFloatOption("###vehicle_weapon_speed", &g_VehicleFeatures->m_VehicleWeaponSpeed, 0.f, 10000.f, 180.f, ImVec2(390.f, 550.f));
			Elements::BoolOption("Audible", &g_VehicleFeatures->m_VehicleWeaponAudible, ImVec2(310.f, 580.f));
			Elements::BoolOption("Visible", &g_VehicleFeatures->m_VehicleWeaponVisible, ImVec2(445.f, 580.f));
			Elements::BoolOption("View Direction", &g_VehicleFeatures->m_ViewVehicleWeaponDirection, ImVec2(310.f, 610.f));
			Elements::BoolOption("Enable##3", &g_VehicleFeatures->m_EnableVehicleWeapon, ImVec2(445.f, 610.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Press your horn to fire");

			Elements::SubOption("Acrobatics", ImVec2(290.f, 75.f), ImVec2(0.f, 720.f));
			ImGui::SetCursorPos(ImVec2(10.f, 750.f));
			ImGui::Text("Acrobatic");
			ImGui::SetCursorPos(ImVec2(98.f, 750.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###vehicle_acrobatics", &g_VehicleFeatures->m_VehicleAcrobaticsPos, g_VehicleFeatures->m_VehicleAcrobatics, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleAcrobatics), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Activate", ImVec2(130.f, 25.f), ImVec2(10.f, 780.f))) {
				g_ScriptQueue->Queue([]
				{
					const auto FirstVector = std::get<0>(Lists::g_VehicleAcrobatics[g_VehicleFeatures->m_VehicleAcrobaticsPos]);
					const auto SecondVector = std::get<1>(Lists::g_VehicleAcrobatics[g_VehicleFeatures->m_VehicleAcrobaticsPos]);
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 1, FirstVector.x, FirstVector.y, FirstVector.z, SecondVector.x, SecondVector.y, SecondVector.z, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
				});
			}
			Elements::BoolOption("On Horn", &g_VehicleFeatures->m_AcrobaticOnHorn, ImVec2(145.f, 780.f));

			Elements::SubOption("Ramps", ImVec2(290.f, 115.f), ImVec2(300.f, 660.f));
			ImGui::SetCursorPos(ImVec2(310.f, 690.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(398.f, 690.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###vehicle_ramp_types", &g_VehicleFeatures->m_VehicleRampTypesPos, g_VehicleFeatures->m_VehicleRampTypes, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleRampTypes), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(310.f, 720.f));
			ImGui::Text("Location");
			ImGui::SetCursorPos(ImVec2(398.f, 720.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###vehicle_ramp_location", &g_VehicleFeatures->m_VehicleRampLocationPos, g_VehicleFeatures->m_VehicleRampLocation, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleRampLocation), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Transparent", &g_VehicleFeatures->m_TransparentRamps, ImVec2(310.f, 750.f));
			if (Elements::RegularOption("Attach", ImVec2(80.f, 25.f), ImVec2(410.f, 750.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						char Buffer[50];
						sprintf_s(Buffer, "PROP_MP_RAMP_0%i", (g_VehicleFeatures->m_VehicleRampTypesPos + 1));
						Local::CreateAttachedRamp(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), Joaat(Buffer), g_VehicleFeatures->m_VehicleRampLocationPos, g_VehicleFeatures->m_TransparentRamps);
					}
				});
			}
			if (Elements::RegularOption("Detach All", ImVec2(80.f, 25.f), ImVec2(500.f, 750.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						const auto Coords = ENTITY::GET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), FALSE);
						std::uint32_t Ramps[] = { 0xB157C9E4, 0xF4F1511E, 0x93948E5E };
						for (int Iterator = 0; Iterator < 3; Iterator++) {
							int Counter = 0;
						search:
							auto Ramp = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(Coords.x, Coords.y, Coords.z, 20.f, Ramps[Iterator], FALSE, FALSE, FALSE);
							if (Ramp) 
							{
								if (ENTITY::DOES_ENTITY_EXIST(Ramp) && ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(Ramp, PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE))) 
								{
									if (*Pointers::pIsSessionStarted)
										Entities::ChangeNetworkObjectOwner(Ramp, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));

									ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Ramp, TRUE, TRUE);
									ENTITY::DELETE_ENTITY(&Ramp);

									Counter++;

									if (Counter < 20) {
										goto search;
									}
								}
							}
						}
					}
				});
			}

			Elements::SubOption("Miscellaneous", ImVec2(290.f, 285.f), ImVec2(0.f, 820.f));
			if (Elements::RegularOption("Formula 1 Wheels", ImVec2(130.f, 25.f), ImVec2(10.f, 850.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
						VEHICLE::SET_VEHICLE_WHEEL_TYPE(Vehicle, 10);
						VEHICLE::SET_VEHICLE_MOD(Vehicle, 23, 1, FALSE);
					}
				});
			}
			if (Elements::RegularOption("Delete Vehicle", ImVec2(130.f, 25.f), ImVec2(145.f, 850.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						// Get control + kick all retards in it
						if (*Pointers::pIsSessionStarted)
							Entities::ChangeNetworkObjectOwner(Vehicle, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
						Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Vehicle, TRUE, TRUE);
						ENTITY::DELETE_ENTITY(&Vehicle);
					}
				});
			}
			if (Elements::RegularOption("Clone Vehicle", ImVec2(130.f, 25.f), ImVec2(10.f, 880.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						Local::CloneVehicle(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE));
					}
				});
			}
			if (Elements::RegularOption("Flip Vehicle", ImVec2(130.f, 25.f), ImVec2(145.f, 880.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) 
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), 5.f);
				});
			}
			if (Elements::RegularOption("Max Performance", ImVec2(130.f, 25.f), ImVec2(10.f, 910.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						// Performance: brakes, engine, armor, transmission, suspension, turbo, bullet proof tires
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
						VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_BRAKES, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_BRAKES) - 1, FALSE);
						VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_ENGINE, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_ENGINE) - 1, FALSE);
						VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_ARMOR, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_ARMOR) - 1, FALSE);
						VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_TRANSMISSION, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_TRANSMISSION) - 1, FALSE);
						VEHICLE::SET_VEHICLE_MOD(Vehicle, MOD_SUSPENSION, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, MOD_SUSPENSION) - 1, FALSE);
						VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, MOD_TURBO, TRUE);
						VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, FALSE);
					}
				});
			}
			if (Elements::RegularOption("Low Performance", ImVec2(130.f, 25.f), ImVec2(145.f, 910.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						// Performance: brakes, engine, armor, transmission, suspension, turbo, bullet proof tires
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_BRAKES);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_ENGINE);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_ARMOR);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_TRANSMISSION);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_SUSPENSION);
						VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, MOD_TURBO, FALSE);
						VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, TRUE);
					}
				});
			}
			if (Elements::RegularOption("Max Upgrades", ImVec2(130.f, 25.f), ImVec2(10.f, 940.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
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
				});
			}
			if (Elements::RegularOption("Low Upgrades", ImVec2(130.f, 25.f), ImVec2(145.f, 940.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						VEHICLE::SET_VEHICLE_FIXED(Vehicle);
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0.f);
						VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);

						for (int Iterator = MOD_SPOILER; Iterator <= MOD_ARMOR; Iterator++) {
							VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, Iterator);
						}

						for (int Iterator = MOD_PLATEHOLDER; Iterator <= MOD_LIVERY; Iterator++) {
							VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, Iterator);
						}

						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_HORNS);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_FRONTWHEELS);

						for (int Iterator = MOD_UNK17; Iterator <= MOD_XENONLIGHTS; Iterator++) {
							VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, Iterator, FALSE);
						}

						VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle);
						VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle);
						VEHICLE::SET_VEHICLE_COLOURS(Vehicle, 0, 0);
						VEHICLE::SET_VEHICLE_EXTRA_COLOURS(Vehicle, 0, 0);
						VEHICLE::SET_VEHICLE_WINDOW_TINT(Vehicle, 0);
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle, 1);
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, "SENTINEL");

						for (int Iterator = 0; Iterator < 4; Iterator++) {
							VEHICLE::SET_VEHICLE_NEON_ENABLED(Vehicle, Iterator, FALSE);
						}

						// Performance: brakes, engine, armor, transmission, suspension, turbo, bullet proof tires
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_BRAKES);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_ENGINE);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_ARMOR);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_TRANSMISSION);
						VEHICLE::REMOVE_VEHICLE_MOD(Vehicle, MOD_SUSPENSION);
						VEHICLE::TOGGLE_VEHICLE_MOD(Vehicle, MOD_TURBO, FALSE);
						VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, TRUE);
					}
				});
			}
			Elements::BoolOption("Infinite Boost", &g_VehicleFeatures->m_InfiniteBoost, ImVec2(10.f, 970.f));
			Elements::BoolOption("Allow Boost", &g_VehicleFeatures->m_AllowBoost, ImVec2(145.f, 970.f));
			Elements::BoolOption("Allow Jumping", &g_VehicleFeatures->m_AllowJumping, ImVec2(10.f, 1000.f));
			Elements::BoolOption("Allow Parachute", &g_VehicleFeatures->m_AllowParachute, ImVec2(145.f, 1000.f));
			Elements::BoolOption("Infinite Missiles", &g_VehicleFeatures->m_InfiniteMissiles, ImVec2(10.f, 1030.f));
			Elements::BoolOption("Infinite Bombs", &g_VehicleFeatures->m_InfiniteBombs, ImVec2(145.f, 1030.f));
			Elements::BoolOption("Infinite Counter Measures", &g_VehicleFeatures->m_InfiniteCounterMeasures, ImVec2(10.f, 1060.f));
			Elements::BoolOption("Indicator Lights", &g_VehicleFeatures->m_IndicatorLights, ImVec2(10.f, 1090.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Use your turn signals\nArrow Left: Left Signal\nArrow Right: Right Signal\nArrow Down: Both Signals");
			Elements::BoolOption("Mute Siren", &g_VehicleFeatures->m_MuteSiren, ImVec2(145.f, 1090.f));

			Elements::SubOption("Speedometer", ImVec2(290.f, 75.f), ImVec2(300.f, 800.f));
			ImGui::SetCursorPos(ImVec2(310.f, 830.f));
			ImGui::Text("Metric");
			ImGui::SetCursorPos(ImVec2(398.f, 830.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###speedometer_metric", &g_VehicleFeatures->m_SpeedoMeterMetricPos, g_VehicleFeatures->m_SpeedoMeterMetric, IM_ARRAYSIZE(g_VehicleFeatures->m_SpeedoMeterMetric), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Numberplate", &g_VehicleFeatures->m_PlateSpeedo, ImVec2(310.f, 860.f));
			Elements::BoolOption("Onscreen", &g_VehicleFeatures->m_ScreenSpeedo, ImVec2(445.f, 860.f));

			Elements::SubOption("Visibility", ImVec2(290.f, 75.f), ImVec2(300.f, 900.f));
			Elements::BoolOption("Alpha", &g_VehicleFeatures->m_EnableVehicleAlpha, ImVec2(310.f, 930.f));
			ImGui::SetCursorPos(ImVec2(398.f, 930.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###vehicle_alpha_level", &g_VehicleFeatures->m_VehicleAlphaLevelPos, g_VehicleFeatures->m_VehicleAlphaLevel, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleAlphaLevel), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Invisibility", &g_VehicleFeatures->m_VehicleInvisibility, ImVec2(310.f, 960.f));

			Elements::SubOption("Randomization", ImVec2(290.f, 195.f), ImVec2(300.f, 1000.f));
			Elements::BoolOption("Rainbow Paint", &g_VehicleFeatures->m_EnableRainbowPaint, ImVec2(310.f, 1030.f));
			ImGui::SetCursorPos(ImVec2(458.f, 1030.f));
			ImGui::PushItemWidth(120.f);
			Elements::ComboOption("###rainbow_paint_mode", &g_VehicleFeatures->m_RainbowPaintModePos, g_VehicleFeatures->m_RainbowMode, IM_ARRAYSIZE(g_VehicleFeatures->m_RainbowMode), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Rainbow Tire Smoke", &g_VehicleFeatures->m_EnableRainbowTireSmoke, ImVec2(310.f, 1060.f));
			ImGui::SetCursorPos(ImVec2(458.f, 1060.f));
			ImGui::PushItemWidth(120.f);
			Elements::ComboOption("###rainbow_tire_smoke_mode", &g_VehicleFeatures->m_RainbowTireSmokeModePos, g_VehicleFeatures->m_RainbowMode, IM_ARRAYSIZE(g_VehicleFeatures->m_RainbowMode), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Rainbow Neons", &g_VehicleFeatures->m_EnableRainbowNeons, ImVec2(310.f, 1090.f));
			ImGui::SetCursorPos(ImVec2(458.f, 1090.f));
			ImGui::PushItemWidth(120.f);
			Elements::ComboOption("###rainbow_neons_mode", &g_VehicleFeatures->m_RainbowNeonsModePos, g_VehicleFeatures->m_RainbowMode, IM_ARRAYSIZE(g_VehicleFeatures->m_RainbowMode), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Rainbow Headlights", &g_VehicleFeatures->m_RainbowHeadlights, ImVec2(310.f, 1120.f));
			Elements::BoolOption("Random Modifications", &g_VehicleFeatures->m_RandomMods, ImVec2(310.f, 1150.f));
			Elements::BoolOption("Random Acrobatics", &g_VehicleFeatures->m_RandomAcrobatics, ImVec2(310.f, 1180.f));

			Elements::SubOption("Tire Matrix", ImVec2(290.f, 135.f), ImVec2(0.f, 1130.f));
			if (Elements::BoolOption("Width", &g_VehicleFeatures->m_EnableTireWidth, ImVec2(10.f, 1160.f)))
			{
				if (!g_VehicleFeatures->m_EnableTireWidth)
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed()->m_vehicle)
							return;

						const auto CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						Local::TireScaling::SetTireWidth(g_VehicleFeatures->GetTireWidthValue(CurrentVehicle));
					});
				}
			}
			Elements::SliderFloatOption("###tire_width", &g_VehicleFeatures->m_TireWidth, -10.f, 10.f, 155.f, ImVec2(102.5f, 1163.f));
			if (Elements::BoolOption("Height", &g_VehicleFeatures->m_EnableTireHeight, ImVec2(10.f, 1190.f)))
			{
				if (!g_VehicleFeatures->m_EnableTireHeight)
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed()->m_vehicle)
							return;

						const auto CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						Local::TireScaling::SetTireHeight(g_VehicleFeatures->GetTireHeightValue(CurrentVehicle));
					});
				}
			}
			Elements::SliderFloatOption("###tire_height", &g_VehicleFeatures->m_TireHeight, -10.f, 10.f, 155.f, ImVec2(102.5f, 1193.f));
			if (Elements::BoolOption("Scale", &g_VehicleFeatures->m_EnableTireScale, ImVec2(10.f, 1220.f)))
			{
				if (!g_VehicleFeatures->m_EnableTireScale)
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed()->m_vehicle)
							return;

						const auto CurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						Local::TireScaling::SetTireWidth(g_VehicleFeatures->GetTireScaleValue(CurrentVehicle).x);
						Local::TireScaling::SetTireHeight(g_VehicleFeatures->GetTireScaleValue(CurrentVehicle).y);
					});
				}
			}
			Elements::SliderFloatOption("###tire_scale", &g_VehicleFeatures->m_TireScale, -10.f, 10.f, 155.f, ImVec2(102.5f, 1223.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1250.f));
			ImGui::Text("You need custom tires for the scalars to work.");

			Elements::SubOption("Window Manager", ImVec2(290.f, 115.f), ImVec2(300.f, 1220.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1250.f));
			ImGui::Text("Target");
			ImGui::SetCursorPos(ImVec2(398.f, 1250.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###window_manager_target", &g_VehicleFeatures->m_VehicleWindowsPos, g_VehicleFeatures->m_VehicleWindows, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleWindows), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Smash", ImVec2(130.f, 25.f), ImVec2(310.f, 1280.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::SMASH_VEHICLE_WINDOW(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), g_VehicleFeatures->m_VehicleWindowsPos);
				});
			}
			if (Elements::RegularOption("Fix", ImVec2(130.f, 25.f), ImVec2(445.f, 1280.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::FIX_VEHICLE_WINDOW(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), g_VehicleFeatures->m_VehicleWindowsPos);
				});
			}
			if (Elements::RegularOption("Roll Up", ImVec2(130.f, 25.f), ImVec2(310.f, 1310.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::ROLL_UP_WINDOW(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), g_VehicleFeatures->m_VehicleWindowsPos);
				});
			}
			if (Elements::RegularOption("Roll Down", ImVec2(130.f, 25.f), ImVec2(445.f, 1310.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::ROLL_DOWN_WINDOW(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), g_VehicleFeatures->m_VehicleWindowsPos);
				});
			}

			Elements::SubOption("Engine Sound Overwrite", ImVec2(290.f, 75.f), ImVec2(0.f, 1290.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1320.f));
			ImGui::Text("Preset");
			ImGui::SetCursorPos(ImVec2(98.f, 1320.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###engine_audio_preset", &g_VehicleFeatures->m_EngineSoundsPos, g_VehicleFeatures->m_EngineSounds, IM_ARRAYSIZE(g_VehicleFeatures->m_EngineSounds), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Set", ImVec2(130.f, 25.f), ImVec2(10.f, 1350.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) 
						AUDIO::FORCE_USE_AUDIO_GAME_OBJECT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), Local::GetDisplayNameOfVehicleModel(Lists::g_VehicleEngines[g_VehicleFeatures->m_EngineSoundsPos]));
				});
			}
			if (Elements::RegularOption("Reset", ImVec2(130.f, 25.f), ImVec2(145.f, 1350.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						AUDIO::FORCE_USE_AUDIO_GAME_OBJECT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), Local::GetDisplayNameOfVehicle(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE)));
				});
			}

			Elements::SubOption("Seat Manager", ImVec2(290.f, 115.f), ImVec2(300.f, 1360.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1390.f));
			ImGui::Text("Seat");
			ImGui::SetCursorPos(ImVec2(398.f, 1390.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###seat_manager_seat", &g_VehicleFeatures->m_VehicleSeatsPos, g_VehicleFeatures->m_VehicleSeats, IM_ARRAYSIZE(g_VehicleFeatures->m_VehicleSeats), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Kick", ImVec2(130.f, 25.f), ImVec2(310.f, 1420.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						const auto SeatIndex = g_VehicleFeatures->m_VehicleSeatsPos - 1; // Due to the seats starting from -1 and the array starting from 0
						if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, SeatIndex, FALSE))
						{
							const auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, SeatIndex, FALSE);
							if (PED::IS_PED_A_PLAYER(Ped))
								Pointers::pClearPedTasksNetworked(g_PlayerService->GetByPed(Ped)->GetPed(), true);
							else
								Entities::ClearPedTasks(Ped);
						}
					}
				});
			}
			if (Elements::RegularOption("Enter", ImVec2(130.f, 25.f), ImVec2(445.f, 1420.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						const auto SeatIndex = g_VehicleFeatures->m_VehicleSeatsPos - 1; // Due to the seats starting from -1 and the array starting from 0
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, SeatIndex, FALSE))
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, SeatIndex);
					}
				});
			}
			if (Elements::RegularOption("Kick everyone", ImVec2(260.f, 25.f), ImVec2(310.f, 1450.f))) {
				g_ScriptQueue->Queue([]
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
					{
						const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
						for (int Iterator = -1; Iterator < 16; Iterator++)
						{
							if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, Iterator, FALSE))
							{
								const auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, Iterator, FALSE);
								if (PED::IS_PED_A_PLAYER(Ped))
									Pointers::pClearPedTasksNetworked(g_PlayerService->GetByPed(Ped)->GetPed(), true);
								else
									Entities::ClearPedTasks(Ped);
							}
						}
					}
				});
			}

			Elements::SubOption("Tire Tracks", ImVec2(290.f, 55.f), ImVec2(0.f, 1390.f));
			Elements::ColorEditOption("Color Picker", (float*)&g_VehicleFeatures->m_TireTracksColor, ImVec2(10.f, 1420.f));
			Elements::BoolOption("Rainbow", &g_VehicleFeatures->m_TireTracksRainbow, ImVec2(120.f, 1420.f));
			Elements::BoolOption("Enable##4", &g_VehicleFeatures->m_EnableTireTracks, ImVec2(205.f, 1420.f));

			ImGui::Dummy(ImVec2(0.f, 70.f));
			ImGui::Dummy(ImVec2(300.f, 70.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}