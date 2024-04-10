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
#include "../../Features/Weapon.hpp"
#include "../../Utility/Lists.hpp"

namespace Sentinel
{
	void Submenus::WeaponSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###weapon", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Modifiers", ImVec2(290.f, 175.f), ImVec2(0.f, 0.f));
			if (Elements::BoolOption("Infinite Ammo", &g_WeaponFeatures->m_InfiniteAmmo, ImVec2(10.f, 30.f)))
			{
				if (!g_WeaponFeatures->m_InfiniteAmmo)
					g_ScriptQueue->Queue([] { WEAPON::SET_PED_INFINITE_AMMO(PLAYER::PLAYER_PED_ID(), FALSE, NULL); });
			}
			if (Elements::BoolOption("Infinite Clip", &g_WeaponFeatures->m_InfiniteClip, ImVec2(145.f, 30.f)))
			{
				if (!g_WeaponFeatures->m_InfiniteClip)
					g_ScriptQueue->Queue([] { WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), FALSE); });
			}
			Elements::BoolOption("Explosive Ammo", &g_WeaponFeatures->m_ExplosiveAmmo, ImVec2(10.f, 60.f));
			Elements::BoolOption("Fire Ammo", &g_WeaponFeatures->m_FireAmmo, ImVec2(145.f, 60.f));
			if (Elements::BoolOption("No Recoil", &g_WeaponFeatures->m_NoRecoil, ImVec2(10.f, 90.f)))
			{
				if (!g_WeaponFeatures->m_NoRecoil)
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed())
							return;

						const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
						if (WeaponManager)
						{
							const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
							WeaponManager->m_weapon_info->m_explosion_shake_amplitude = g_WeaponFeatures->GetRecoilValue(CurrentWeapon);
						}
					});
				}
			}
			if (Elements::BoolOption("No Spread", &g_WeaponFeatures->m_NoSpread, ImVec2(145.f, 90.f)))
			{
				if (!g_WeaponFeatures->m_NoSpread) 
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed())
							return;

						const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
						if (WeaponManager)
						{
							const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
							WeaponManager->m_weapon_info->m_accuracy_spread = g_WeaponFeatures->GetSpreadValue(CurrentWeapon);
						}
					});
				}
			}

			if (Elements::BoolOption("Shotgun Mode", &g_WeaponFeatures->m_ShotGunMode, ImVec2(10.f, 120.f)))
			{
				if (!g_WeaponFeatures->m_ShotGunMode) 
				{
					g_ScriptQueue->Queue([]
					{
						if (!g_WeaponFeatures->m_ShotGunMode) {
							auto WeaponInfo = GTA::GetLocalPed()->m_weapon_manager->m_weapon_info;
							WeaponInfo->m_batch_spread = 0.1f;
							WeaponInfo->m_bullets_in_batch = 1;
							WeaponInfo->m_force = 200.f;
							WeaponInfo->m_speed = 5000.f;
						}
					});
				}
			}
			if (Elements::BoolOption("One Shot Kill", &g_WeaponFeatures->m_OneShotKill, ImVec2(145.f, 120.f)))
			{
				if (!g_WeaponFeatures->m_OneShotKill) 
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed())
							return;

						const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
						if (WeaponManager)
						{
							const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
							WeaponManager->m_weapon_info->m_damage = g_WeaponFeatures->GetDamageValue(CurrentWeapon);
						}
					});
				}
			}
			if (Elements::BoolOption("No Cooldown", &g_WeaponFeatures->m_NoCooldown, ImVec2(10.f, 150.f)))
			{
				if (!g_WeaponFeatures->m_NoCooldown)
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed())
							return;

						const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
						if (WeaponManager)
						{
							const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
							WeaponManager->m_weapon_info->m_time_between_shots = g_WeaponFeatures->GetCooldownValue(CurrentWeapon);
						}
					});
				}
			}
			if (Elements::BoolOption("No Spin-Up", &g_WeaponFeatures->m_NoSpinUp, ImVec2(145.f, 150.f)))
			{
				if (!g_WeaponFeatures->m_NoSpinUp)
				{
					g_ScriptQueue->Queue([]
					{
						if (!GTA::GetLocalPed())
							return;

						const auto* WeaponManager = GTA::GetLocalPed()->m_weapon_manager;
						if (WeaponManager)
						{
							const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
							WeaponManager->m_weapon_info->m_spinup_time = g_WeaponFeatures->GetSpinUpValue(CurrentWeapon);
						}
					});
				}
			}

			Elements::SubOption("Special Weapons", ImVec2(290.f, 355.f), ImVec2(300.f, 0.f));
			Elements::BoolOption("Portal Gun", &g_WeaponFeatures->m_PortalGun, ImVec2(310.f, 30.f));
			Elements::BoolOption("Teleport Gun", &g_WeaponFeatures->m_TeleportGun, ImVec2(445.f, 30.f));
			Elements::BoolOption("Skydive Gun", &g_WeaponFeatures->m_SkydiveGun, ImVec2(310.f, 60.f));
			Elements::BoolOption("Airstrike Gun", &g_WeaponFeatures->m_AirstrikeGun, ImVec2(445.f, 60.f));
			Elements::BoolOption("Push Gun", &g_WeaponFeatures->m_PushGun, ImVec2(310.f, 90.f));
			Elements::BoolOption("Pull Gun", &g_WeaponFeatures->m_PullGun, ImVec2(445.f, 90.f));
			Elements::BoolOption("Ped Gun", &g_WeaponFeatures->m_PedGun, ImVec2(310.f, 120.f));
			Elements::BoolOption("Rapid Fire", &g_WeaponFeatures->m_RapidFire, ImVec2(445.f, 120.f));
			Elements::BoolOption("Delete Gun", &g_WeaponFeatures->m_DeleteGun, ImVec2(310.f, 150.f));
			Elements::BoolOption("Hijack Gun", &g_WeaponFeatures->m_HijackGun, ImVec2(445.f, 150.f));
			Elements::BoolOption("Rope Gun", &g_WeaponFeatures->m_RopeGun, ImVec2(310.f, 180.f));
			Elements::BoolOption("Valkyrie Gun", &g_WeaponFeatures->m_ValkyrieGun, ImVec2(445.f, 180.f));
			if (Elements::BoolOption("Dead Eyes", &g_WeaponFeatures->m_DeadEyes, ImVec2(310.f, 210.f)))
			{
				if (!g_WeaponFeatures->m_DeadEyes)
				{
					g_ScriptQueue->Queue([]
					{
						MISC::SET_TIME_SCALE(1.f);
						GRAPHICS::ANIMPOSTFX_STOP("HeistLocate");
					});
				}
			}
			Elements::BoolOption("Laser Sight", &g_WeaponFeatures->m_LaserSight, ImVec2(445.f, 210.f));
			Elements::BoolOption("Defibrillator Gun", &g_WeaponFeatures->m_DefibrillatorGun, ImVec2(310.f, 240.f));
			Elements::BoolOption("Hash Gun", &g_WeaponFeatures->m_HashGun, ImVec2(445.f, 240.f));
			Elements::BoolOption("Shrink Gun", &g_WeaponFeatures->m_ShrinkGun, ImVec2(310.f, 270.f));
			Elements::BoolOption("Clown Gun", &g_WeaponFeatures->m_ClownGun, ImVec2(445.f, 270.f));
			Elements::BoolOption("Repair Gun", &g_WeaponFeatures->m_RepairGun, ImVec2(310.f, 300.f));
			Elements::BoolOption("Recolor Gun", &g_WeaponFeatures->m_RecolorGun, ImVec2(445.f, 300.f));
			Elements::BoolOption("Instrument Of Death", &g_WeaponFeatures->m_InstrumentOfDeath, ImVec2(310.f, 330.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("You must be unarmed.\nControls:\nAttack: Create Rocket\nAim: Move rocket");

			Elements::SubOption("Vortex Gun", ImVec2(290.f, 235.f), ImVec2(0.f, 200.f));
			Elements::BoolOption("Peds", &g_WeaponFeatures->m_VortexGunPeds, ImVec2(10.f, 230.f));
			Elements::SliderIntOption("###vortex_gun_ped_capacity", &g_WeaponFeatures->m_VortexGunPedCapacity, 5, 15, 180.f, ImVec2(90.f, 230.f));
			Elements::BoolOption("Vehicles", &g_WeaponFeatures->m_VortexGunVehicles, ImVec2(10.f, 260.f));
			Elements::SliderIntOption("###vortex_gun_vehicle_capacity", &g_WeaponFeatures->m_VortexGunVehicleCapacity, 5, 15, 180.f, ImVec2(90.f, 260.f));
			Elements::BoolOption("Objects", &g_WeaponFeatures->m_VortexGunObjects, ImVec2(10.f, 290.f));
			Elements::SliderIntOption("###vortex_gun_object_capacity", &g_WeaponFeatures->m_VortexGunObjectCapacity, 5, 15, 180.f, ImVec2(90.f, 290.f));
			ImGui::SetCursorPos(ImVec2(10.f, 320.f));
			ImGui::Text("Distance");
			Elements::SliderFloatOption("###vortex_gun_distance", &g_WeaponFeatures->m_VortexGunDistance, 10.f, 100.f, 180.f, ImVec2(90.f, 320.f));
			ImGui::SetCursorPos(ImVec2(10.f, 350.f));
			ImGui::Text("Force");
			Elements::SliderFloatOption("###vortex_gun_force", &g_WeaponFeatures->m_VortexGunForce, 100.f, 1000.f, 180.f, ImVec2(90.f, 350.f));
			Elements::BoolOption("Draw Sphere", &g_WeaponFeatures->m_VortexGunSphere, ImVec2(10.f, 380.f));
			Elements::BoolOption("Enable##1", &g_WeaponFeatures->m_VortexGunEnable, ImVec2(10.f, 410.f));

			Elements::SubOption("Totem Gun", ImVec2(290.f, 115.f), ImVec2(300.f, 380.f));
			ImGui::SetCursorPos(ImVec2(310.f, 410.f));
			ImGui::Text("Distance");
			Elements::SliderFloatOption("###totem_gun_distance", &g_WeaponFeatures->m_TotemGunDistance, 1.f, 100.f, 180.f, ImVec2(390.f, 410.f));
			ImGui::SetCursorPos(ImVec2(310.f, 440.f));
			ImGui::Text("Spacing");
			Elements::SliderFloatOption("###totem_gun_spacing", &g_WeaponFeatures->m_TotemGunSpacing, 1.f, 50.f, 180.f, ImVec2(390.f, 440.f));
			Elements::BoolOption("Enable##2", &g_WeaponFeatures->m_EnableTotemGun, ImVec2(310.f, 470.f));

			Elements::SubOption("Bullet Tracers", ImVec2(290.f, 75.f), ImVec2(0.f, 460.f));
			ImGui::SetCursorPos(ImVec2(10.f, 490.f));
			ImGui::Text("Duration");
			Elements::SliderIntOption("###bullet_tracer_duration", &g_WeaponFeatures->m_BulletTracerDuration, 250, 10000, 180.f, ImVec2(90.f, 490.f));
			Elements::ColorEditOption("Color Picker##1", (float*)&g_WeaponFeatures->m_BulletTracerColor, ImVec2(10.f, 520.f));
			Elements::BoolOption("Rainbow##1", &g_WeaponFeatures->m_RainbowBulletTracers, ImVec2(120.f, 520.f));
			Elements::BoolOption("Enable##3", &g_WeaponFeatures->m_EnableBulletTracers, ImVec2(205.f, 520.f));

			Elements::SubOption("Gravity Gun", ImVec2(290.f, 115.f), ImVec2(300.f, 520.f));
			ImGui::SetCursorPos(ImVec2(310.f, 550.f));
			ImGui::Text("Distance");
			Elements::SliderFloatOption("###gravity_gun_distance", &g_WeaponFeatures->m_GravityGunDistance, 1.f, 100.f, 180.f, ImVec2(390.f, 550.f));
			ImGui::SetCursorPos(ImVec2(310.f, 580.f));
			ImGui::Text("Force");
			Elements::SliderFloatOption("###gravity_gun_force", &g_WeaponFeatures->m_GravityGunForce, 100.f, 1000.f, 180.f, ImVec2(390.f, 580.f));
			Elements::BoolOption("Enable##4", &g_WeaponFeatures->m_EnableGravityGun, ImVec2(310.f, 610.f));

			Elements::SubOption("Bullet Changer", ImVec2(290.f, 115.f), ImVec2(0.f, 560.f));
			ImGui::SetCursorPos(ImVec2(10.f, 590.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(98.f, 590.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###bullet_changer_type", &g_WeaponFeatures->m_BulletChangerTypePos, g_WeaponFeatures->m_BulletChangerType, IM_ARRAYSIZE(g_WeaponFeatures->m_BulletChangerType), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(10.f, 620.f));
			ImGui::Text("Speed");
			Elements::SliderFloatOption("###bullet_changer_speed", &g_WeaponFeatures->m_BulletChangerSpeed, 1.f, 10000.f, 180.f, ImVec2(90.f, 620.f));
			Elements::BoolOption("Audible##1", &g_WeaponFeatures->m_BulletChangerAudible, ImVec2(10.f, 650.f));
			Elements::BoolOption("Visible##1", &g_WeaponFeatures->m_BulletChangerVisible, ImVec2(100.f, 650.f));
			Elements::BoolOption("Enable##5", &g_WeaponFeatures->m_EnableBulletChanger, ImVec2(190.f, 650.f));

			Elements::SubOption("Impact Gun", ImVec2(290.f, 175.f), ImVec2(300.f, 660.f));
			ImGui::SetCursorPos(ImVec2(310.f, 690.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(398.f, 690.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###impact_gun_type", &g_WeaponFeatures->m_ImpactGunTypePos, g_WeaponFeatures->m_ImpactGunType, IM_ARRAYSIZE(g_WeaponFeatures->m_ImpactGunType), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(310.f, 720.f));
			ImGui::Text("Damage");
			Elements::SliderFloatOption("###impact_gun_damage", &g_WeaponFeatures->m_ImpactGunDamageScale, 1.f, 9999.f, 180.f, ImVec2(390.f, 720.f));
			ImGui::SetCursorPos(ImVec2(310.f, 750.f));
			ImGui::Text("Camera Shake");
			Elements::SliderFloatOption("###impact_gun_camera_shake", &g_WeaponFeatures->m_ImpactGunCameraShake, 1.f, 9999.f, 180.f, ImVec2(390.f, 750.f));
			Elements::BoolOption("Audible##2", &g_WeaponFeatures->m_ImpactGunAudible, ImVec2(310.f, 780.f));
			Elements::BoolOption("Visible##2", &g_WeaponFeatures->m_ImpactGunVisible, ImVec2(445.f, 780.f));
			Elements::BoolOption("Enable##6", &g_WeaponFeatures->m_EnableImpactGun, ImVec2(310.f, 810.f));

			Elements::SubOption("Paint Gun", ImVec2(290.f, 175.f), ImVec2(0.f, 700.f));
			Elements::ColorEditOption("Color Picker##2", (float*)&g_WeaponFeatures->m_PaintGunColor, ImVec2(10.f, 730.f));
			Elements::BoolOption("Rainbow##2", &g_WeaponFeatures->m_RainbowPaintGun, ImVec2(145.f, 730.f));
			ImGui::SetCursorPos(ImVec2(10.f, 760.f));
			ImGui::Text("Size");
			Elements::SliderFloatOption("###paint_gun_size", &g_WeaponFeatures->m_PaintGunSize, 0.1f, 10.f, 180.f, ImVec2(90.f, 760.f));
			ImGui::SetCursorPos(ImVec2(10.f, 790.f));
			ImGui::Text("Intensity");
			Elements::SliderFloatOption("###paint_gun_intensity", &g_WeaponFeatures->m_PaintGunIntensity, 1000.f, 10000.f, 180.f, ImVec2(90.f, 790.f));
			ImGui::SetCursorPos(ImVec2(10.f, 820.f));
			ImGui::Text("Duration");
			Elements::SliderIntOption("###paint_gun_duration", &g_WeaponFeatures->m_PaintGunDuration, 250, 10000, 180.f, ImVec2(90.f, 820.f));
			Elements::BoolOption("Enable##7", &g_WeaponFeatures->m_EnablePaintGun, ImVec2(10.f, 850.f));

			Elements::SubOption("Particle Gun", ImVec2(290.f, 115.f), ImVec2(300.f, 860.f));
			ImGui::SetCursorPos(ImVec2(310.f, 890.f));
			ImGui::Text("Effect");
			ImGui::SetCursorPos(ImVec2(398.f, 890.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###particle_gun_effect", &g_WeaponFeatures->m_ParticleGunEffectPos, g_WeaponFeatures->m_ParticleGunEffect, IM_ARRAYSIZE(g_WeaponFeatures->m_ParticleGunEffect), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(310.f, 920.f));
			ImGui::Text("Scale");
			Elements::SliderFloatOption("###particle_gun_scale", &g_WeaponFeatures->m_ParticleGunScale, 0.1f, 10.f, 180.f, ImVec2(390.f, 920.f));
			Elements::BoolOption("Enable##8", &g_WeaponFeatures->m_EnableParticleGun, ImVec2(310.f, 950.f));

			Elements::SubOption("Entity Gun", ImVec2(290.f, 175.f), ImVec2(0.f, 900.f));
			ImGui::SetCursorPos(ImVec2(10.f, 930.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(98.f, 930.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###entity_gun_type", &g_WeaponFeatures->m_EntityGunTypePos, g_WeaponFeatures->m_EntityGunType, IM_ARRAYSIZE(g_WeaponFeatures->m_EntityGunType), -1);
			ImGui::PopItemWidth();
			switch (g_WeaponFeatures->m_EntityGunTypePos)
			{
			case 0:
				ImGui::SetCursorPos(ImVec2(10.f, 960.f));
				ImGui::Text("Object");
				ImGui::SetCursorPos(ImVec2(98.f, 960.f));
				ImGui::PushItemWidth(180.f);
				Elements::ComboOption("###entity_gun_object", &g_WeaponFeatures->m_EntityGunObjectPos, g_WeaponFeatures->m_EntityGunObject, IM_ARRAYSIZE(g_WeaponFeatures->m_EntityGunObject), -1);
				ImGui::PopItemWidth();
				break;
			case 1:
				ImGui::SetCursorPos(ImVec2(10.f, 960.f));
				ImGui::Text("Vehicle");
				ImGui::SetCursorPos(ImVec2(98.f, 960.f));
				ImGui::PushItemWidth(180.f);
				Elements::ComboOption("###entity_gun_vehicle", &g_WeaponFeatures->m_EntityGunVehiclePos, g_WeaponFeatures->m_EntityGunVehicle, IM_ARRAYSIZE(g_WeaponFeatures->m_EntityGunVehicle), -1);
				ImGui::PopItemWidth();
				break;
			case 2:
				ImGui::SetCursorPos(ImVec2(10.f, 960.f));
				ImGui::Text("Ped");
				ImGui::SetCursorPos(ImVec2(98.f, 960.f));
				ImGui::PushItemWidth(180.f);
				Elements::ComboOption("###entity_gun_ped", &g_WeaponFeatures->m_EntityGunPedPos, g_WeaponFeatures->m_EntityGunPed, IM_ARRAYSIZE(g_WeaponFeatures->m_EntityGunPed), -1);
				ImGui::PopItemWidth();
				break;
			}
			ImGui::SetCursorPos(ImVec2(10.f, 990.f));
			ImGui::Text("Delay");
			Elements::SliderIntOption("###entity_gun_delay", &g_WeaponFeatures->m_EntityGunDelay, 0, 10000, 180.f, ImVec2(90.f, 990.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1020.f));
			ImGui::Text("Force");
			Elements::SliderFloatOption("###entity_gun_force", &g_WeaponFeatures->m_EntityGunForce, 0.f, 10000.f, 180.f, ImVec2(90.f, 1020.f));
			Elements::BoolOption("Enable##9", &g_WeaponFeatures->m_EnableEntityGun, ImVec2(10.f, 1050.f));

			Elements::SubOption("Matrix", ImVec2(290.f, 175.f), ImVec2(300.f, 1000.f));
			if (Elements::BoolOption("Width", &g_WeaponFeatures->m_EnableWeaponWidth, ImVec2(310.f, 1030.f)))
			{
				if (!g_WeaponFeatures->m_EnableWeaponWidth)
					GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x7C }, 1.f);
			}
			Elements::SliderFloatOption("###weapon_width", &g_WeaponFeatures->m_WeaponWidth, -10.f, 10.f, 155.f, ImVec2(402.5f, 1033.f));
			if (Elements::BoolOption("Height", &g_WeaponFeatures->m_EnableWeaponHeight, ImVec2(310.f, 1060.f)))
			{
				if (!g_WeaponFeatures->m_EnableWeaponHeight)
					GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x8C }, 1.f);
			}
			Elements::SliderFloatOption("###weapon_height", &g_WeaponFeatures->m_WeaponHeight, -10.f, 10.f, 155.f, ImVec2(402.5f, 1063.f));
			if (Elements::BoolOption("Scale", &g_WeaponFeatures->m_EnableWeaponScale, ImVec2(310.f, 1090.f)))
			{
				if (!g_WeaponFeatures->m_EnableWeaponScale) {
					GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x7C }, 1.f);
					GTA::Offsets::SetValue<float>({ 0x8, 0x10B8, 0x78, 0x8C }, 1.f);
				}
			}
			Elements::SliderFloatOption("###weapon_scale", &g_WeaponFeatures->m_WeaponScale, -10.f, 10.f, 155.f, ImVec2(402.5f, 1093.f));
			if (Elements::BoolOption("Position-X", &g_WeaponFeatures->m_EnablePositionX, ImVec2(310.f, 1120.f)))
			{
				if (!g_WeaponFeatures->m_EnablePositionX)
					GTA::GetLocalPed()->m_transformation_matrix.data[0][7] = 1.f;
			}
			Elements::SliderFloatOption("###weapon_position_x", &g_WeaponFeatures->m_PositionX, -25.f, 25.f, 155.f, ImVec2(402.5f, 1123.f));
			if (Elements::BoolOption("Position-Y", &g_WeaponFeatures->m_EnablePositionY, ImVec2(310.f, 1150.f)))
			{
				if (!g_WeaponFeatures->m_EnablePositionX)
					GTA::GetLocalPed()->m_transformation_matrix.data[0][11] = 1.f;
			}
			Elements::SliderFloatOption("###weapon_position_y", &g_WeaponFeatures->m_PositionY, -25.f, 25.f, 155.f, ImVec2(402.5f, 1153.f));

			Elements::SubOption("Bypass Restrictions", ImVec2(290.f, 175.f), ImVec2(0.f, 1100.f));
			Elements::BoolOption("Weapons In Interiors", &g_WeaponFeatures->m_InteriorWeapons, ImVec2(10.f, 1130.f));
			Elements::BoolOption("Weapons In Passive Mode", &g_WeaponFeatures->m_PassiveWeapons, ImVec2(10.f, 1160.f));
			Elements::BoolOption("Heavy Weapons In Vehicle", &g_WeaponFeatures->m_AllowWeaponsInVehicle, ImVec2(10.f, 1190.f));
			Elements::BoolOption("Friendly Fire", &g_WeaponFeatures->m_FriendlyFire, ImVec2(10.f, 1220.f));
			Elements::BoolOption("Instant Lockon", &g_WeaponFeatures->m_InstantLockOn, ImVec2(145.f, 1220.f));
			Elements::BoolOption("Bypass C4 Limit", &g_WeaponFeatures->m_BypassC4Limit, ImVec2(10.f, 1250.f));
			Elements::BoolOption("Bypass Flare Limit", &g_WeaponFeatures->m_BypassFlareLimit, ImVec2(145.f, 1250.f));

			Elements::SubOption("Triggerbot", ImVec2(290.f, 145.f), ImVec2(300.f, 1200.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1230.f));
			ImGui::Text("Bone");
			ImGui::SetCursorPos(ImVec2(398.f, 1230.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###trigger_bot_bone", &g_WeaponFeatures->m_TriggerBotBonePos, g_WeaponFeatures->m_TriggerBotBone, IM_ARRAYSIZE(g_WeaponFeatures->m_TriggerBotBone), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Police##1", &g_WeaponFeatures->m_TriggerBotPolice, ImVec2(310.f, 1260.f));
			Elements::BoolOption("Civilians##1", &g_WeaponFeatures->m_TriggerBotCivilians, ImVec2(398.f, 1260.f));
			Elements::BoolOption("Enemies##1", &g_WeaponFeatures->m_TriggerBotEnemies, ImVec2(310.f, 1290.f));
			Elements::BoolOption("Players##1", &g_WeaponFeatures->m_TriggerBotPlayers, ImVec2(398.f, 1290.f));
			Elements::BoolOption("Enable##10", &g_WeaponFeatures->m_EnableTriggerBot, ImVec2(310.f, 1320.f));

			Elements::SubOption("Aimbot", ImVec2(290.f, 235.f), ImVec2(0.f, 1300.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1330.f));
			ImGui::Text("Bone");
			ImGui::SetCursorPos(ImVec2(98.f, 1330.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###aimbot_bone", &g_WeaponFeatures->m_AimBotBonePos, g_WeaponFeatures->m_AimBotBone, IM_ARRAYSIZE(g_WeaponFeatures->m_AimBotBone), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(10.f, 1360.f));
			ImGui::Text("Legit Speed");
			Elements::SliderFloatOption("###aimbot_legit_speed", &g_WeaponFeatures->m_AimBotLegitSpeed, 1.f, 12.f, 180.f, ImVec2(90.f, 1360.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1390.f));
			ImGui::Text("FOV");
			Elements::SliderFloatOption("###aimbot_fov", &g_WeaponFeatures->m_AimBotFOV, 1.f, 360.f, 180.f, ImVec2(90.f, 1390.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1420.f));
			ImGui::Text("Distance");
			Elements::SliderFloatOption("###aimbot_distance", &g_WeaponFeatures->m_AimBotDistance, 1.f, 360.f, 180.f, ImVec2(90.f, 1420.f));
			Elements::BoolOption("Police##2", &g_WeaponFeatures->m_AimBotPolice, ImVec2(10.f, 1450.f));
			Elements::BoolOption("Civilians##2", &g_WeaponFeatures->m_AimBotCivilians, ImVec2(98.f, 1450.f));
			Elements::BoolOption("Enemies##2", &g_WeaponFeatures->m_AimBotEnemies, ImVec2(10.f, 1480.f));
			Elements::BoolOption("Players##2", &g_WeaponFeatures->m_AimBotPlayers, ImVec2(98.f, 1480.f));
			Elements::BoolOption("Legit", &g_WeaponFeatures->m_LegitBot, ImVec2(10.f, 1510.f));
			Elements::BoolOption("Enable##11", &g_WeaponFeatures->m_EnableAimBot, ImVec2(98.f, 1510.f));

			Elements::SubOption("Gun Locker", ImVec2(290.f, 265.f), ImVec2(300.f, 1370.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1400.f));
			ImGui::Text("Category");
			ImGui::SetCursorPos(ImVec2(398.f, 1400.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###gun_locker_cat", &g_WeaponFeatures->m_GunLockerCatPos, g_WeaponFeatures->m_GunLockerCat, IM_ARRAYSIZE(g_WeaponFeatures->m_GunLockerCat), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(310.f, 1430.f));
			ImGui::Text("Weapon");
			ImGui::SetCursorPos(ImVec2(398.f, 1430.f));
			ImGui::PushItemWidth(180.f);
			switch (g_WeaponFeatures->m_GunLockerCatPos)
			{
			case 0:
				Elements::ComboOption("###gun_locker_melee", &g_WeaponFeatures->m_MeleeWeaponsPos, g_WeaponFeatures->m_MeleeWeapons, IM_ARRAYSIZE(g_WeaponFeatures->m_MeleeWeapons), -1);
				break;
			case 1:
				Elements::ComboOption("###gun_locker_handguns", &g_WeaponFeatures->m_HandgunsPos, g_WeaponFeatures->m_Handguns, IM_ARRAYSIZE(g_WeaponFeatures->m_Handguns), -1);
				break;
			case 2:
				Elements::ComboOption("###gun_locker_smgs", &g_WeaponFeatures->m_SubmachineGunsPos, g_WeaponFeatures->m_SubmachineGuns, IM_ARRAYSIZE(g_WeaponFeatures->m_SubmachineGuns), -1);
				break;
			case 3:
				Elements::ComboOption("###gun_locker_shotguns", &g_WeaponFeatures->m_ShotgunsPos, g_WeaponFeatures->m_Shotguns, IM_ARRAYSIZE(g_WeaponFeatures->m_Shotguns), -1);
				break;
			case 4:
				Elements::ComboOption("###gun_locker_assault_rifles", &g_WeaponFeatures->m_AssaultRiflesPos, g_WeaponFeatures->m_AssaultRifles, IM_ARRAYSIZE(g_WeaponFeatures->m_AssaultRifles), -1);
				break;
			case 5:
				Elements::ComboOption("###gun_locker_light_machine_guns", &g_WeaponFeatures->m_LightMachineGunsPos, g_WeaponFeatures->m_LightMachineGuns, IM_ARRAYSIZE(g_WeaponFeatures->m_LightMachineGuns), -1);
				break;
			case 6:
				Elements::ComboOption("###gun_locker_sniper_rifles", &g_WeaponFeatures->m_SniperRiflesPos, g_WeaponFeatures->m_SniperRifles, IM_ARRAYSIZE(g_WeaponFeatures->m_SniperRifles), -1);
				break;
			case 7:
				Elements::ComboOption("###gun_locker_heavy_weapons", &g_WeaponFeatures->m_HeavyWeaponsPos, g_WeaponFeatures->m_HeavyWeapons, IM_ARRAYSIZE(g_WeaponFeatures->m_HeavyWeapons), -1);
				break;
			case 8:
				Elements::ComboOption("###gun_locker_throwables", &g_WeaponFeatures->m_ThrowablesPos, g_WeaponFeatures->m_Throwables, IM_ARRAYSIZE(g_WeaponFeatures->m_Throwables), -1);
				break;
			case 9:
				Elements::ComboOption("###gun_locker_miscellaneous_weapons", &g_WeaponFeatures->m_MiscellaneousWeaponsPos, g_WeaponFeatures->m_MiscellaneousWeapons, IM_ARRAYSIZE(g_WeaponFeatures->m_MiscellaneousWeapons), -1);
				break;
			}
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Give Weapon", ImVec2(270.f, 25.f), ImVec2(310.f, 1460.f)))
			{
				g_ScriptQueue->Queue([] 
				{ 
					switch (g_WeaponFeatures->m_GunLockerCatPos)
					{
					case 0:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MeleeWeapons[g_WeaponFeatures->m_MeleeWeaponsPos], 9999, FALSE);
						break;
					case 1:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Handguns[g_WeaponFeatures->m_HandgunsPos], 9999, FALSE);
						break;
					case 2:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SubmachineGuns[g_WeaponFeatures->m_SubmachineGunsPos], 9999, FALSE);						
						break;
					case 3:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Shotguns[g_WeaponFeatures->m_ShotgunsPos], 9999, FALSE);
						break;
					case 4:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_AssaultRifles[g_WeaponFeatures->m_AssaultRiflesPos], 9999, FALSE);
						break;
					case 5:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_LightMachineGuns[g_WeaponFeatures->m_LightMachineGunsPos], 9999, FALSE);
						break;
					case 6:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SniperRifles[g_WeaponFeatures->m_SniperRiflesPos], 9999, FALSE);
						break;
					case 7:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_HeavyWeapons[g_WeaponFeatures->m_HeavyWeaponsPos], 9999, FALSE);
						break;
					case 8:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Throwables[g_WeaponFeatures->m_ThrowablesPos], 9999, FALSE);
						break;
					case 9:
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MiscellaneousWeapons[g_WeaponFeatures->m_MiscellaneousWeaponsPos], 9999, FALSE);
						break;
					}
				});
			}
			if (Elements::RegularOption("Take Weapon", ImVec2(270.f, 25.f), ImVec2(310.f, 1490.f)))
			{
				g_ScriptQueue->Queue([] 
				{ 
					switch (g_WeaponFeatures->m_GunLockerCatPos)
					{
					case 0:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MeleeWeapons[g_WeaponFeatures->m_MeleeWeaponsPos]);
						break;
					case 1:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Handguns[g_WeaponFeatures->m_HandgunsPos]);
						break;
					case 2:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SubmachineGuns[g_WeaponFeatures->m_SubmachineGunsPos]);
						break;
					case 3:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Shotguns[g_WeaponFeatures->m_ShotgunsPos]);
						break;
					case 4:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_AssaultRifles[g_WeaponFeatures->m_AssaultRiflesPos]);
						break;
					case 5:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_LightMachineGuns[g_WeaponFeatures->m_LightMachineGunsPos]);
						break;
					case 6:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SniperRifles[g_WeaponFeatures->m_SniperRiflesPos]);
						break;
					case 7:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_HeavyWeapons[g_WeaponFeatures->m_HeavyWeaponsPos]);
						break;
					case 8:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Throwables[g_WeaponFeatures->m_ThrowablesPos]);
						break;
					case 9:
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MiscellaneousWeapons[g_WeaponFeatures->m_MiscellaneousWeaponsPos]);
						break;
					}
				});
			}
			if (Elements::RegularOption("Give Entire Category", ImVec2(270.f, 25.f), ImVec2(310.f, 1520.f)))
			{
				g_ScriptQueue->Queue([]
				{
					switch (g_WeaponFeatures->m_GunLockerCatPos)
					{
					case 0:
						for (int i = 0; i < 17; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MeleeWeapons[i], 9999, FALSE);
						}
						break;
					case 1:
						for (int i = 0; i < 19; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Handguns[i], 9999, FALSE);
						}
						break;
					case 2:
						for (int i = 0; i < 8; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SubmachineGuns[i], 9999, FALSE);
						}
						break;
					case 3:
						for (int i = 0; i < 10; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Shotguns[i], 9999, FALSE);
						}
						break;
					case 4:
						for (int i = 0; i < 13; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_AssaultRifles[i], 9999, FALSE);
						}
						break;
					case 5:
						for (int i = 0; i < 4; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_LightMachineGuns[i], 9999, FALSE);
						}
						break;
					case 6:
						for (int i = 0; i < 6; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SniperRifles[i], 9999, FALSE);
						}
						break;
					case 7:
						for (int i = 0; i < 10; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_HeavyWeapons[i], 9999, FALSE);
						}
						break;
					case 8:
						for (int i = 0; i < 10; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Throwables[i], 9999, FALSE);
						}						
						break;
					case 9:
						for (int i = 0; i < 5; i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MiscellaneousWeapons[i], 9999, FALSE);
						}
						break;
					}
				});
			}
			if (Elements::RegularOption("Take Entire Category", ImVec2(270.f, 25.f), ImVec2(310.f, 1550.f)))
			{
				g_ScriptQueue->Queue([]
				{
					switch (g_WeaponFeatures->m_GunLockerCatPos)
					{
					case 0:
						for (int i = 0; i < 17; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MeleeWeapons[i]);
						}
						break;
					case 1:
						for (int i = 0; i < 19; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Handguns[i]);
						}
						break;
					case 2:
						for (int i = 0; i < 8; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SubmachineGuns[i]);
						}
						break;
					case 3:
						for (int i = 0; i < 10; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Shotguns[i]);
						}
						break;
					case 4:
						for (int i = 0; i < 13; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_AssaultRifles[i]);
						}
						break;
					case 5:
						for (int i = 0; i < 4; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_LightMachineGuns[i]);
						}
						break;
					case 6:
						for (int i = 0; i < 6; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SniperRifles[i]);
						}
						break;
					case 7:
						for (int i = 0; i < 10; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_HeavyWeapons[i]);
						}
						break;
					case 8:
						for (int i = 0; i < 10; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Throwables[i]);
						}
						break;
					case 9:
						for (int i = 0; i < 5; i++) {
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MiscellaneousWeapons[i]);
						}
						break;
					}
				});
			}
			if (Elements::RegularOption("Give Entire Locker", ImVec2(270.f, 25.f), ImVec2(310.f, 1580.f))) 
			{
				g_ScriptQueue->Queue([]
				{
					for (int i = 0; i < 17; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MeleeWeapons[i], 9999, FALSE);
					}

					for (int i = 0; i < 19; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Handguns[i], 9999, FALSE);
					}

					for (int i = 0; i < 8; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SubmachineGuns[i], 9999, FALSE);
					}

					for (int i = 0; i < 10; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Shotguns[i], 9999, FALSE);
					}

					for (int i = 0; i < 13; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_AssaultRifles[i], 9999, FALSE);
					}

					for (int i = 0; i < 4; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_LightMachineGuns[i], 9999, FALSE);
					}

					for (int i = 0; i < 6; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SniperRifles[i], 9999, FALSE);
					}

					for (int i = 0; i < 10; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_HeavyWeapons[i], 9999, FALSE);
					}

					for (int i = 0; i < 10; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Throwables[i], 9999, FALSE);
					}

					for (int i = 0; i < 5; i++) {
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MiscellaneousWeapons[i], 9999, FALSE);
					}
				});
			}
			if (Elements::RegularOption("Take Entire Locker", ImVec2(270.f, 25.f), ImVec2(310.f, 1610.f))) 
			{
				g_ScriptQueue->Queue([]
				{
					for (int i = 0; i < 17; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MeleeWeapons[i]);
					}

					for (int i = 0; i < 19; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Handguns[i]);
					}

					for (int i = 0; i < 8; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SubmachineGuns[i]);
					}

					for (int i = 0; i < 10; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Shotguns[i]);
					}

					for (int i = 0; i < 13; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_AssaultRifles[i]);
					}

					for (int i = 0; i < 4; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_LightMachineGuns[i]);
					}

					for (int i = 0; i < 6; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_SniperRifles[i]);
					}

					for (int i = 0; i < 10; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_HeavyWeapons[i]);
					}

					for (int i = 0; i < 10; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_Throwables[i]);
					}

					for (int i = 0; i < 5; i++) {
						WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), Lists::g_MiscellaneousWeapons[i]);
					}
				});
			}

			Elements::SubOption("MK2 Tint Manager", ImVec2(290.f, 75.f), ImVec2(0.f, 1560.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1590.f));
			ImGui::Text("Tint");
			ImGui::SetCursorPos(ImVec2(98.f, 1590.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###mk2_tint_manager_tint", &g_WeaponFeatures->m_Mk2TintsPos, g_WeaponFeatures->m_Mk2Tints, IM_ARRAYSIZE(g_WeaponFeatures->m_Mk2Tints), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Set Tint##1", ImVec2(130.f, 25.f), ImVec2(10.f, 1620.f))) 
			{
				g_ScriptQueue->Queue([]
				{
					WEAPON::SET_PED_WEAPON_TINT_INDEX(PLAYER::PLAYER_PED_ID(), WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), g_WeaponFeatures->m_Mk2TintsPos);
				});
			}
			Elements::BoolOption("Rainbow Tint##1", &g_WeaponFeatures->m_RainbowMk2Tint, ImVec2(150.f, 1620.f));

			Elements::SubOption("Regular Tint Manager", ImVec2(290.f, 75.f), ImVec2(300.f, 1660.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1690.f));
			ImGui::Text("Tint");
			ImGui::SetCursorPos(ImVec2(398.f, 1690.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###regular_tint_manager_tint", &g_WeaponFeatures->m_RegularTintsPos, g_WeaponFeatures->m_RegularTints, IM_ARRAYSIZE(g_WeaponFeatures->m_RegularTints), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Set Tint##2", ImVec2(130.f, 25.f), ImVec2(310.f, 1720.f))) {
				g_ScriptQueue->Queue([]
				{
					WEAPON::SET_PED_WEAPON_TINT_INDEX(PLAYER::PLAYER_PED_ID(), WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), g_WeaponFeatures->m_RegularTintsPos);
				});
			}
			Elements::BoolOption("Rainbow Tint##2", &g_WeaponFeatures->m_RainbowRegularTint, ImVec2(450.f, 1720.f));

			Elements::SubOption("Crosshair", ImVec2(290.f, 75.f), ImVec2(0.f, 1660.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1690.f));
			ImGui::Text("Texture");
			ImGui::SetCursorPos(ImVec2(98.f, 1690.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###crosshair_texture", &g_WeaponFeatures->m_CrosshairPos, g_WeaponFeatures->m_Crosshairs, IM_ARRAYSIZE(g_WeaponFeatures->m_Crosshairs), -1);
			ImGui::PopItemWidth();
			Elements::ColorEditOption("Color Picker##3", (float*)&g_WeaponFeatures->m_CrosshairColor, ImVec2(10.f, 1720.f));
			Elements::BoolOption("Rainbow##3", &g_WeaponFeatures->m_RainbowCrosshair, ImVec2(120.f, 1720.f));
			Elements::BoolOption("Enable##12", &g_WeaponFeatures->m_EnableCrosshair, ImVec2(205.f, 1720.f));

			ImGui::Dummy(ImVec2(0.f, 10.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}