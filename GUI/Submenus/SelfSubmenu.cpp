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
#include "../../Features/Self.hpp"
#include "../../Utility/All.hpp"
#include "../../Features/Misc.hpp"

namespace Sentinel
{
	void Submenus::SelfSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###self", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Health", ImVec2(290.f, 235.f), ImVec2(0.f, 0.f));
			if (Elements::ListBoxHeaderOption("###health", ImVec2(290.f, 235.f), ImVec2(0.f, 25.f)))
			{
				if (Elements::BoolOption("God", &g_SelfFeatures->m_GodMode, ImVec2(-1.f, -1.f)))
				{
					if (!g_SelfFeatures->m_GodMode)
						GTA::Offsets::SetValue<bool>({ 0x8, 0x189 }, false);
				}
				ImGui::SameLine();
				Elements::BoolOption("Semi God", &g_SelfFeatures->m_SemiGodMode, ImVec2(-1.f, -1.f));
				Elements::BoolOption("Unbreakable Armor", &g_SelfFeatures->m_UnbreakableArmor, ImVec2(-1.f, -1.f));
				Elements::ComboOption("Component", &g_SelfFeatures->m_ComponentPos, g_SelfFeatures->m_RestoreComponent, IM_ARRAYSIZE(g_SelfFeatures->m_RestoreComponent), -1);
				Elements::BoolOption("Auto Restore", &g_SelfFeatures->m_AutoRestore, ImVec2(-1.f, -1.f));
				Elements::BoolOption("Restore In Cover", &g_SelfFeatures->m_RestoreInCover, ImVec2(-1.f, -1.f));
				if (Elements::RegularOption("Fix Armor", ImVec2(80.f, 25.f), ImVec2(-1.f, -1.f))) {
					g_ScriptQueue->Queue([] { GTA::GetLocalPed()->m_armor = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID()); });
				}
				ImGui::SameLine();
				if (Elements::RegularOption("Break Armor", ImVec2(80.f, 25.f), ImVec2(-1.f, -1.f))) {
					GTA::GetLocalPed()->m_armor = 0;
				}
				if (Elements::RegularOption("Max Health", ImVec2(80.f, 25.f), ImVec2(-1.f, -1.f))) {
					GTA::GetLocalPed()->m_health = GTA::GetLocalPed()->m_maxhealth;
				}
				ImGui::SameLine();
				if (Elements::RegularOption("Suicide", ImVec2(80.f, 25.f), ImVec2(-1.f, -1.f))) {
					GTA::GetLocalPed()->m_health = 0.f;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Movement", ImVec2(290.f, 265.f), ImVec2(300.f, 0.f));
			Elements::BoolOption("Noclip", &g_SelfFeatures->m_NoClip, ImVec2(310.f, 30.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("It follows your game camera.\nKeybinds:\nForward: Go Forwards\nBackward: Go Backwards\nSprint: Go Faster\nDuck: Go Slower");
			Elements::SliderFloatOption("###noclip_speed", &g_SelfFeatures->m_NoClipSpeed, 1.f, 10.f, 155.f, ImVec2(402.5f, 33.f));
			Elements::BoolOption("Slide Run", &g_SelfFeatures->m_SlideRun, ImVec2(310.f, 60.f));
			Elements::SliderFloatOption("###sliderun_speed", &g_SelfFeatures->m_SlideRunSpeed, 1.f, 10.f, 155.f, ImVec2(402.5f, 63.f));
			Elements::BoolOption("Ninja Jump", &g_SelfFeatures->m_NinjaJump, ImVec2(310.f, 90.f));
			Elements::SliderIntOption("###ninja_jumps", &g_SelfFeatures->m_NinjaJumps, 1, 10, 155.f, ImVec2(402.5f, 93.f));
			Elements::BoolOption("Super Man", &g_SelfFeatures->m_SuperMan, ImVec2(310.f, 120.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Keybinds:\nSprint: Fly Upwards\nDuck: Fly Downwards");
			Elements::BoolOption("Super Jump", &g_SelfFeatures->m_SuperJump, ImVec2(450.f, 120.f));
			Elements::BoolOption("Beast Jump", &g_SelfFeatures->m_BeastJump, ImVec2(310.f, 150.f));
			Elements::BoolOption("Graceful Landing", &g_SelfFeatures->m_GracefulLanding, ImVec2(450.f, 150.f));
			Elements::BoolOption("Walk On Air", &g_SelfFeatures->m_WalkOnAir, ImVec2(310.f, 180.f));
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Disable mid air and re-enable when at desired altitude to go down.");
			}
			Elements::BoolOption("Walk On Water", &g_SelfFeatures->m_WalkOnWater, ImVec2(450.f, 180.f));
			Elements::BoolOption("Walk Under Water", &g_SelfFeatures->m_WalkUnderWater, ImVec2(310.f, 210.f));
			if (Elements::BoolOption("No Ragdoll", &g_SelfFeatures->m_NoRagdoll, ImVec2(450.f, 210.f)))
			{
				if (!g_SelfFeatures->m_NoRagdoll) 
				{
					g_ScriptQueue->Queue([]  
					{ 
						// Ragdoll Flags
						for (const auto& Flag : g_RagdollBlockingFlags)
							PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(PLAYER::PLAYER_PED_ID(), static_cast<int>(Flag));

						PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), TRUE);
						PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), TRUE);
						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 287, FALSE);

						PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 166, TRUE);
						PED::SET_PED_INJURED_ON_GROUND_BEHAVIOUR(PLAYER::PLAYER_PED_ID(), NULL);
					});
				}
			}
			Elements::BoolOption("Mozes Mode", &g_SelfFeatures->m_WalkThroughWater, ImVec2(310.f, 240.f));
			Elements::BoolOption("Freecam", &g_SelfFeatures->m_Freecam, ImVec2(450.f, 240.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Move your camera freely.\nKeybinds:\nNavigation: Regular Controls\nSprint: Go Up\nDuck: Go Down");

			Elements::SubOption("Modifiers", ImVec2(290.f, 195.f), ImVec2(0.f, 260.f));
			ImGui::SetCursorPos(ImVec2(10.f, 290.f));
			ImGui::Text("Health Regen");
			if (Elements::SliderFloatOption("###health_regen", &g_SelfFeatures->m_HealthRegenModifier, 0.f, 1.f, 155.f, ImVec2(105.f, 290.f)))
			{
				g_ScriptQueue->Queue([] { PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), g_SelfFeatures->m_HealthRegenModifier); });
			}
			Elements::BoolOption("Run Speed", &g_SelfFeatures->m_EnableRunSpeed, ImVec2(10.f, 315.f));
			Elements::SliderFloatOption("###run_speed", &g_SelfFeatures->m_RunSpeed, 1.f, 10.f, 155.f, ImVec2(105.f, 318.f));
			Elements::BoolOption("Swim Speed", &g_SelfFeatures->m_EnableSwimSpeed, ImVec2(10.f, 345.f));
			Elements::SliderFloatOption("###swim_speed", &g_SelfFeatures->m_SwimSpeed, 1.f, 10.f, 155.f, ImVec2(105.f, 348.f));
			Elements::BoolOption("Ped Width", &g_SelfFeatures->m_EnablePedWidth, ImVec2(10.f, 375.f));
			Elements::SliderFloatOption("###ped_width", &g_SelfFeatures->m_PedWidth, -50.f, 50.f, 155.f, ImVec2(105.f, 378.f));
			Elements::BoolOption("Ped Height", &g_SelfFeatures->m_EnablePedHeight, ImVec2(10.f, 405.f));
			Elements::SliderFloatOption("###ped_height", &g_SelfFeatures->m_PedHeight, -50.f, 50.f, 155.f, ImVec2(105.f, 408.f));
			Elements::BoolOption("Ped Lean", &g_SelfFeatures->m_EnablePedLean, ImVec2(10.f, 435.f));
			Elements::SliderFloatOption("###ped_lean", &g_SelfFeatures->m_PedLean, -50.f, 50.f, 155.f, ImVec2(105.f, 438.f));

			Elements::SubOption("Ped Proofs", ImVec2(290.f, 115.f), ImVec2(0.f, 480.f));
			Elements::BoolOption("Fire", &g_SelfFeatures->m_FireProof, ImVec2(10.f, 510.f));
			Elements::BoolOption("Explosion", &g_SelfFeatures->m_ExplosionProof, ImVec2(80.f, 510.f));
			Elements::BoolOption("Bullet", &g_SelfFeatures->m_BulletProof, ImVec2(10.f, 540.f));
			Elements::BoolOption("Collision", &g_SelfFeatures->m_CollisionProof, ImVec2(80.f, 540.f));
			Elements::BoolOption("Melee", &g_SelfFeatures->m_MeleeProof, ImVec2(10.f, 570.f));
			Elements::BoolOption("Drown", &g_SelfFeatures->m_DrownProof, ImVec2(80.f, 570.f));

			Elements::SubOption("Ped Flags", ImVec2(290.f, 175.f), ImVec2(300.f, 290.f));
			if (Elements::BoolOption("Swim Anywhere", &g_SelfFeatures->m_SwimAnywhere, ImVec2(310.f, 320.f)))
			{
				if (!g_SelfFeatures->m_SwimAnywhere)
					g_ScriptQueue->Queue([] { PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 65, FALSE); });
			}
			if (Elements::BoolOption("No Melee", &g_SelfFeatures->m_NoMelee, ImVec2(450.f, 320.f)))
			{
				if (!g_SelfFeatures->m_NoMelee)
					g_ScriptQueue->Queue([] { PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 122, FALSE); });
			}
			if (Elements::BoolOption("Can Ped Fly Through Windshield", &g_SelfFeatures->m_CanPedFlyThroughWindshield, ImVec2(310.f, 350.f)))
			{
				if (!g_SelfFeatures->m_CanPedFlyThroughWindshield)
					g_ScriptQueue->Queue([] { PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 32, FALSE); });
			}
			if (Elements::BoolOption("Dies By Ragdoll", &g_SelfFeatures->m_PedDiesByRagdoll, ImVec2(310.f, 380.f)))
			{
				if (!g_SelfFeatures->m_PedDiesByRagdoll)
					g_ScriptQueue->Queue([] { PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 33, FALSE); });
			}
			Elements::BoolOption("No Jumping", &g_SelfFeatures->m_NoJumping, ImVec2(450.f, 380.f));
			if (Elements::BoolOption("No Collision", &g_SelfFeatures->m_NoCollision, ImVec2(310.f, 410.f)))
			{
				if (!g_SelfFeatures->m_NoCollision)
					g_ScriptQueue->Queue([] { ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), TRUE, TRUE); });
			}
			Elements::BoolOption("Reduced Collision", &g_SelfFeatures->m_ReducedCollision, ImVec2(450.f, 410.f));
			if (Elements::BoolOption("Freeze", &g_SelfFeatures->m_Freeze, ImVec2(310.f, 440.f)))
			{
				if (!g_SelfFeatures->m_Freeze)
					g_ScriptQueue->Queue([] { ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), FALSE); });
			}
			if (Elements::BoolOption("Shrink", &g_SelfFeatures->m_Shrink, ImVec2(450.f, 440.f)))
			{
				if (!g_SelfFeatures->m_Shrink)
					g_ScriptQueue->Queue([] { PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, FALSE); });
			}

			Elements::SubOption("Handtrails", ImVec2(290.f, 85.f), ImVec2(300.f, 500.f));
			if (Elements::ListBoxHeaderOption("###handtrails", ImVec2(290.f, 75.f), ImVec2(300.f, 525.f)))
			{
				Elements::BoolOption("Enable##1", &g_SelfFeatures->m_Handtrails, ImVec2(-1.f, -1.f));
				ImGui::SameLine();
				Elements::BoolOption("Rainbow", &g_SelfFeatures->m_RainbowHandtrails, ImVec2(-1.f, -1.f));
				Elements::ColorEditOption("Color Picker", (float*)&g_SelfFeatures->m_HandTrailsColor, ImVec2(170.f, 4.f));
				Elements::InputFloatOption("Scale", &g_SelfFeatures->m_HandtrailsScale, 0.f, 60.f, "%.2f");

				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Police", ImVec2(290.f, 145.f), ImVec2(0.f, 620.f));
			if (Elements::BoolOption("Never Wanted", &g_SelfFeatures->m_NeverWanted, ImVec2(10.f, 650.f)))
			{
				if (!g_SelfFeatures->m_NeverWanted)
				{
					Pointers::pMaxWantedLevel->Restore();
					Pointers::pMaxWantedLevel2->Restore();
				}
			}
			if (Elements::BoolOption("Disable Wanted Level Sharing", &g_SelfFeatures->m_DisableSharing, ImVec2(10.f, 680.f)))
			{
				if (!g_SelfFeatures->m_DisableSharing)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE))
						VEHICLE::SET_VEHICLE_INFLUENCES_WANTED_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), TRUE);
				}
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Your vehicle won't affect your wanted level.");
			if (!g_SelfFeatures->m_NeverWanted)
			{
				Elements::BoolOption("Fake Wanted Level", &g_SelfFeatures->m_EnableFakeWantedLevel, ImVec2(10.f, 710.f));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This is only visual.");
				if (Elements::SliderIntOption("###fake_wanted_level", &g_SelfFeatures->m_FakeWantedLevel, 0, 5, 100.f, ImVec2(137.5f, 713.f)))
					GTA::GetLocalPlayerInfo()->m_wanted_level_display = g_SelfFeatures->m_FakeWantedLevel;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This is only visual.");
				Elements::BoolOption("Real Wanted Level", &g_SelfFeatures->m_EnableRealWantedLevel, ImVec2(10.f, 740.f));
				if (Elements::SliderIntOption("###real_wanted_level", &g_SelfFeatures->m_RealWantedLevel, 0, 5, 100.f, ImVec2(137.5f, 743.f)))
					GTA::GetLocalPlayerInfo()->m_wanted_level = g_SelfFeatures->m_RealWantedLevel;
			}

			Elements::SubOption("Miscellaneous", ImVec2(290.f, 235.f), ImVec2(300.f, 610.f));
			Elements::BoolOption("Quick Respawn", &g_SelfFeatures->m_QuickRespawn, ImVec2(310.f, 640.f));
			Elements::BoolOption("Respawn At Place Of Death", &g_SelfFeatures->m_RespawnAtDeath, ImVec2(310.f, 670.f));
			Elements::BoolOption("Infinite Stamina", &g_SelfFeatures->m_InfiniteStamina, ImVec2(310.f, 700.f));
			Elements::BoolOption("Ingored By Peds", &g_SelfFeatures->m_Ignored, ImVec2(310.f, 730.f));
			Elements::BoolOption("Anti NPC", &g_SelfFeatures->m_AntiNPC, ImVec2(310.f, 760.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Applies fatal damage to NPC's.");
			Elements::BoolOption("Karma", &g_SelfFeatures->m_Karma, ImVec2(450.f, 760.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Applies fatal damage to the player that kills you.");
			if (Elements::BoolOption("Fire Breath", &g_SelfFeatures->m_FireBreath, ImVec2(310.f, 790.f)))
			{
				if (!g_SelfFeatures->m_FireBreath) 
					g_ScriptQueue->Queue([] { GRAPHICS::REMOVE_PARTICLE_FX(g_SelfFeatures->m_FireBreathHandle, FALSE); });
			}
			if (Elements::BoolOption("Vomit", &g_SelfFeatures->m_Vomit, ImVec2(450.f, 790.f)))
			{
				if (!g_SelfFeatures->m_Vomit)
					g_ScriptQueue->Queue([] { GRAPHICS::REMOVE_PARTICLE_FX(g_SelfFeatures->m_VomitHandle, FALSE); });
			}
			if (Elements::BoolOption("Slowmotion", &g_SelfFeatures->m_SlowMotion, ImVec2(310.f, 820.f)))
			{
				if (!g_SelfFeatures->m_SlowMotion)
					g_ScriptQueue->Queue([] { MISC::SET_TIME_SCALE(1.f); });
			}
			if (Elements::BoolOption("Mobile Radio", &g_SelfFeatures->m_MobileRadio, ImVec2(450.f, 820.f)))
			{
				if (!g_SelfFeatures->m_MobileRadio)
				{
					g_ScriptQueue->Queue([] 
					{
						AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(FALSE);
						AUDIO::SET_MOBILE_PHONE_RADIO_STATE(FALSE);
					});
				}
			}

			Elements::SubOption("Visibility", ImVec2(290.f, 85.f), ImVec2(0.f, 790.f));
			if (Elements::ListBoxHeaderOption("###visibility", ImVec2(290.f, 85.f), ImVec2(0.f, 815.f)))
			{
				ImGui::SetCursorPos(ImVec2(8.f, 5.f));
				if (Elements::BoolOption("Invisibility", &g_SelfFeatures->m_Invisibility, ImVec2(-1.f, -1.f)))
				{
					if (!g_SelfFeatures->m_Invisibility)
					{
						g_ScriptQueue->Queue([] 
						{
							ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
							NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(PLAYER::PLAYER_PED_ID(), FALSE);
							NETWORK::SET_LOCAL_PLAYER_INVISIBLE_LOCALLY(FALSE);
						});
					}
				}
				ImGui::SetCursorPos(ImVec2(105.f, 5.f));
				ImGui::PushItemWidth(155.f);
				Elements::ComboOption("###mode", &g_SelfFeatures->m_InvisibilityModePos, g_SelfFeatures->m_InvisibilityMode, IM_ARRAYSIZE(g_SelfFeatures->m_InvisibilityMode), -1);
				ImGui::PopItemWidth();
				ImGui::SetCursorPos(ImVec2(8.f, 35.f));
				if (Elements::BoolOption("Opacity", &g_SelfFeatures->m_EnableOpacity, ImVec2(-1.f, -1.f)))
				{
					if (!g_SelfFeatures->m_EnableOpacity)
						g_ScriptQueue->Queue([] { ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 255, FALSE); });
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This is local only.");
				ImGui::PushItemWidth(155.f);
				Elements::SliderIntOption("###opacity_slider", &g_SelfFeatures->m_Opacity, 0, 255, 155.f, ImVec2(97.5f, 38.f));
				ImGui::PopItemWidth();
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Particles On Bone", ImVec2(290.f, 120.f), ImVec2(300.f, 870.f));
			if (Elements::ListBoxHeaderOption("###particles_on_bone", ImVec2(290.f, 110.f), ImVec2(300.f, 895.f)))
			{
				Elements::ComboOption("Types", &g_SelfFeatures->m_ParticleManTypesPos, g_SelfFeatures->m_ParticleManTypes, IM_ARRAYSIZE(g_SelfFeatures->m_ParticleManTypes), -1);
				Elements::InputFloatOption("Scale", &g_SelfFeatures->m_ParticleManScale, 0.f, 60.f, "%.2f");
				if (ImGui::CollapsingHeader("Body Parts"))
				{
					Elements::BoolOption("Head", &g_SelfFeatures->m_ParticleManHead, ImVec2(-1.f, -1.f));
					Elements::BoolOption("Left Hand", &g_SelfFeatures->m_ParticleManLeftHand, ImVec2(-1.f, -1.f));
					Elements::BoolOption("Right Hand", &g_SelfFeatures->m_ParticleManRightHand, ImVec2(-1.f, -1.f));
					Elements::BoolOption("Left Foot", &g_SelfFeatures->m_ParticleManLeftFoot, ImVec2(-1.f, -1.f));
					Elements::BoolOption("Right Foot", &g_SelfFeatures->m_ParticleManRightFoot, ImVec2(-1.f, -1.f));
					ImGui::Text("");
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Particles On Ped", ImVec2(290.f, 235.f), ImVec2(0.f, 900.f));
			if (Elements::ListBoxHeaderOption("###particles_on_ped", ImVec2(290.f, 225.f), ImVec2(0.f, 925.f)))
			{
				ImGui::SetCursorPos(ImVec2(8.f, 5.f));
				Elements::InputFloatOption("Scale", &g_SelfFeatures->m_ParticleOnPedScale, 0.f, 60.f, "%.2f");
				ImGui::SetCursorPos(ImVec2(8.f, 35.f));
				Elements::BoolOption("Fireworks", &g_SelfFeatures->m_LoopFireworkParticles, ImVec2(-1.f, -1.f));
				ImGui::SetCursorPos(ImVec2(105.f, 35.f));
				ImGui::PushItemWidth(155.f);
				if (Elements::ComboOption("###firework", &g_SelfFeatures->m_FireworkParticlesPos, g_SelfFeatures->m_FireworkParticles, IM_ARRAYSIZE(g_SelfFeatures->m_FireworkParticles), -1))
					g_ScriptQueue->Queue([] { Local::ParseParticle(0, g_SelfFeatures->m_FireworkParticlesPos); });
				ImGui::PopItemWidth();
				ImGui::SetCursorPos(ImVec2(8.f, 65.f));
				Elements::BoolOption("Clown", &g_SelfFeatures->m_LoopClownParticles, ImVec2(-1.f, -1.f));
				ImGui::SetCursorPos(ImVec2(105.f, 65.f));
				ImGui::PushItemWidth(155.f);
				if (Elements::ComboOption("###clown", &g_SelfFeatures->m_ClownParticlesPos, g_SelfFeatures->m_ClownParticles, IM_ARRAYSIZE(g_SelfFeatures->m_ClownParticles), -1))
					g_ScriptQueue->Queue([] { Local::ParseParticle(1, g_SelfFeatures->m_ClownParticlesPos); });
				ImGui::PopItemWidth();
				ImGui::SetCursorPos(ImVec2(8.f, 95.f));
				Elements::BoolOption("Alien", &g_SelfFeatures->m_LoopAlienParticles, ImVec2(-1.f, -1.f));
				ImGui::SetCursorPos(ImVec2(105.f, 95.f));
				ImGui::PushItemWidth(155.f);
				if (Elements::ComboOption("###alien", &g_SelfFeatures->m_AlienParticlesPos, g_SelfFeatures->m_AlienParticles, IM_ARRAYSIZE(g_SelfFeatures->m_AlienParticles), -1))
					g_ScriptQueue->Queue([] { Local::ParseParticle(2, g_SelfFeatures->m_AlienParticlesPos); });
				ImGui::PopItemWidth();
				ImGui::SetCursorPos(ImVec2(8.f, 125.f));
				Elements::BoolOption("Construct", &g_SelfFeatures->m_LoopConstructionParticles, ImVec2(-1.f, -1.f));
				ImGui::SetCursorPos(ImVec2(105.f, 125.f));
				ImGui::PushItemWidth(155.f);
				if (Elements::ComboOption("###construction", &g_SelfFeatures->m_ConstructionParticlesPos, g_SelfFeatures->m_ConstructionParticles, IM_ARRAYSIZE(g_SelfFeatures->m_ConstructionParticles), -1))
					g_ScriptQueue->Queue([] { Local::ParseParticle(3, g_SelfFeatures->m_ConstructionParticlesPos); });
				ImGui::PopItemWidth();
				ImGui::SetCursorPos(ImVec2(8.f, 155.f));
				Elements::BoolOption("Gore", &g_SelfFeatures->m_LoopGoreParticles, ImVec2(-1.f, -1.f));
				ImGui::SetCursorPos(ImVec2(105.f, 155.f));
				ImGui::PushItemWidth(155.f);
				if (Elements::ComboOption("###gore", &g_SelfFeatures->m_GoreParticlesPos, g_SelfFeatures->m_GoreParticles, IM_ARRAYSIZE(g_SelfFeatures->m_GoreParticles), -1))
					g_ScriptQueue->Queue([] { Local::ParseParticle(4, g_SelfFeatures->m_GoreParticlesPos); });
				ImGui::SetCursorPos(ImVec2(8.f, 185.f));
				Elements::BoolOption("Misc", &g_SelfFeatures->m_LoopMiscParticles, ImVec2(-1.f, -1.f));
				ImGui::SetCursorPos(ImVec2(105.f, 185.f));
				ImGui::PushItemWidth(155.f);
				if (Elements::ComboOption("###misc", &g_SelfFeatures->m_MiscParticlesPos, g_SelfFeatures->m_MiscParticles, IM_ARRAYSIZE(g_SelfFeatures->m_MiscParticles), -1))
					g_ScriptQueue->Queue([] { Local::ParseParticle(5, g_SelfFeatures->m_MiscParticlesPos); });
				ImGui::PopItemWidth();
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Animations", ImVec2(290.f, 280.f), ImVec2(300.f, 1015.f));
			if (Elements::ListBoxHeaderOption("###animations", ImVec2(290.f, 270.f), ImVec2(300.f, 1040.f)))
			{
				if (Elements::RegularOption("Stop Animation", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f)))
					g_ScriptQueue->Queue([] { Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID()); });
				Elements::BoolOption("Controllable", &g_SelfFeatures->m_ControllableAnimation, ImVec2(-1.f, -1.f));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Only your upper body gets animated.");
				Elements::BoolOption("Contort", &g_SelfFeatures->m_ContortAnimation, ImVec2(-1.f, -1.f));
				if (ImGui::CollapsingHeader("Sexual"))
				{
					for (int i = 0; i < Lists::g_SexualAnimations.size(); i++)
					{
						const auto MainPair = std::get<1>(Lists::g_SexualAnimations[i]);
						if (Elements::RegularOption(std::get<0>(Lists::g_SexualAnimations[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::PlayAnimation(std::get<0>(MainPair), std::get<1>(MainPair)); });
					}
				}
				if (ImGui::CollapsingHeader("Animal"))
				{
					for (int i = 0; i < Lists::g_AnimalAnimations.size(); i++)
					{
					    const auto MainPair = std::get<1>(Lists::g_AnimalAnimations[i]);
						if (Elements::RegularOption(std::get<0>(Lists::g_AnimalAnimations[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::PlayAnimation(std::get<0>(MainPair), std::get<1>(MainPair)); });
					}
				}
				if (ImGui::CollapsingHeader("Actions"))
				{
					for (int i = 0; i < Lists::g_ActionAnimations.size(); i++)
					{
						const auto MainPair = std::get<1>(Lists::g_ActionAnimations[i]);
						if (Elements::RegularOption(std::get<0>(Lists::g_ActionAnimations[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::PlayAnimation(std::get<0>(MainPair), std::get<1>(MainPair)); });
					}
				}
				if (ImGui::CollapsingHeader("Dance"))
				{
					for (int i = 0; i < Lists::g_DanceAnimations.size(); i++)
					{
						const auto MainPair = std::get<1>(Lists::g_DanceAnimations[i]);
						if (Elements::RegularOption(std::get<0>(Lists::g_DanceAnimations[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::PlayAnimation(std::get<0>(MainPair), std::get<1>(MainPair)); });
					}
				}
				if (ImGui::CollapsingHeader("Miscellaneous"))
				{
					for (int i = 0; i < Lists::g_MiscAnimations.size(); i++)
					{
						const auto MainPair = std::get<1>(Lists::g_MiscAnimations[i]);
						if (Elements::RegularOption(std::get<0>(Lists::g_MiscAnimations[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::PlayAnimation(std::get<0>(MainPair), std::get<1>(MainPair)); });
					}
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Scenarios", ImVec2(290.f, 220.f), ImVec2(0.f, 1160.f));
			if (Elements::ListBoxHeaderOption("###scenarios", ImVec2(290.f, 210.f), ImVec2(0.f, 1185.f)))
			{
				if (Elements::RegularOption("Stop Scenarios", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f)))
					g_ScriptQueue->Queue([] { Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID()); });
				if (ImGui::CollapsingHeader("Ambient"))
				{
					for (int i = 0; i < Lists::g_AmbientScenarios.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_AmbientScenarios[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), std::get<1>(Lists::g_AmbientScenarios[i]), 0, TRUE); });
					}
				}
				if (ImGui::CollapsingHeader("Activity"))
				{
					for (int i = 0; i < Lists::g_ActivityScenarios.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_ActivityScenarios[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), std::get<1>(Lists::g_ActivityScenarios[i]), 0, TRUE); });
					}
				}
				if (ImGui::CollapsingHeader("Animal"))
				{
					for (int i = 0; i < Lists::g_AnimalScenarios.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_AnimalScenarios[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), std::get<1>(Lists::g_AnimalScenarios[i]), 0, TRUE); });
					}
				}
				if (ImGui::CollapsingHeader("Fitness"))
				{
					for (int i = 0; i < Lists::g_FitnessScenarios.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_FitnessScenarios[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), std::get<1>(Lists::g_FitnessScenarios[i]), 0, TRUE); });
					}
				}
				if (ImGui::CollapsingHeader("Industrial"))
				{
					for (int i = 0; i < Lists::g_IndustrialScenarios.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_IndustrialScenarios[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), std::get<1>(Lists::g_IndustrialScenarios[i]), 0, TRUE); });
					}
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Clipset", ImVec2(290.f, 160.f), ImVec2(300.f, 1320.f));
			if (Elements::ListBoxHeaderOption("###clipset", ImVec2(290.f, 150.f), ImVec2(300.f, 1345.f)))
			{
				if (Elements::RegularOption("Reset Motion Clipset", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f)))
					g_ScriptQueue->Queue([] { PED::RESET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), 1.f); });
				if (Elements::RegularOption("Reset Weapon Clipset", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f)))
					g_ScriptQueue->Queue([] { PED::RESET_PED_WEAPON_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID()); });
				if (ImGui::CollapsingHeader("Motion"))
				{
					for (int i = 0; i < Lists::g_ClipsetMotion.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_ClipsetMotion[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::StartClipset(false, std::get<1>(Lists::g_ClipsetMotion[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Weapon"))
				{
					for (int i = 0; i < Lists::g_WeaponClipset.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_WeaponClipset[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::StartClipset(true, std::get<1>(Lists::g_WeaponClipset[i])); });
					}
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Model Changer", ImVec2(290.f, 340.f), ImVec2(0.f, 1405.f));
			if (Elements::ListBoxHeaderOption("###model_changer", ImVec2(290.f, 330.f), ImVec2(0.f, 1430.f)))
			{
				if (Elements::RegularOption("Refresh Model", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f)))
				{
					g_ScriptQueue->Queue([] 
					{
						PED::RESET_PED_VISIBLE_DAMAGE(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_DECORATIONS(PLAYER::PLAYER_PED_ID());
					});
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Resets all visual damage to your ped.");
				if (Elements::RegularOption("Refresh Model Cache", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f)))
					g_SelfFeatures->m_LastModel = 0;
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Refresh the cache that re-applies your model when you die.");
				if (ImGui::CollapsingHeader("Custom Model"))
				{
					GUI::SetTyping();
					Elements::InputTextOption("###custom_model", g_SelfFeatures->m_CustomModel, IM_ARRAYSIZE(g_SelfFeatures->m_CustomModel));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Input the model! (Hint: name, hash or numeric)");
					if (Elements::RegularOption("Set Model", ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
					{
						g_ScriptQueue->Queue([] 
						{
							if (g_SelfFeatures->m_CustomModel)
							{
								if (g_SelfFeatures->m_CustomModel[0] == '0' && (g_SelfFeatures->m_CustomModel[1] == 'x' || g_SelfFeatures->m_CustomModel[1] == 'X'))
								{
									// Hash
									Local::ChangeModel((std::uint32_t)_strtoui64(g_SelfFeatures->m_CustomModel, 0, 0));
								}
								else
								{
									std::string str(g_SelfFeatures->m_CustomModel);
									if (std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end())
									{
										// Numeric
										Local::ChangeModel((std::uint32_t)atoi(g_SelfFeatures->m_CustomModel));
									}
									else {
										// Name
										Local::ChangeModel(Joaat(g_SelfFeatures->m_CustomModel));
									}
								}
							}
						});
					}
				}
				else
					GUI::SetDoneTyping();
				if (ImGui::CollapsingHeader("Popular"))
				{
					for (int i = 0; i < Lists::g_PopularModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_PopularModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_PopularModels[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Story"))
				{
					for (int i = 0; i < Lists::g_StoryModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_StoryModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_StoryModels[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Animals"))
				{
					for (int i = 0; i < Lists::g_AnimalModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_AnimalModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_AnimalModels[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Emergency"))
				{
					for (int i = 0; i < Lists::g_EmergencyModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_EmergencyModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_EmergencyModels[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Role"))
				{
					for (int i = 0; i < Lists::g_RoleModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_RoleModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_RoleModels[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Ambient"))
				{
					for (int i = 0; i < Lists::g_AmbientModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_AmbientModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_AmbientModels[i])); });
					}
				}
				if (ImGui::CollapsingHeader("Misc"))
				{
					for (int i = 0; i < Lists::g_MiscModels.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_MiscModels[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([=] { Local::ChangeModel(std::get<1>(Lists::g_MiscModels[i])); });
					}
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Global Modification", ImVec2(290.f, 185.f), ImVec2(300.f, 1505.f));
			if (Elements::BoolOption("Off Radar", &g_SelfFeatures->m_OffRadar, ImVec2(310.f, 1535.f)))
			{
				if (!g_SelfFeatures->m_OffRadar)
				{
					*ScriptGlobal(2657589).At(PLAYER::PLAYER_ID(), 466).Add(210).As<std::int32_t*>() = 0;
					*ScriptGlobal(2672505).Add(57).As<std::int32_t*>() = 0;
				}
			}
			if (Elements::BoolOption("Cops Turn Blind Eye", &g_SelfFeatures->m_CopsTurnBlindEye, ImVec2(310.f, 1565.f)))
			{
				if (!g_SelfFeatures->m_CopsTurnBlindEye)
				{
					*ScriptGlobal(2793046).Add(4654).As<std::int32_t*>() = 0;
					*ScriptGlobal(2793046).Add(4657).As<std::int32_t*>() = 0;
				}
			}
			if (Elements::BoolOption("Reveal Players", &g_SelfFeatures->m_RevealPlayers, ImVec2(310.f, 1595.f)))
			{
				if (!g_SelfFeatures->m_RevealPlayers)
				{
					*ScriptGlobal(2657589).At(PLAYER::PLAYER_ID(), 466).Add(213).As<std::int32_t*>() = 0;
					*ScriptGlobal(2672505).Add(58).As<std::int32_t*>() = 0;
				}
			}
			if (Elements::RegularOption("Ammo Drop", ImVec2(80.f, 25.f), ImVec2(310.f, 1625.f))) 
				*ScriptGlobal(2793046).Add(886).As<int*>() = 1;
			if (Elements::RegularOption("Boat Pickup", ImVec2(80.f, 25.f), ImVec2(400.f, 1625.f)))
				*ScriptGlobal(2793046).Add(887).As<int*>() = 1;
			if (Elements::RegularOption("Heli Pickup", ImVec2(80.f, 25.f), ImVec2(490.f, 1625.f)))
				*ScriptGlobal(2793046).Add(888).As<int*>() = 1;
			if (Elements::RegularOption("Backup Heli", ImVec2(80.f, 25.f), ImVec2(310.f, 1655.f)))
				*ScriptGlobal(2793046).Add(4484).As<int*>() = 1;
			if (Elements::RegularOption("Airstrike", ImVec2(80.f, 25.f), ImVec2(400.f, 1655.f)))
				*ScriptGlobal(2793046).Add(4485).As<int*>() = 1;
			if (Elements::RegularOption("Fix MMS", ImVec2(80.f, 25.f), ImVec2(490.f, 1655.f)))
				Mobile::FixAllMMS();
			if (ImGui::IsItemHovered()) 
				ImGui::SetTooltip("Repairs all your Mors Mutual Vehicles free of charge.");

			Elements::SubOption("Abilities", ImVec2(290.f, 175.f), ImVec2(0.f, 1770.f));
			Elements::BoolOption("Yoga God", &g_SelfFeatures->m_YogaGod, ImVec2(10.f, 1800.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Become a meditating god.\nKeybinds:\nLeft Click: Push Entities\nRight Click: Pull Entities\nScroll: Move Entities");
			Elements::BoolOption("Electric Sprint", &g_SelfFeatures->m_ElectricSprint, ImVec2(10.f, 1830.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Become an electrical sphere.\nKeybinds:\nForwards: Go Forward\nBackward: Go Backwards\nSprint: Go Faster");
			Elements::BoolOption("Lazer Eyes", &g_SelfFeatures->m_LazerEyes, ImVec2(10.f, 1860.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Shoot explosions from your eyes.\nKeybinds:\nLeft Click: Shoot");
			Elements::BoolOption("Force Push", &g_SelfFeatures->m_ForcePush, ImVec2(10.f, 1890.f));
			Elements::BoolOption("Force Pull", &g_SelfFeatures->m_ForcePull, ImVec2(10.f, 1920.f));

			Elements::SubOption("Forcefield", ImVec2(290.f, 175.f), ImVec2(300.f, 1715.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1748.f));
			ImGui::Text("Range");
			Elements::SliderFloatOption("###force_field_range", &g_SelfFeatures->m_ForcefieldRange, 1.f, 100.f, 200.f, ImVec2(350.f, 1748.f));
			Elements::BoolOption("Draw Sphere", &g_SelfFeatures->m_ForcefieldSphere, ImVec2(310.f, 1770.f));
			Elements::BoolOption("Target Peds", &g_SelfFeatures->m_ForcefieldPeds, ImVec2(310.f, 1800.f));
			Elements::BoolOption("Target Vehicles", &g_SelfFeatures->m_ForcefieldVehicles, ImVec2(310.f, 1830.f));
			Elements::BoolOption("Enable##2", &g_SelfFeatures->m_EnableForcefield, ImVec2(310.f, 1860.f));

			Elements::SubOption("Bodyguard", ImVec2(290.f, 265.f), ImVec2(0.f, 1970.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2000.f));
			ImGui::PushItemWidth(155.f);
			Elements::ComboOption("Model", &g_SelfFeatures->m_BodyGuardModelPos, g_SelfFeatures->m_BodyGuardModel, IM_ARRAYSIZE(g_SelfFeatures->m_BodyGuardModel), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(10.f, 2030.f));
			ImGui::PushItemWidth(155.f);
			Elements::ComboOption("Weapon", &g_SelfFeatures->m_BodyGuardWeaponPos, g_SelfFeatures->m_BodyGuardWeapon, IM_ARRAYSIZE(g_SelfFeatures->m_BodyGuardWeapon), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Invincible", &g_SelfFeatures->m_BodyGuardInvincible, ImVec2(10.f, 2060.f));
			Elements::BoolOption("Invisible", &g_SelfFeatures->m_BodyGuardInvisible, ImVec2(10.f, 2090.f));
			Elements::BoolOption("No Fall", &g_SelfFeatures->m_BodyGuardNoRagdoll, ImVec2(10.f, 2120.f));
			if (Elements::RegularOption("Create Bodyguard", ImVec2(270.f, 25.f), ImVec2(10.f, 2150.f)))
			{
				g_ScriptQueue->Queue([] 
				{
					Hash BodyguardModel = Lists::g_BodyguardModelHashes[g_SelfFeatures->m_BodyGuardModelPos];
					Hash BodyguardWeapon = Lists::g_BodyguardWeaponHashes[g_SelfFeatures->m_BodyGuardWeaponPos];

					while (!STREAMING::HAS_MODEL_LOADED(BodyguardModel))
					{
						STREAMING::REQUEST_MODEL(BodyguardModel);
						Script::Current()->Yield(5ms);
					}

					while (!WEAPON::HAS_WEAPON_ASSET_LOADED(BodyguardWeapon))
					{
						WEAPON::REQUEST_WEAPON_ASSET(BodyguardWeapon, 31, 0);
						Script::Current()->Yield(5ms);
					}

					const auto Group = PED::GET_PED_GROUP_INDEX(PLAYER::PLAYER_PED_ID());
					PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), Group);
					const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
					System::ModelSpawnBypass(true);
					const auto Ped = PED::CREATE_PED(26, BodyguardModel, Coords.x + rand() % 5, Coords.y + rand() % 5, Coords.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), TRUE, FALSE);
					System::ModelSpawnBypass(false);
					PED::SET_PED_AS_GROUP_MEMBER(Ped, Group);
					PED::SET_PED_NEVER_LEAVES_GROUP(Ped, TRUE);
					PED::SET_GROUP_FORMATION(Ped, 3);
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(Ped, BodyguardWeapon, 9999, TRUE);
					PED::SET_PED_CAN_SWITCH_WEAPON(Ped, FALSE);
					PED::SET_PED_COMBAT_ABILITY(Ped, 2); // Professional
					PED::SET_PED_COMBAT_RANGE(Ped, 2); // Far
					PED::SET_CAN_ATTACK_FRIENDLY(Ped, FALSE, FALSE);
					PED::SET_ALLOW_LOCKON_TO_PED_IF_FRIENDLY(Ped, FALSE);
					const auto RelationShip = PED::GET_PED_RELATIONSHIP_GROUP_HASH(PLAYER::PLAYER_PED_ID());
					PED::SET_PED_RELATIONSHIP_GROUP_HASH(Ped, RelationShip);

					if (g_SelfFeatures->m_BodyGuardInvincible)
						ENTITY::SET_ENTITY_INVINCIBLE(Ped, TRUE);

					if (g_SelfFeatures->m_BodyGuardInvisible)
					{
						NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Ped, TRUE);
						ENTITY::SET_ENTITY_VISIBLE(Ped, FALSE, FALSE);
					}

					if (g_SelfFeatures->m_BodyGuardNoRagdoll)
					{
						// Ragdoll Flags
						for (const auto& Flag : g_RagdollBlockingFlags)
							PED::SET_RAGDOLL_BLOCKING_FLAGS(Ped, static_cast<int>(Flag));

						PED::SET_PED_CAN_RAGDOLL(Ped, FALSE);
						PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(Ped, FALSE);
						PED::SET_PED_CONFIG_FLAG(Ped, 287, TRUE);

						// This speeds up the weird falling animation where you're injured
						PED::SET_PED_CONFIG_FLAG(Ped, 166, FALSE);
						PED::DISABLE_PED_INJURED_ON_GROUND_BEHAVIOUR(Ped);

						// Stop Injured Clipset
						PED::RESET_PED_STRAFE_CLIPSET(Ped);
					}

					g_SelfFeatures->m_CreatedBodyguards.push_back(Ped);
				});
			}
			if (Elements::RegularOption("Delete Bodyguards", ImVec2(270.f, 25.f), ImVec2(10.f, 2180.f)))
			{
				if (!g_SelfFeatures->m_CreatedBodyguards.empty())
				{
					g_ScriptQueue->Queue([] 
					{
						for (int i = 0; i < g_SelfFeatures->m_CreatedBodyguards.size(); i++)
						{
							const Ped CurrentPed = g_SelfFeatures->m_CreatedBodyguards[i];
							if (ENTITY::DOES_ENTITY_EXIST(CurrentPed))
							{
								Entities::DeleteEntity(CurrentPed);
								Script::Current()->Yield(200ms);
								g_SelfFeatures->m_CreatedBodyguards.erase(g_SelfFeatures->m_CreatedBodyguards.begin() + i);
							}
						}
					});
				}
			}
			if (Elements::RegularOption("Teleport To Me", ImVec2(270.f, 25.f), ImVec2(10.f, 2210.f)))
			{
				if (!g_SelfFeatures->m_CreatedBodyguards.empty())
				{
					g_ScriptQueue->Queue([] 
					{
						for (int i = 0; i < g_SelfFeatures->m_CreatedBodyguards.size(); i++)
						{
							const Ped CurrentPed = g_SelfFeatures->m_CreatedBodyguards[i];
							if (ENTITY::DOES_ENTITY_EXIST(CurrentPed))
							{
								const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
								ENTITY::SET_ENTITY_COORDS(CurrentPed, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
							}
						}
					});
				}
			}

			Elements::SubOption("Visions", ImVec2(290.f, 175.f), ImVec2(300.f, 1915.f));
			if (Elements::ListBoxHeaderOption("###vision_options", ImVec2(290.f, 165.f), ImVec2(300.f, 1940.f)))
			{
				if (Elements::BoolOption("Blackout", &g_SelfFeatures->m_Blackout, ImVec2(-1.f, -1.f)))
				{
					if (!g_SelfFeatures->m_Blackout)
					{
						g_ScriptQueue->Queue([=] 
						{
							GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(FALSE);
							GRAPHICS::SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(FALSE);
						});
					}
				}
				if (Elements::BoolOption("Thermal Vision", &g_SelfFeatures->m_ThermalVision, ImVec2(-1.f, -1.f)))
				{
					if (!g_SelfFeatures->m_ThermalVision)
					{
						g_ScriptQueue->Queue([=] 
						{
							*ScriptGlobal(1853910).At(PLAYER::PLAYER_ID(), 862).Add(821).Add(11).As<int*>() = 0;
							GRAPHICS::SET_SEETHROUGH(FALSE);
						});
					}
				}
				if (Elements::BoolOption("Night Vision", &g_SelfFeatures->m_NightVision, ImVec2(-1.f, -1.f)))
				{
					if (!g_SelfFeatures->m_NightVision)
					{
						g_ScriptQueue->Queue([=] 
						{
							*ScriptGlobal(1853910).At(PLAYER::PLAYER_ID(), 862).Add(821).Add(11).As<int*>() = 0;
							GRAPHICS::SET_NIGHTVISION(FALSE);
						});
					}
				}
				if (Elements::RegularOption("Reset Timecycle Modifier", ImVec2(270.f, 25.f), ImVec2(-1.f, -1.f))) 
					g_ScriptQueue->Queue([] { GRAPHICS::CLEAR_TIMECYCLE_MODIFIER(); });
				if (ImGui::CollapsingHeader("Timecycle Modifiers"))
				{
					for (int i = 0; i < Lists::g_TimecycleVisions.size(); i++)
					{
						if (Elements::RegularOption(std::get<0>(Lists::g_TimecycleVisions[i]), ImVec2(260.f, 25.f), ImVec2(-1.f, -1.f))) 
							g_ScriptQueue->Queue([=] { GRAPHICS::SET_TIMECYCLE_MODIFIER(std::get<1>(Lists::g_TimecycleVisions[i])); });
					}
				}
				ImGui::ListBoxFooter();
			}

			ImGui::Dummy(ImVec2(0.f, 120.f));
			ImGui::Dummy(ImVec2(300.f, 120.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}