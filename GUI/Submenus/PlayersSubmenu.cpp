 #include "../../Common.hpp"
#include "Submenus.hpp"
#include "../GUI.hpp"
#include "../Overlays/Overlays.hpp"
#include "../Elements.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../GTA/GTA.hpp"
#include "../../GTA/enums.hpp"
#include "../../GTA/Kicks.hpp"
#include "../../GTA/Crashes.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../GTA/Script/ScriptQueue.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/Script/ScriptLocal.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../Features/Players.hpp"
#include "../../Features/AngryPlanes.hpp"
#include "../../Services/Players/PlayerService.hpp"
#include "../../Utility/All.hpp"

namespace Sentinel
{
	void Submenus::PlayersSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###players", ImVec2(606.f, 610.f)))
		{
			const auto PlayerCount = g_PlayerService->Players().size() + 1;

			if (!*Pointers::pIsSessionStarted && PlayerCount < 2) {
				ImGui::SetCursorPos(ImVec2(10.f, 0.f));
				ImGui::Text("You should probably go online.");
				return;
			}

			Elements::Gradient(ImVec2(ImGui::GetCurrentWindow()->Pos.x + 209.f, ImGui::GetCurrentWindow()->Pos.y), ImVec2(ImGui::GetCurrentWindow()->Pos.x + 208.f, ImGui::GetCurrentWindow()->Pos.y + 608.f), ImGui::ColorConvertU32ToFloat4(ImColor(80, 80, 80, 255)), ImGui::ColorConvertU32ToFloat4(ImColor(80, 80, 80, 255)));
			if (Elements::ListBoxHeaderOption("###plist", ImVec2(200.f, 600.f), ImVec2(0.f, 0.f)))
			{
				std::string SelfText = g_PlayerService->GetSelf()->GetName();
				SelfText += " [ME]";
				if (g_PlayerService->GetSelf()->IsHost())
					SelfText += " [H]";
				if (g_PlayerService->GetSelf()->m_ScriptHost)
					SelfText += " [SH]";
				if (ImGui::Selectable(SelfText.c_str(), g_PlayerService->GetSelf() == g_PlayerService->GetSelected()))
					g_PlayerService->SetSelected(g_PlayerService->GetSelf());

				for (const auto& [_, player] : g_PlayerService->Players())
				{
					std::string PlayerText = player->GetName();
					if (player->IsHost())
						PlayerText += " [H]";
					if (player->m_ScriptHost)
						PlayerText += " [SH]";
					if (player->IsFriend())
						PlayerText += " [F]";
					if (ImGui::Selectable(PlayerText.c_str(), player == g_PlayerService->GetSelected()))
						g_PlayerService->SetSelected(player);
				}

				ImGui::ListBoxFooter();
			}

			if (Elements::ListBoxHeaderOption("###features", ImVec2(406.f, 600.f), ImVec2(200.f, 0.f)))
			{
				if (g_PlayerService->GetSelected()->IsValid())
				{
					Elements::SubOption("Trolling", ImVec2(180.f, 445.f), ImVec2(20.f, 0.f));
					Elements::BoolOption("Kill", &g_PlayerService->GetSelected()->m_KillPlayer, ImVec2(30.f, 30.f));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("This isn't silent, your name will be displayed.");
					Elements::BoolOption("Freeze##1", &g_PlayerService->GetSelected()->m_FreezePlayer, ImVec2(30.f, 60.f));
					Elements::BoolOption("Ragdoll", &g_PlayerService->GetSelected()->m_RagdollPlayer, ImVec2(30.f, 90.f));
					Elements::BoolOption("Explode", &g_PlayerService->GetSelected()->m_ExplodePlayer, ImVec2(30.f, 120.f));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Will make it appear as a constant suicide.");
					if (Elements::BoolOption("Burn", &g_PlayerService->GetSelected()->m_BurnPlayer, ImVec2(30.f, 150.f)))
					{
						if (g_PlayerService->GetSelected()->m_BurnPlayer)
							g_ScriptQueue->Queue([] { Target::BurnPlayer(g_PlayerService->GetSelected()); });
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Interval: 5s");
					Elements::BoolOption("Flame Ray", &g_PlayerService->GetSelected()->m_FlameRay, ImVec2(30.f, 180.f));
					Elements::BoolOption("Water Hydrant", &g_PlayerService->GetSelected()->m_WaterHydrant, ImVec2(30.f, 210.f));
					Elements::BoolOption("Shake Camera", &g_PlayerService->GetSelected()->m_ShakeCamera, ImVec2(30.f, 240.f));
					Elements::BoolOption("Glitch Physics", &g_PlayerService->GetSelected()->m_GlitchPhysics, ImVec2(30.f, 270.f));
					Elements::BoolOption("Rocket Rain", &g_PlayerService->GetSelected()->m_RainRockets, ImVec2(30.f, 300.f));
					Elements::BoolOption("Clone Spam", &g_PlayerService->GetSelected()->m_CloneSpam, ImVec2(30.f, 330.f));
					Elements::BoolOption("Glitchfield", &g_PlayerService->GetSelected()->m_Glitchfield, ImVec2(30.f, 360.f));
					Elements::BoolOption("Give Wanted", &g_PlayerService->GetSelected()->m_GiveWantedLevel, ImVec2(30.f, 390.f));
					Elements::SliderIntOption("###give_wanted_level", &g_PlayerService->GetSelected()->m_WantedLevelToGive, 1, 5, 50.f, ImVec2(128.f, 393.f));
					if (Elements::RegularOption("Blame", ImVec2(75.f, 25.f), ImVec2(30.f, 420.f)))
					{
						g_ScriptQueue->Queue([]
						{
							// We love nesting
							for (int i = 0; i < MAX_PLAYERS; ++i)
							{
								const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
								if (i != PLAYER::PLAYER_ID() && !INTERIOR::IS_VALID_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(Ped)) && ENTITY::DOES_ENTITY_EXIST(Ped))
								{
									const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
									Target::BlameExplodeCoord(g_PlayerService->GetSelected(), Coords, EXP_TAG_BLIMP, 1000.f, true, false, 0.f);
								}
							}
						});
					}
					if (Elements::RegularOption("Clone", ImVec2(75.f, 25.f), ImVec2(115.f, 420.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							PED::CLONE_PED(Ped, TRUE, FALSE, TRUE);
						});
					}

					Elements::SubOption("General", ImVec2(180.f, 115.f), ImVec2(20.f, 470.f));
					Elements::BoolOption("Spectate", &g_PlayersFeatures->m_Spectate, ImVec2(30.f, 500.f));
					if (Elements::RegularOption("Open SC", ImVec2(75.f, 25.f), ImVec2(115.f, 500.f)))
					{
						g_ScriptQueue->Queue([]
						{
							Any Handle[76];
							NETWORK::NETWORK_HANDLE_FROM_PLAYER(g_PlayerService->GetSelected()->ID(), &Handle[0], 13);
							NETWORK::NETWORK_SHOW_PROFILE_UI(&Handle[0]);
						});
					}
					if (Elements::RegularOption("Add Friend", ImVec2(75.f, 25.f), ImVec2(30.f, 530.f)))
					{
						g_ScriptQueue->Queue([]
						{
							Any Handle[76];
							NETWORK::NETWORK_HANDLE_FROM_PLAYER(g_PlayerService->GetSelected()->ID(), &Handle[0], 13);
							NETWORK::NETWORK_ADD_FRIEND(&Handle[0], "Can we be friends UwU");
						});
					}
					if (Elements::RegularOption("Join Crew", ImVec2(75.f, 25.f), ImVec2(115.f, 530.f)))
					{
						g_ScriptQueue->Queue([]
						{
							Any Handle; Any Description;
							NETWORK::NETWORK_CLAN_PLAYER_GET_DESC(&Description, 35, &Handle);
							NETWORK::NETWORK_CLAN_JOIN(Handle);
						});
					}
					if (Elements::RegularOption("Copy RID", ImVec2(75.f, 25.f), ImVec2(30.f, 560.f)))
					{
						ImGui::SetClipboardText(std::format("{}'s RID: {}", g_PlayerService->GetSelected()->GetName(), g_PlayerService->GetSelected()->GetNetData()->m_gamer_handle.m_rockstar_id).c_str());
					}
					if (Elements::RegularOption("Copy IP", ImVec2(75.f, 25.f), ImVec2(115.f, 560.f)))
					{
						const auto netAddress = g_PlayerService->GetSelected()->GetIPAddress();
						ImGui::SetClipboardText(std::format("{}'s IP: {}.{}.{}.{}", g_PlayerService->GetSelected()->GetName(), netAddress.m_field1, netAddress.m_field2, netAddress.m_field3, netAddress.m_field4).c_str());
					}

					Elements::SubOption("Teleportation", ImVec2(180.f, 205.f), ImVec2(20.f, 610.f));
					if (Elements::RegularOption("To Coords", ImVec2(155.f, 25.f), ImVec2(30.f, 640.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
							PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
						});
					}
					if (Elements::RegularOption("In Vehicle", ImVec2(155.f, 25.f), ImVec2(30.f, 670.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
							const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);

							for (int i = -1; i < 16; i++)
							{
								if (VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, i, TRUE))
								{
									ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
									PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, i);
								}
							}
						});
					}
					if (Elements::RegularOption("Bring", ImVec2(155.f, 25.f), ImVec2(30.f, 700.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);
								const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
								if (Entities::RequestControl(Vehicle))
									ENTITY::SET_ENTITY_COORDS(Vehicle, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
							}
						});
					}
					if (Elements::RegularOption("Bring To Waypoint", ImVec2(155.f, 25.f), ImVec2(30.f, 730.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);
								const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
								if (Entities::RequestControl(Vehicle))
									Teleport::ToWaypoint(Vehicle);
							}
						});
					}
					if (Elements::RegularOption("Bring To Objective", ImVec2(155.f, 25.f), ImVec2(30.f, 760.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);
								const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
								if (Entities::RequestControl(Vehicle))
									Teleport::ToObjective(Vehicle);
							}
						});
					}
					if (Elements::RegularOption("Bring To Ocean", ImVec2(155.f, 25.f), ImVec2(30.f, 790.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							NativeVector3 Ocean = { -6233.433f, -5388.144f, -105.988304f };
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);
								MISC::GET_GROUND_Z_FOR_3D_COORD(Ocean.x, Ocean.y, Ocean.z, &Ocean.z, FALSE, FALSE);
								STREAMING::REQUEST_COLLISION_AT_COORD(Ocean.x, Ocean.y, Ocean.z);
								if (Entities::RequestControl(Vehicle))
									ENTITY::SET_ENTITY_COORDS(Vehicle, Ocean.x, Ocean.y, Ocean.z, FALSE, FALSE, FALSE, FALSE);
							}
						});
					}

					Elements::SubOption("Trapping", ImVec2(180.f, 145.f), ImVec2(20.f, 840.f));
					Elements::BoolOption("Invisible##1", &g_PlayersFeatures->m_InvisibleCages, ImVec2(30.f, 870.f));
					if (Elements::RegularOption("Regular Cage", ImVec2(155.f, 25.f), ImVec2(30.f, 900.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Hash = Joaat("prop_gold_cont_01");
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
							MISC::GET_GROUND_Z_FOR_3D_COORD(Coords.x, Coords.y, Coords.z, &Coords.z, FALSE, FALSE);

							while (!STREAMING::HAS_MODEL_LOADED(Hash))
							{
								STREAMING::REQUEST_MODEL(Hash);
								Script::Current()->Yield(5ms);
							}

							System::ModelSpawnBypass(true);
							const auto Object = OBJECT::CREATE_OBJECT(Hash, Coords.x, Coords.y, Coords.z, TRUE, FALSE, FALSE);
							ENTITY::FREEZE_ENTITY_POSITION(Object, TRUE);
							System::ModelSpawnBypass(false);

							if (ENTITY::DOES_ENTITY_EXIST(Object) && g_PlayersFeatures->m_InvisibleCages)
								ENTITY::SET_ENTITY_VISIBLE(Object, FALSE, FALSE);
						});
					}
					if (Elements::RegularOption("Fence Cage", ImVec2(155.f, 25.f), ImVec2(30.f, 930.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Hash = 0x466EB7B8;
							while (!STREAMING::HAS_MODEL_LOADED(Hash))
							{
								STREAMING::REQUEST_MODEL(Hash);
								Script::Current()->Yield(5ms);
							}

							NativeVector3 MinDimensions;
							NativeVector3 MaxDimensions;

							MISC::GET_MODEL_DIMENSIONS(Hash, &MinDimensions, &MaxDimensions);
							const auto Diameter = Math::Subtract(MaxDimensions, MinDimensions);

							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Ped, -1.5f, -1.5f, 0.f);
							MISC::GET_GROUND_Z_FOR_3D_COORD(Coords.x, Coords.y, Coords.z, &Coords.z, FALSE, FALSE);

							System::ModelSpawnBypass(true);
							const auto Object = OBJECT::CREATE_OBJECT(Hash, Coords.x, Coords.y, Coords.z, TRUE, FALSE, FALSE);
							System::ModelSpawnBypass(false);

							if (ENTITY::DOES_ENTITY_EXIST(Object))
							{
								auto Rotation = ENTITY::GET_ENTITY_HEADING(Ped);
								ENTITY::SET_ENTITY_ROTATION(Object, 0.f, 0.f, Rotation, 2, FALSE);
								ENTITY::FREEZE_ENTITY_POSITION(Object, TRUE);

								if (g_PlayersFeatures->m_InvisibleCages)
									ENTITY::SET_ENTITY_VISIBLE(Object, FALSE, FALSE);

								auto NextObject = Object;
								for (int i = 0; i < 3; i++) 
								{
									Rotation += 90.f;

									const auto NextCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(NextObject, Diameter.x, 0.f, 0.f);
									System::ModelSpawnBypass(true);
									NextObject = OBJECT::CREATE_OBJECT(Hash, NextCoords.x, NextCoords.y, NextCoords.z, TRUE, FALSE, FALSE);
									System::ModelSpawnBypass(false);
									if (ENTITY::DOES_ENTITY_EXIST(NextObject))
									{
										ENTITY::SET_ENTITY_ROTATION(NextObject, 0.f, 0.f, Rotation, 2, FALSE);
										ENTITY::FREEZE_ENTITY_POSITION(NextObject, TRUE);

										if (g_PlayersFeatures->m_InvisibleCages)
											ENTITY::SET_ENTITY_VISIBLE(NextObject, FALSE, FALSE);
									}
								}
							}
						});
					}
					if (Elements::RegularOption("Trolly Cage", ImVec2(155.f, 25.f), ImVec2(30.f, 960.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Hash = 0x7B059043;
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
							MISC::GET_GROUND_Z_FOR_3D_COORD(Coords.x, Coords.y, Coords.z, &Coords.z, FALSE, FALSE);

							while (!STREAMING::HAS_MODEL_LOADED(Hash))
							{
								STREAMING::REQUEST_MODEL(Hash);
								Script::Current()->Yield(5ms);
							}

							System::ModelSpawnBypass(true);
							auto Object = OBJECT::CREATE_OBJECT(Hash, Coords.x, Coords.y, Coords.z - 0.5f, TRUE, FALSE, FALSE);
							ENTITY::FREEZE_ENTITY_POSITION(Object, TRUE);
							System::ModelSpawnBypass(false);

							if (ENTITY::DOES_ENTITY_EXIST(Object) && g_PlayersFeatures->m_InvisibleCages)
								ENTITY::SET_ENTITY_VISIBLE(Object, FALSE, FALSE);

							System::ModelSpawnBypass(true);
							Object = OBJECT::CREATE_OBJECT(Hash, Coords.x, Coords.y, Coords.z + 2.f, TRUE, FALSE, FALSE);
							ENTITY::FREEZE_ENTITY_POSITION(Object, TRUE);
							ENTITY::SET_ENTITY_ROTATION(Object, 0.f, 180.f, 90.f, 0, FALSE);
							System::ModelSpawnBypass(false);

							if (ENTITY::DOES_ENTITY_EXIST(Object) && g_PlayersFeatures->m_InvisibleCages)
								ENTITY::SET_ENTITY_VISIBLE(Object, FALSE, FALSE);
						});
					}

					Elements::SubOption("Shoot", ImVec2(180.f, 115.f), ImVec2(20.f, 1010.f));
					ImGui::SetCursorPos(ImVec2(30.f, 1040.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###bullet", &g_PlayerService->GetSelected()->m_ShootBulletPos, g_PlayerService->GetSelected()->m_ShootBullet, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_ShootBullet), -1);
					ImGui::PopItemWidth();
					if (Elements::RegularOption("Shoot", ImVec2(155.f, 25.f), ImVec2(30.f, 1070.f)))
					{
						const auto WeaponHash = Lists::g_ShootBulletHashes[g_PlayerService->GetSelected()->m_ShootBulletPos];
						g_ScriptQueue->Queue([=] { Target::ShootPlayer(g_PlayerService->GetSelected(), WeaponHash); });
					}
					Elements::BoolOption("Repeated Shooting", &g_PlayerService->GetSelected()->m_LoopShoot, ImVec2(30.f, 1100.f));

					Elements::SubOption("Remote Teleport", ImVec2(180.f, 205.f), ImVec2(20.f, 1150.f));
					ImGui::SetCursorPos(ImVec2(30.f, 1180.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###apt_array", &g_PlayerService->GetSelected()->m_AppartmentIndex, g_PlayerService->GetSelected()->m_AppartmentNames, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_AppartmentNames), -1);
					ImGui::PopItemWidth();
					if (Elements::RegularOption("Send To Appartment", ImVec2(155.f, 25.f), ImVec2(30.f, 1210.f)))
					{
						g_ScriptQueue->Queue([] 
						{ 
							const std::size_t ArgCount = 9;
							std::int64_t Args[ArgCount] = { 
								(std::int64_t)eRemoteEvent::Teleport, 
								PLAYER::GET_PLAYER_INDEX(),
								(std::int64_t)g_PlayerService->GetSelected()->ID(),
								(std::int64_t)(int)-1, 1, 
								(std::int64_t)g_PlayerService->GetSelected()->m_AppartmentIndex,
								1, 1, 1 
							};

							Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << g_PlayerService->GetSelected()->ID());
						});
					}
					ImGui::SetCursorPos(ImVec2(30.f, 1240.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###wh_array", &g_PlayerService->GetSelected()->m_WarehouseIndex, g_PlayerService->GetSelected()->m_WarehouseNames, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_WarehouseNames), -1);
					ImGui::PopItemWidth();
					if (Elements::RegularOption("Send To Warehouse", ImVec2(155.f, 25.f), ImVec2(30.f, 1270.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const std::size_t ArgCount = 6;
							std::int64_t Args[ArgCount] = { 
								(std::int64_t)eRemoteEvent::TeleportToWarehouse, 
								PLAYER::GET_PLAYER_INDEX(),
								(std::int64_t)g_PlayerService->GetSelected()->ID(),
								1, 
								(std::int64_t)g_PlayerService->GetSelected()->m_WarehouseIndex
							};

							Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << g_PlayerService->GetSelected()->ID());
						});
					}
					ImGui::SetCursorPos(ImVec2(30.f, 1300.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###int_array", &g_PlayerService->GetSelected()->m_InteriorIndex, g_PlayerService->GetSelected()->m_InteriorNames, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_InteriorNames), -1);
					ImGui::PopItemWidth();
					if (Elements::RegularOption("Send To Interior", ImVec2(155.f, 25.f), ImVec2(30.f, 1330.f)))
					{
						g_ScriptQueue->Queue([]
						{
							int InteriorIndex;

							switch (g_PlayerService->GetSelected()->m_InteriorIndex)
							{
							case 0:
								InteriorIndex = 81;
								break;
							case 1:
								InteriorIndex = 123;
								break;
							case 2:
								InteriorIndex = 124;
								break;
							case 3:
								InteriorIndex = 128;
								break;
							case 4:
								InteriorIndex = 146;
								break;
							case 5:
								InteriorIndex = 148;
								break;
							case 6:
								InteriorIndex = 149;
								break;
							case 7:
								InteriorIndex = 155;
								break;
							case 8:
								InteriorIndex = 160;
								break;
							case 9:
								InteriorIndex = 161;
								break;
							}

							float Max = 1e+38f;
							auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID()), FALSE);
							const std::size_t ArgCount = 15;
							std::int64_t Args[ArgCount] = {
								(std::int64_t)eRemoteEvent::InteriorControl,
								(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
								(std::int64_t)InteriorIndex,
								(std::int64_t)PLAYER::GET_PLAYER_INDEX(),
								(std::int64_t)false,
								(std::int64_t)true,// True means enter sender interior
								(std::int64_t) * (std::uint32_t*)&Coords.x,
								(std::int64_t) * (std::uint32_t*)&Coords.y,
								(std::int64_t) * (std::uint32_t*)&Coords.z,
								0,
								0,
								1,
								(std::int64_t) * (std::uint32_t*)&Max,
								(std::int64_t)true,
								-1 
							};

							Pointers::pTriggerScriptEvent(1, Args, ArgCount, 1 << g_PlayerService->GetSelected()->ID());
						});
					}

					Elements::SubOption("Entity Spammer", ImVec2(180.f, 145.f), ImVec2(20.f, 1380.f));
					ImGui::SetCursorPos(ImVec2(30.f, 1410.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###ent_spammer", &g_PlayerService->GetSelected()->m_EntitySpammerPos, g_PlayerService->GetSelected()->m_EntitySpammer, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_EntitySpammer), -1);
					ImGui::PopItemWidth();
					Elements::BoolOption("Enable##1", &g_PlayerService->GetSelected()->m_EntitySpammerEnable, ImVec2(30.f, 1440.f));
					if (Elements::RegularOption("Attempt Cleanup", ImVec2(155.f, 25.f), ImVec2(30.f, 1470.f)))
					{
						g_ScriptQueue->Queue([]
						{
							for (int Index = 0; Index < g_PlayerService->GetSelected()->m_CreatedEntities.size(); Index++)
							{
								const auto& Entity = g_PlayerService->GetSelected()->m_CreatedEntities[Index];
								if (!ENTITY::DOES_ENTITY_EXIST(Entity))
									return;

								if (ENTITY::IS_ENTITY_A_PED(Entity)) {
									Entities::RequestControl(Entity);
									Entities::ClearPedTasks(Entity);
									Entities::DeleteEntity(Entity);
								}

								if (ENTITY::IS_ENTITY_A_VEHICLE(Entity)) {
									Entities::RequestControl(Entity);
									Entities::DeleteEntity(Entity);
								}

								Script::Current()->Yield(200ms);

								g_PlayerService->GetSelected()->m_CreatedEntities.erase(g_PlayerService->GetSelected()->m_CreatedEntities.begin() + Index);
							}
						});
					}
					ImGui::SetCursorPos(ImVec2(30.f, 1505.f));
					ImGui::Text("Delay");
					Elements::SliderIntOption("###ent_spammer_delay", &g_PlayerService->GetSelected()->m_EntitySpammerDelay, 0, 10000, 105.f, ImVec2(65.f, 1505.f));

					Elements::SubOption("SMS Service", ImVec2(180.f, 200.f), ImVec2(20.f, 1550.f));
					if (Elements::ListBoxHeaderOption("###sms_service", ImVec2(180.f, 190.f), ImVec2(20.f, 1575.f)))
					{
						ImGui::TextWrapped(std::format("Message: {}", g_PlayerService->GetSelected()->m_TextMessage).c_str());
						if (ImGui::CollapsingHeader("Presets"))
						{
							if (Elements::RegularOption("Buy Sentinel##1", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f))) {
								char temp[29] = "Buy Sentinel at sentinel.sbs";
								strcpy(g_PlayerService->GetSelected()->m_TextMessage, temp);
							}
							if (Elements::RegularOption("Sentinel #1##1", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[12] = "Sentinel #1";
								strcpy(g_PlayerService->GetSelected()->m_TextMessage, temp);
							}
							if (Elements::RegularOption("Enjoy the crash##1", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f))) 
							{
								char temp[21] = "Enjoy the crash lmao";
								strcpy(g_PlayerService->GetSelected()->m_TextMessage, temp);
							}
							if (Elements::RegularOption("Type 123 for money drop##1", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[24] = "Type 123 for money drop";
								strcpy(g_PlayerService->GetSelected()->m_TextMessage, temp);
							}
							if (Elements::RegularOption("Crash me if you can##1", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[20] = "Crash me if you can";
								strcpy(g_PlayerService->GetSelected()->m_TextMessage, temp);
							}
							if (Elements::RegularOption("Light yourself on fire##1", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[23] = "Light yourself on fire";
								strcpy(g_PlayerService->GetSelected()->m_TextMessage, temp);
							}
						}
						if (ImGui::CollapsingHeader("Custom"))
						{
							GUI::SetTyping();
							Elements::InputTextOption("###custom_message", g_PlayerService->GetSelected()->m_TextMessage, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_TextMessage));
						}
						else
							GUI::SetDoneTyping();
						ImGui::PushItemWidth(155.f);
						Elements::ComboOption("###msg_color", &g_PlayerService->GetSelected()->m_MessageColorPos, g_PlayerService->GetSelected()->m_MessageColor, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_MessageColor), -1);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(155.f);
						Elements::ComboOption("###msg_font", &g_PlayerService->GetSelected()->m_MessageFontPos, g_PlayerService->GetSelected()->m_MessageFont, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_MessageFont), -1);
						ImGui::PopItemWidth();
						if (Elements::RegularOption("Send", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([] { Target::SendTextMessage(g_PlayerService->GetSelected(), g_PlayerService->GetSelected()->m_TextMessage, g_PlayerService->GetSelected()->m_MessageColorPos, g_PlayerService->GetSelected()->m_MessageFontPos); });
						ImGui::ListBoxFooter();
					}

					Elements::SubOption("Private Chat Spammer", ImVec2(180.f, 240.f), ImVec2(20.f, 1775.f));
					if (Elements::ListBoxHeaderOption("###chat_spammer", ImVec2(180.f, 230.f), ImVec2(20.f, 1800.f)))
					{
						ImGui::TextWrapped(std::format("Message: {}", g_PlayerService->GetSelected()->m_ChatMessage).c_str());
						if (ImGui::CollapsingHeader("Presets"))
						{
							if (Elements::RegularOption("Buy Sentinel##2", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f))) {
								char temp[29] = "Buy Sentinel at sentinel.sbs";
								strcpy(g_PlayerService->GetSelected()->m_ChatMessage, temp);
							}
							if (Elements::RegularOption("Sentinel #1##2", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[12] = "Sentinel #1";
								strcpy(g_PlayerService->GetSelected()->m_ChatMessage, temp);
							}
							if (Elements::RegularOption("Enjoy the crash##2", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[21] = "Enjoy the crash lmao";
								strcpy(g_PlayerService->GetSelected()->m_ChatMessage, temp);
							}
							if (Elements::RegularOption("Type 123 for money drop##2", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[24] = "Type 123 for money drop";
								strcpy(g_PlayerService->GetSelected()->m_ChatMessage, temp);
							}
							if (Elements::RegularOption("Crash me if you can##2", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[20] = "Crash me if you can";
								strcpy(g_PlayerService->GetSelected()->m_ChatMessage, temp);
							}
							if (Elements::RegularOption("Light yourself on fire##2", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							{
								char temp[23] = "Light yourself on fire";
								strcpy(g_PlayerService->GetSelected()->m_ChatMessage, temp);
							}
						}
						if (ImGui::CollapsingHeader("Custom"))
						{
							GUI::SetTyping();
							Elements::InputTextOption("###custom_chat_message", g_PlayerService->GetSelected()->m_ChatMessage, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_ChatMessage));
						}
						else
							GUI::SetDoneTyping();
						Elements::BoolOption("View Locally", &g_PlayerService->GetSelected()->m_DrawLocally, ImVec2(-1.f, -1.f));
						if (Elements::RegularOption("Send Once", ImVec2(155.f, 25.f), ImVec2(-1.f, -1.f)))
							g_ScriptQueue->Queue([] { Target::SendPrivateChatMessage(g_PlayerService->GetSelected(), g_PlayerService->GetSelected()->m_ChatMessage, g_PlayerService->GetSelected()->m_DrawLocally); });
						ImGui::Text("Delay");
						ImGui::SameLine();
						Elements::SliderIntOption("###chat_delay", &g_PlayerService->GetSelected()->m_SpamDelay, 0, 10000, 105.f, ImVec2(-1.f, -1.f));
						if (Elements::BoolOption("Spam Chat", &g_PlayerService->GetSelected()->m_StartSpamming, ImVec2(-1.f, -1.f)))
						{
							g_ScriptQueue->Queue([] 
							{
								if (!g_PlayerService->GetSelected()->m_StartSpamming)
								{
									Script::Current()->Yield(100ms);
									PLAYER::SET_PLAYER_TEAM(PLAYER::PLAYER_ID(), -1);
									PLAYER::SET_PLAYER_TEAM(g_PlayerService->GetSelected()->ID(), -1);
								}
							});
						}
						ImGui::ListBoxFooter();
					}

					Elements::SubOption("Bodyguards", ImVec2(180.f, 265.f), ImVec2(20.f, 2040.f));
					ImGui::SetCursorPos(ImVec2(30.f, 2070.f));
					ImGui::PushItemWidth(110.f);
					Elements::ComboOption("Model##1", &g_PlayerService->GetSelected()->m_BodyGuardModelPos, g_PlayerService->GetSelected()->m_BodyGuardModel, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_BodyGuardModel), -1);
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(30.f, 2100.f));
					ImGui::PushItemWidth(110.f);
					Elements::ComboOption("Weapon##1", &g_PlayerService->GetSelected()->m_BodyGuardWeaponPos, g_PlayerService->GetSelected()->m_BodyGuardWeapon, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_BodyGuardWeapon), -1);
					ImGui::PopItemWidth();
					Elements::BoolOption("Invincible##3", &g_PlayerService->GetSelected()->m_BodyGuardInvincible, ImVec2(30.f, 2130.f));
					Elements::BoolOption("Invisible##3", &g_PlayerService->GetSelected()->m_BodyGuardInvisible, ImVec2(30.f, 2160.f));
					Elements::BoolOption("No Fall##1", &g_PlayerService->GetSelected()->m_BodyGuardNoRagdoll, ImVec2(30.f, 2190.f));
					if (Elements::RegularOption("Create##1", ImVec2(155.f, 25.f), ImVec2(30.f, 2220.f)))
					{
						g_ScriptQueue->Queue([]
						{
							Hash BodyguardModel = Lists::g_BodyguardModelHashes[g_PlayerService->GetSelected()->m_BodyGuardModelPos];
							Hash BodyguardWeapon = Lists::g_BodyguardWeaponHashes[g_PlayerService->GetSelected()->m_BodyGuardWeaponPos];

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

							const auto PlayerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							const auto Group = PED::GET_PED_GROUP_INDEX(PlayerPed);
							PED::SET_PED_AS_GROUP_LEADER(PlayerPed, Group);
							const auto Coords = ENTITY::GET_ENTITY_COORDS(PlayerPed, TRUE);
							System::ModelSpawnBypass(true);
							const auto Ped = PED::CREATE_PED(26, BodyguardModel, Coords.x + rand() % 5, Coords.y + rand() % 5, Coords.z, ENTITY::GET_ENTITY_HEADING(PlayerPed), TRUE, FALSE);
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
							const auto RelationShip = PED::GET_PED_RELATIONSHIP_GROUP_HASH(PlayerPed);
							PED::SET_PED_RELATIONSHIP_GROUP_HASH(Ped, RelationShip);

							if (g_PlayerService->GetSelected()->m_BodyGuardInvincible)
								ENTITY::SET_ENTITY_INVINCIBLE(Ped, TRUE);

							if (g_PlayerService->GetSelected()->m_BodyGuardInvisible)
							{
								NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Ped, TRUE);
								ENTITY::SET_ENTITY_VISIBLE(Ped, FALSE, FALSE);
							}

							if (g_PlayerService->GetSelected()->m_BodyGuardNoRagdoll)
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

							g_PlayerService->GetSelected()->m_CreatedBodyguards.push_back(Ped);
						});
					}
					if (Elements::RegularOption("Delete##1", ImVec2(155.f, 25.f), ImVec2(30.f, 2250.f)))
					{
						if (!g_PlayerService->GetSelected()->m_CreatedBodyguards.empty())
						{
							g_ScriptQueue->Queue([]
							{
								for (int i = 0; i < g_PlayerService->GetSelected()->m_CreatedBodyguards.size(); i++)
								{
									const Ped CurrentPed = g_PlayerService->GetSelected()->m_CreatedBodyguards[i];
									if (ENTITY::DOES_ENTITY_EXIST(CurrentPed))
									{
										Entities::RequestControl(CurrentPed);
										Entities::DeleteEntity(CurrentPed);
										Script::Current()->Yield(200ms);
										g_PlayerService->GetSelected()->m_CreatedBodyguards.erase(g_PlayerService->GetSelected()->m_CreatedBodyguards.begin() + i);
									}
								}
							});
						}
					}
					if (Elements::RegularOption("Teleport##1", ImVec2(155.f, 25.f), ImVec2(30.f, 2280.f)))
					{
						if (!g_PlayerService->GetSelected()->m_CreatedBodyguards.empty())
						{
							g_ScriptQueue->Queue([]
							{
								for (int i = 0; i < g_PlayerService->GetSelected()->m_CreatedBodyguards.size(); i++)
								{
									const Ped CurrentPed = g_PlayerService->GetSelected()->m_CreatedBodyguards[i];
									if (ENTITY::DOES_ENTITY_EXIST(CurrentPed))
									{
										Entities::RequestControl(CurrentPed);
										const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID()), TRUE);
										ENTITY::SET_ENTITY_COORDS(CurrentPed, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
									}
								}
							});
						}
					}

					Elements::SubOption("Attackers", ImVec2(180.f, 265.f), ImVec2(20.f, 2330.f));
					ImGui::SetCursorPos(ImVec2(30.f, 2360.f));
					ImGui::PushItemWidth(110.f);
					Elements::ComboOption("Model##2", &g_PlayerService->GetSelected()->m_AttackerModelPos, g_PlayerService->GetSelected()->m_AttackerModel, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_AttackerModel), -1);
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(30.f, 2390.f));
					ImGui::PushItemWidth(110.f);
					Elements::ComboOption("Weapon##2", &g_PlayerService->GetSelected()->m_AttackerWeaponPos, g_PlayerService->GetSelected()->m_AttackerWeapon, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_AttackerWeapon), -1);
					ImGui::PopItemWidth();
					Elements::BoolOption("Invincible##4", &g_PlayerService->GetSelected()->m_AttackerInvincible, ImVec2(30.f, 2420.f));
					Elements::BoolOption("Invisible##4", &g_PlayerService->GetSelected()->m_AttackerInvisible, ImVec2(30.f, 2450.f));
					Elements::BoolOption("No Fall##2", &g_PlayerService->GetSelected()->m_AttackerNoRagdoll, ImVec2(30.f, 2480.f));
					if (Elements::RegularOption("Create##2", ImVec2(155.f, 25.f), ImVec2(30.f, 2510.f)))
					{
						g_ScriptQueue->Queue([]
						{
							Hash AttackerModel = Lists::g_BodyguardModelHashes[g_PlayerService->GetSelected()->m_AttackerModelPos];
							Hash AttackerWeapon = Lists::g_BodyguardWeaponHashes[g_PlayerService->GetSelected()->m_AttackerWeaponPos];
				
							while (!STREAMING::HAS_MODEL_LOADED(AttackerModel))
							{
								STREAMING::REQUEST_MODEL(AttackerModel);
								Script::Current()->Yield(5ms);
							}

							while (!WEAPON::HAS_WEAPON_ASSET_LOADED(AttackerWeapon))
							{
								WEAPON::REQUEST_WEAPON_ASSET(AttackerWeapon, 31, 0);
								Script::Current()->Yield(5ms);
							}

							const auto PlayerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							const auto Coords = ENTITY::GET_ENTITY_COORDS(PlayerPed, TRUE);
							System::ModelSpawnBypass(true);
							const auto Ped = PED::CREATE_PED(26, AttackerModel, Coords.x + rand() % 5, Coords.y + rand() % 5, Coords.z, ENTITY::GET_ENTITY_HEADING(PlayerPed), TRUE, FALSE);
							System::ModelSpawnBypass(false);
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(Ped, AttackerWeapon, 9999, TRUE);
							PED::SET_PED_CAN_SWITCH_WEAPON(Ped, FALSE);
							PED::SET_PED_COMBAT_ABILITY(Ped, 2); // Professional
							PED::SET_PED_COMBAT_RANGE(Ped, 2); // Far
							TASK::TASK_COMBAT_PED(Ped, PlayerPed, 1, 1);
							PED::SET_PED_KEEP_TASK(Ped, TRUE);
							PED::SET_PED_ALERTNESS(Ped, 3);

							if (g_PlayerService->GetSelected()->m_AttackerInvincible)
								ENTITY::SET_ENTITY_INVINCIBLE(Ped, TRUE);

							if (g_PlayerService->GetSelected()->m_AttackerInvisible)
							{
								NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Ped, TRUE);
								ENTITY::SET_ENTITY_VISIBLE(Ped, FALSE, FALSE);
							}

							if (g_PlayerService->GetSelected()->m_AttackerNoRagdoll)
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

							g_PlayerService->GetSelected()->m_CreatedAttackers.push_back(Ped);
						});
					}
					if (Elements::RegularOption("Delete##2", ImVec2(155.f, 25.f), ImVec2(30.f, 2540.f)))
					{
						if (!g_PlayerService->GetSelected()->m_CreatedAttackers.empty())
						{
							g_ScriptQueue->Queue([]
							{
								for (int i = 0; i < g_PlayerService->GetSelected()->m_CreatedAttackers.size(); i++)
								{
									const Ped CurrentPed = g_PlayerService->GetSelected()->m_CreatedAttackers[i];
									if (ENTITY::DOES_ENTITY_EXIST(CurrentPed))
									{
										Entities::RequestControl(CurrentPed);
										Script::Current()->Yield(200ms);
										Entities::DeleteEntity(CurrentPed);
										Script::Current()->Yield(200ms);
										g_PlayerService->GetSelected()->m_CreatedAttackers.erase(g_PlayerService->GetSelected()->m_CreatedAttackers.begin() + i);
									}
								}
							});
						}
					}
					if (Elements::RegularOption("Teleport##2", ImVec2(155.f, 25.f), ImVec2(30.f, 2570.f)))
					{
						if (!g_PlayerService->GetSelected()->m_CreatedAttackers.empty())
						{
							g_ScriptQueue->Queue([]
							{
								for (int i = 0; i < g_PlayerService->GetSelected()->m_CreatedAttackers.size(); i++)
								{
									const Ped CurrentPed = g_PlayerService->GetSelected()->m_CreatedAttackers[i];
									if (ENTITY::DOES_ENTITY_EXIST(CurrentPed))
									{
										Entities::RequestControl(CurrentPed);
										Script::Current()->Yield(200ms);
										const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID()), TRUE);
										ENTITY::SET_ENTITY_COORDS(CurrentPed, Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
									}
								}
							});
						}
					}

					Elements::SubOption("Bounty Placement", ImVec2(180.f, 115.f), ImVec2(20.f, 2620.f));
					ImGui::SetCursorPos(ImVec2(30.f, 2650.f));
					ImGui::Text("Amount");
					Elements::SliderIntOption("###bounty_amount", &g_PlayerService->GetSelected()->m_BountyAmount, 1, 10000, 105.f, ImVec2(70.f, 2650.f));
					Elements::BoolOption("Anonymous", &g_PlayerService->GetSelected()->m_AnonymousBounty, ImVec2(30.f, 2680.f));
					if (Elements::RegularOption("Send", ImVec2(155.f, 25.f), ImVec2(30.f, 2710.f)))
					{
						g_ScriptQueue->Queue([] { Target::SetBounty(g_PlayerService->GetSelected(), g_PlayerService->GetSelected()->m_BountyAmount, g_PlayerService->GetSelected()->m_AnonymousBounty); });
					}

					Elements::SubOption("Sound Player", ImVec2(180.f, 185.f), ImVec2(20.f, 2760.f));
					ImGui::SetCursorPos(ImVec2(30.f, 2790.f));
					ImGui::PushItemWidth(110.f);
					Elements::ComboOption("Name", &g_PlayerService->GetSelected()->m_SoundNamesPos, g_PlayerService->GetSelected()->m_SoundNames, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_SoundNames), -1);
					ImGui::PopItemWidth();
					if (Elements::RegularOption("Start", ImVec2(155.f, 25.f), ImVec2(30.f, 2820.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID()), TRUE);
							const char* SoundName = "";
							const char* DictName = "";
							switch (g_PlayerService->GetSelected()->m_SoundNamesPos)
							{
							case 0:
								SoundName = "MP_5_SECOND_TIMER";
								DictName = "HUD_FRONTEND_DEFAULT_SOUNDSET";
								break;
							case 1:
								SoundName = "Jet_Explosions";
								DictName = "exile_1";
								break;
							case 2:
								SoundName = "Horn";
								DictName = "DLC_Apt_Yacht_Ambient_Soundset";
								break;
							}

							AUDIO::PLAY_SOUND_FROM_COORD(-1, SoundName, Coords.x, Coords.y, Coords.z, DictName, TRUE, 150, FALSE);
						});
					}
					if (Elements::RegularOption("Stop", ImVec2(155.f, 25.f), ImVec2(30.f, 2850.f)))
						g_ScriptQueue->Queue([] { AUDIO::STOP_SOUND(-1); });
					ImGui::SetCursorPos(ImVec2(30.f, 2885.f));
					ImGui::Text("Delay");
					Elements::SliderIntOption("###sound_spammer_delay", &g_PlayerService->GetSelected()->m_SoundSpammerDelay, 0, 1000, 105.f, ImVec2(65.f, 2885.f));
					Elements::BoolOption("Enable##2", &g_PlayerService->GetSelected()->m_EnableSoundSpammer, ImVec2(30.f, 2915.f));

					Elements::SubOption("Kicks", ImVec2(180.f, 175.f), ImVec2(215.f, 0.f));
					static bool g_CheaterMessage = false;
					if (Elements::RegularOption("Breakup", ImVec2(75.f, 25.f), ImVec2(225.f, 30.f)))
						Kicks::BreakupKick(g_PlayerService->GetSelected(), g_CheaterMessage);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Breakup Kick:\nUnblockable\nDetectable\nKarma Possibility\nInstant\nStrong");
					Elements::BoolOption("Cheater", &g_CheaterMessage, ImVec2(310.f, 31.f));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(std::format("Displays a '{} was detected cheating and has been removed from the session.' message.", g_PlayerService->GetSelected()->GetName()).c_str());
					if (Elements::RegularOption("Bail Kick", ImVec2(155.f, 25.f), ImVec2(225.f, 60.f)))
						Kicks::BailKick(g_PlayerService->GetSelected());
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Bail Kick:\nBlockable\nDetectable\nKarma Possibility\nInstant\nVery Weak");
					if (Elements::RegularOption("Session Host Kick", ImVec2(155.f, 25.f), ImVec2(225.f, 90.f)))
						Kicks::HostKick(g_PlayerService->GetSelected());
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Session Host Kick:\nOnly works if you're Session Host\nUnblockable\nDetectable\nKarma Possibility\nDelayed\nStrongest");
					if (Elements::RegularOption("Script Host Kick", ImVec2(155.f, 25.f), ImVec2(225.f, 120.f)))
						Kicks::ScriptHostKick(g_PlayerService->GetSelected());
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Script Host Kick:\nOnly works if you're Script Host\nForces you as Script Host\nBlockable\nDetectable\nKarma Possibility\nInstant\nMediocre");
					if (Elements::RegularOption("Pickup Kick", ImVec2(155.f, 25.f), ImVec2(225.f, 150.f)))
						Kicks::PickupKick(g_PlayerService->GetSelected());
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Pickup Kick:\nWorks on everyone that's not in an interior or vehicle\nBlockable\nDetectable\nKarma Possibility\nDelayed\nDecent");

					Elements::SubOption("Crashes", ImVec2(180.f, 175.f), ImVec2(215.f, 200.f));
					if (Elements::RegularOption("Sync Crash", ImVec2(155.f, 25.f), ImVec2(225.f, 230.f)))
						Crashes::SyncCrash(g_PlayerService->GetSelected());
					if (Elements::RegularOption("Task Crash", ImVec2(155.f, 25.f), ImVec2(225.f, 260.f)))
						Crashes::TaskCrash(g_PlayerService->GetSelected());
					if (Elements::RegularOption("Kit Crash", ImVec2(155.f, 25.f), ImVec2(225.f, 290.f)))
						Crashes::KitCrash(g_PlayerService->GetSelected());
					if (Elements::RegularOption("Pogo Crash", ImVec2(155.f, 25.f), ImVec2(225.f, 320.f)))
						Crashes::PogoCrash(g_PlayerService->GetSelected());
					if (Elements::RegularOption("Script Crash", ImVec2(155.f, 25.f), ImVec2(225.f, 350.f)))
						Crashes::ScriptCrash(g_PlayerService->GetSelected());

					Elements::SubOption("Action Denying", ImVec2(180.f, 505.f), ImVec2(215.f, 400.f));
					Elements::BoolOption("Vehicles", &g_PlayerService->GetSelected()->m_DenyVehicles, ImVec2(225.f, 430.f));
					Elements::BoolOption("Planes", &g_PlayerService->GetSelected()->m_DenyPlanes, ImVec2(225.f, 460.f));
					Elements::BoolOption("Helicopters", &g_PlayerService->GetSelected()->m_DenyHelicopters, ImVec2(225.f, 490.f));
					Elements::BoolOption("Boats", &g_PlayerService->GetSelected()->m_DenyBoats, ImVec2(225.f, 520.f));
					Elements::BoolOption("Aiming", &g_PlayerService->GetSelected()->m_DenyAiming, ImVec2(225.f, 550.f));
					Elements::BoolOption("Shooting", &g_PlayerService->GetSelected()->m_DenyShooting, ImVec2(225.f, 580.f));
					Elements::BoolOption("Cover", &g_PlayerService->GetSelected()->m_DenyCover, ImVec2(225.f, 610.f));
					Elements::BoolOption("Reloading", &g_PlayerService->GetSelected()->m_DenyReloading, ImVec2(225.f, 640.f));
					Elements::BoolOption("Switching Weapons", &g_PlayerService->GetSelected()->m_DenyWeaponSwitch, ImVec2(225.f, 670.f));
					Elements::BoolOption("Skydiving", &g_PlayerService->GetSelected()->m_DenySkydiving, ImVec2(225.f, 700.f));
					Elements::BoolOption("Jumping", &g_PlayerService->GetSelected()->m_DenyJumping, ImVec2(225.f, 730.f));
					Elements::BoolOption("Swimming", &g_PlayerService->GetSelected()->m_DenySwimming, ImVec2(225.f, 760.f));
					Elements::BoolOption("Sprinting", &g_PlayerService->GetSelected()->m_DenySprinting, ImVec2(225.f, 790.f));
					Elements::BoolOption("Walking", &g_PlayerService->GetSelected()->m_DenyWalking, ImVec2(225.f, 820.f));
					Elements::BoolOption("Ragdolling", &g_PlayerService->GetSelected()->m_DenyRagdoll, ImVec2(225.f, 850.f));
					Elements::BoolOption("Climbing", &g_PlayerService->GetSelected()->m_DenyClimbing, ImVec2(225.f, 880.f));

					Elements::SubOption("Explosion Creator", ImVec2(180.f, 295.f), ImVec2(215.f, 930.f));
					ImGui::SetCursorPos(ImVec2(225.f, 960.f));
					ImGui::PushItemWidth(105.f);
					Elements::ComboOption("Tag", &g_PlayerService->GetSelected()->m_ExplosionTagPos, g_PlayerService->GetSelected()->m_ExplosionTag, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_ExplosionTag), -1);
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(225.f, 990.f));
					ImGui::PushItemWidth(105.f);
					Elements::InputFloatOption("Damage", &g_PlayerService->GetSelected()->m_DamageScale, 1.f, 9999.f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(225.f, 1020.f));
					ImGui::PushItemWidth(105.f);
					Elements::InputFloatOption("Shake", &g_PlayerService->GetSelected()->m_CameraShake, 1.f, 9999.f, "%.2f");
					ImGui::PopItemWidth();
					Elements::BoolOption("Audible", &g_PlayerService->GetSelected()->m_Audible, ImVec2(225.f, 1050.f));
					Elements::BoolOption("Visible", &g_PlayerService->GetSelected()->m_Visible, ImVec2(225.f, 1080.f));
					Elements::BoolOption("Take Blame", &g_PlayerService->GetSelected()->m_TakeBlame, ImVec2(225.f, 1110.f));
					Elements::BoolOption("On Player Ped", &g_PlayerService->GetSelected()->m_OnPlayer, ImVec2(225.f, 1140.f));
					Elements::BoolOption("On Bullet Impact", &g_PlayerService->GetSelected()->m_WeaponImpact, ImVec2(225.f, 1170.f));
					if (Elements::RegularOption("Once On Player Ped", ImVec2(155.f, 25.f), ImVec2(225.f, 1200.f)))
					{
						g_ScriptQueue->Queue([] 
						{ 
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

							if (g_PlayerService->GetSelected()->m_TakeBlame)
								FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, g_PlayerService->GetSelected()->m_ExplosionTagPos, g_PlayerService->GetSelected()->m_DamageScale, g_PlayerService->GetSelected()->m_Audible, !g_PlayerService->GetSelected()->m_Visible, g_PlayerService->GetSelected()->m_CameraShake, FALSE);
							else
								Target::BlameExplodeCoord(g_PlayerService->GetSelected(), Coords, (eExplosionTag)g_PlayerService->GetSelected()->m_ExplosionTagPos, g_PlayerService->GetSelected()->m_DamageScale, g_PlayerService->GetSelected()->m_Audible, !g_PlayerService->GetSelected()->m_Visible, g_PlayerService->GetSelected()->m_CameraShake);
						});
					}

					Elements::SubOption("Remote Events", ImVec2(180.f, 105.f), ImVec2(215.f, 1250.f));
					ImGui::SetCursorPos(ImVec2(225.f, 1280.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###re_array", &g_PlayerService->GetSelected()->m_RemoteEventPos, g_PlayerService->GetSelected()->m_RemoteEvent, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_RemoteEvent), -1);
					ImGui::PopItemWidth();
					if (Elements::RegularOption("Send Once", ImVec2(155.f, 25.f), ImVec2(225.f, 1310.f)))
						g_ScriptQueue->Queue([] { Target::SendRemoteEvent(g_PlayerService->GetSelected(), g_PlayerService->GetSelected()->m_RemoteEventPos); });
					Elements::BoolOption("Send", &g_PlayerService->GetSelected()->m_LoopedRemoteEvents, ImVec2(225.f, 1340.f));

					Elements::SubOption("Angry Planes", ImVec2(180.f, 175.f), ImVec2(215.f, 1380.f));
					if (Elements::BoolOption("Enable##3", &g_PlayerService->GetSelected()->m_EnableAngryPlanes, ImVec2(225.f, 1410.f)))
					{
						if (!g_PlayerService->GetSelected()->m_EnableAngryPlanes)
							g_ScriptQueue->Queue([] { g_AngryPlanes.Destroy(); });
					}
					Elements::BoolOption("Rockets", &g_PlayerService->GetSelected()->m_SpawnWithRockets, ImVec2(225.f, 1440.f));
					Elements::BoolOption("Invincible##1", &g_PlayerService->GetSelected()->m_SpawnInvincible, ImVec2(225.f, 1470.f));
					Elements::BoolOption("Invisible##2", &g_PlayerService->GetSelected()->m_SpawnInvisible, ImVec2(225.f, 1500.f));
					ImGui::SetCursorPos(ImVec2(225.f, 1530.f));
					ImGui::Text("Amount");
					Elements::SliderIntOption("###angryplanes_amount", &g_PlayerService->GetSelected()->m_AmountOfPlanes, 1, 30, 105.f, ImVec2(265.f, 1530.f));

					Elements::SubOption("Friendly", ImVec2(180.f, 295.f), ImVec2(215.f, 1580.f));
					Elements::BoolOption("Semi God", &g_PlayerService->GetSelected()->m_SemiGod, ImVec2(225.f, 1610.f));
					Elements::BoolOption("Unbreakable Armor", &g_PlayerService->GetSelected()->m_UnbreakableArmor, ImVec2(225.f, 1640.f));
					Elements::BoolOption("Never Wanted", &g_PlayerService->GetSelected()->m_NeverWanted, ImVec2(225.f, 1670.f));
					if (Elements::BoolOption("Off Radar", &g_PlayerService->GetSelected()->m_OffRadar, ImVec2(225.f, 1700.f)))
					{
						if (!g_PlayerService->GetSelected()->m_OffRadar)
						{
							g_ScriptQueue->Queue([]
							{
								*ScriptGlobal(2657589).At(g_PlayerService->GetSelected()->ID(), 466).Add(210).As<std::int32_t*>() = 0;
								int Bitset = *ScriptGlobal(2672505).Add(57).As<std::int32_t*>() = 0;
								MISC::SET_BIT(&Bitset, g_PlayerService->GetSelected()->ID());
							});
						}
					}
					if (Elements::BoolOption("Reveal Players", &g_PlayerService->GetSelected()->m_RevealPlayers, ImVec2(225.f, 1730.f)))
					{
						if (!g_PlayerService->GetSelected()->m_RevealPlayers)
						{
							g_ScriptQueue->Queue([]
							{
								*ScriptGlobal(2657589).At(g_PlayerService->GetSelected()->ID(), 466).Add(213).As<std::int32_t*>() = 0;
								int Bitset = *ScriptGlobal(2672505).Add(58).As<std::int32_t*>() = 0;
								MISC::SET_BIT(&Bitset, g_PlayerService->GetSelected()->ID());
							});
						}
					}
					if (Elements::RegularOption("Give Health", ImVec2(155.f, 25.f), ImVec2(225.f, 1760.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const CPed* PlayerPed = g_PlayerService->GetSelected()->GetPed();

							if (PlayerPed->m_health < PlayerPed->m_maxhealth)
							{
								Pointers::pGivePickupRewards(1 << g_PlayerService->GetSelected()->ID(), rage::joaat("REWARD_HEALTH"));
							}
						});
					}
					if (Elements::RegularOption("Give Armor", ImVec2(155.f, 25.f), ImVec2(225.f, 1790.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const CPed* PlayerPed = g_PlayerService->GetSelected()->GetPed();

							if (PlayerPed->m_armor < PLAYER::GET_PLAYER_MAX_ARMOUR(g_PlayerService->GetSelected()->ID()))
							{
								Pointers::pGivePickupRewards(1 << g_PlayerService->GetSelected()->ID(), rage::joaat("REWARD_ARMOUR"));
							}
						});
					}
					if (Elements::RegularOption("Clear Wanted Level", ImVec2(155.f, 25.f), ImVec2(225.f, 1820.f)))
					{
						g_ScriptQueue->Queue([]
						{
							if (PLAYER::GET_PLAYER_WANTED_LEVEL(g_PlayerService->GetSelected()->ID()) > 0)
								Target::ClearPlayerWantedLevel(g_PlayerService->GetSelected()->ID());
						});
					}
					if (Elements::RegularOption("Copy Outfit", ImVec2(155.f, 25.f), ImVec2(225.f, 1850.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Self = PLAYER::PLAYER_PED_ID();
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());

							if (ENTITY::GET_ENTITY_MODEL(Self) != ENTITY::GET_ENTITY_MODEL(Ped))
								return;
							for (int i = 0; i < 12; i++)
								PED::SET_PED_COMPONENT_VARIATION(Self, i, PED::GET_PED_DRAWABLE_VARIATION(Ped, i), PED::GET_PED_TEXTURE_VARIATION(Ped, i), PED::GET_PED_PALETTE_VARIATION(Ped, i));
						});
					}

					Elements::SubOption("Drop Manager", ImVec2(180.f, 175.f), ImVec2(215.f, 1900.f));
					ImGui::SetCursorPos(ImVec2(225.f, 1930.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###drop_type", &g_PlayerService->GetSelected()->m_DropTypePos, g_PlayerService->GetSelected()->m_DropType, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_DropType), -1);
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(225.f, 1960.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###drop_model", &g_PlayerService->GetSelected()->m_DropModelPos, g_PlayerService->GetSelected()->m_DropModel, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_DropModel), -1);
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(225.f, 1990.f));
					ImGui::PushItemWidth(155.f);
					Elements::ComboOption("###drop_style", &g_PlayerService->GetSelected()->m_DropStylePos, g_PlayerService->GetSelected()->m_DropStyle, IM_ARRAYSIZE(g_PlayerService->GetSelected()->m_DropStyle), -1);
					ImGui::PopItemWidth();
					ImGui::SetCursorPos(ImVec2(225.f, 2020.f));
					ImGui::Text("Delay");
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Leave on 2.5s delay for money drop to prevent error messages.");
					Elements::SliderIntOption("###drop_delay", &g_PlayerService->GetSelected()->m_DropDelay, 0, 10000, 105.f, ImVec2(260.f, 2020.f));
					Elements::BoolOption("Drop", &g_PlayerService->GetSelected()->m_StartDropping, ImVec2(225.f, 2045.f));

					Elements::SubOption("Forcefield", ImVec2(180.f, 175.f), ImVec2(215.f, 2100.f));
					ImGui::SetCursorPos(ImVec2(225.f, 2133.f));
					ImGui::Text("Range");
					Elements::SliderFloatOption("###force_field_range", &g_PlayerService->GetSelected()->m_ForcefieldRange, 1.f, 100.f, 105.f, ImVec2(260.f, 2133.f));
					Elements::BoolOption("Draw Sphere", &g_PlayerService->GetSelected()->m_ForcefieldSphere, ImVec2(225.f, 2155.f));
					Elements::BoolOption("Target Peds", &g_PlayerService->GetSelected()->m_ForcefieldPeds, ImVec2(225.f, 2185.f));
					Elements::BoolOption("Target Vehicles", &g_PlayerService->GetSelected()->m_ForcefieldVehicles, ImVec2(225.f, 2215.f));
					Elements::BoolOption("Enable##4", &g_PlayerService->GetSelected()->m_EnableForcefield, ImVec2(225.f, 2245.f));

					Elements::SubOption("Weapon", ImVec2(180.f, 205.f), ImVec2(215.f, 2300.f));
					Elements::BoolOption("Bullet Redirection", &g_PlayerService->GetSelected()->m_BulletRedirection, ImVec2(225.f, 2330.f));
					Elements::BoolOption("Ped Gun", &g_PlayerService->GetSelected()->m_PedGun, ImVec2(225.f, 2360.f));
					Elements::BoolOption("Airstrike Gun", &g_PlayerService->GetSelected()->m_AirstrikeGun, ImVec2(225.f, 2390.f));
					Elements::BoolOption("Teleport Gun", &g_PlayerService->GetSelected()->m_TeleportGun, ImVec2(225.f, 2420.f));
					Elements::BoolOption("Money Gun", &g_PlayerService->GetSelected()->m_MoneyGun, ImVec2(225.f, 2450.f));
					Elements::BoolOption("Fake Money Gun", &g_PlayerService->GetSelected()->m_FakeMoneyGun, ImVec2(225.f, 2480.f));

					Elements::SubOption("Vehicle", ImVec2(180.f, 415.f), ImVec2(215.f, 2530.f));
					if (Elements::BoolOption("Invincibility", &g_PlayerService->GetSelected()->m_VehicleInvincibility, ImVec2(225.f, 2560.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								ENTITY::SET_ENTITY_INVINCIBLE(Vehicle, g_PlayerService->GetSelected()->m_VehicleInvincibility);
							}
						});
					}
					if (Elements::BoolOption("Disorient Handling", &g_PlayerService->GetSelected()->m_DisorientHandling, ImVec2(225.f, 2590.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_STEER_BIAS(Vehicle, g_PlayerService->GetSelected()->m_DisorientHandling ? 1.f : 0.f);
							}
						});
					}
					if (Elements::BoolOption("No Collision", &g_PlayerService->GetSelected()->m_NoCollision, ImVec2(225.f, 2620.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								ENTITY::SET_ENTITY_COLLISION(Vehicle, !g_PlayerService->GetSelected()->m_NoCollision, !g_PlayerService->GetSelected()->m_NoCollision);
							}
						});
					}
					if (Elements::BoolOption("Slippy Wheels", &g_PlayerService->GetSelected()->m_SlippyWheels, ImVec2(225.f, 2650.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_REDUCE_GRIP(Vehicle, g_PlayerService->GetSelected()->m_SlippyWheels);
							}
						});
					}
					if (Elements::RegularOption("Slash Tires", ImVec2(155.f, 25.f), ImVec2(225.f, 2680.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, TRUE);
								for (int i = 0; i < 8; i++)
								{
									Entities::RequestControl(Vehicle);
									VEHICLE::SET_VEHICLE_TYRE_BURST(Vehicle, i, TRUE, 1000.f);
								}
							}
						});
					}
					if (Elements::RegularOption("Kick Driver", ImVec2(155.f, 25.f), ImVec2(225.f, 2710.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, -1, FALSE))
								{
									const auto Target = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, -1, FALSE);
									Target::SendRemoteEvent(g_PlayerService->GetByPed(Target), 3);
								}			
							}
						});
					}
					if (Elements::RegularOption("Kick All Seats", ImVec2(155.f, 25.f), ImVec2(225.f, 2740.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								for (int i = -1; i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(Vehicle); i++) {
									if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, i, FALSE)) {
										const auto Target = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, i, FALSE);
										Target::SendRemoteEvent(g_PlayerService->GetByPed(Target), 3);
									}
								}
							}
						});
					}
					if (Elements::RegularOption("Lock Doors", ImVec2(155.f, 25.f), ImVec2(225.f, 2770.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_DOORS_LOCKED(Vehicle, 4);
							}
						});
					}
					if (Elements::RegularOption("Unlock Doors", ImVec2(155.f, 25.f), ImVec2(225.f, 2800.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_DOORS_LOCKED(Vehicle, 0);
							}
						});
					}
					if (Elements::RegularOption("Place on Ground", ImVec2(155.f, 25.f), ImVec2(225.f, 2830.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, 0.f);
							}
						});
					}
					if (Elements::RegularOption("Hijack", ImVec2(155.f, 25.f), ImVec2(225.f, 2860.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, -1, FALSE))
								{
									const auto Target = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Vehicle, -1, FALSE);
									Target::SendRemoteEvent(g_PlayerService->GetByPed(Target), 3);
								}
								while (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, -1, FALSE))
								{
									Script::Current()->Yield(1ms);
								}
								PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Vehicle, -1);
								VEHICLE::SET_VEHICLE_ENGINE_ON(Vehicle, TRUE, TRUE, FALSE);
							}
						});
					}
					if (Elements::RegularOption("Repair", ImVec2(155.f, 25.f), ImVec2(225.f, 2890.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								const auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::RequestControl(Vehicle);
								VEHICLE::SET_VEHICLE_FIXED(Vehicle);
								VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
								VEHICLE::SET_VEHICLE_UNDRIVEABLE(Vehicle, FALSE);
								VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0.f);
								VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, "Sentinel");
							}
						});
					}
					if (Elements::RegularOption("Vanish", ImVec2(155.f, 25.f), ImVec2(225.f, 2920.f)))
					{
						g_ScriptQueue->Queue([]
						{
							const NativeVector3 Ocean = { -6233.433f, -5388.144f, -105.988304f };
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
							{
								auto Vehicle = PED::GET_VEHICLE_PED_IS_USING(Ped);
								Entities::ChangeNetworkObjectOwner(Vehicle, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
								while (!VEHICLE::IS_VEHICLE_SEAT_FREE(Vehicle, -1, FALSE))
								{
									Script::Current()->Yield(1ms);
								}
								Entities::RequestControl(Vehicle);
								ENTITY::SET_ENTITY_COORDS(Vehicle, Ocean.x, Ocean.y, Ocean.z, FALSE, FALSE, FALSE, FALSE);
							}
						});
					}

					ImGui::Dummy(ImVec2(0.f, 10.f));
				}
			}
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}