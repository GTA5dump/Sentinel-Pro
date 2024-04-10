#include "Players.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/enums.hpp"
#include "../Pointers/Pointers.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/Script/ScriptLocal.hpp"
#include "../GTA/Script/ScriptQueue.hpp"
#include "../Utility/Timer.hpp"
#include "../Utility/Joaat.hpp"
#include "../Utility/Math.hpp"
#include "../Utility/All.hpp"
#include "AngryPlanes.hpp"

namespace Sentinel
{
	void PlayersFeatures::PlayerInfo()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		// Self Info
		g_PlayerService->GetSelf()->m_ScriptHost = (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0) == PLAYER::PLAYER_ID());

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			// Script Host
			player.second->m_ScriptHost = (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0) == player.second->ID());
		});
	}

	static bool g_LastSpectate = true;
	void PlayersFeatures::Spectate(bool enable)
	{
		const auto MyVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
		const auto MyPed = PLAYER::PLAYER_PED_ID();

		if (!g_PlayerService->GetSelected()->IsValid() || !m_Spectate)
		{
			if (m_Spectate)
				m_Spectate = false;

			if (!g_LastSpectate)
			{
				g_LastSpectate = true;

				NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(FALSE, -1);
				HUD::SET_MINIMAP_IN_SPECTATOR_MODE(FALSE, -1);

				ENTITY::FREEZE_ENTITY_POSITION(MyPed, FALSE);
				ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, FALSE);

				STREAMING::SET_FOCUS_ENTITY(MyPed);
			}

			return;
		}

		const auto Target = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());

		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(TRUE, Target);
		HUD::SET_MINIMAP_IN_SPECTATOR_MODE(TRUE, Target);

		ENTITY::FREEZE_ENTITY_POSITION(MyPed, TRUE);
		ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, TRUE);

		STREAMING::SET_FOCUS_ENTITY(Target);

		g_LastSpectate = false;
	}

	void PlayersFeatures::IterateTrollingOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_KillPlayer && !(player.second->GetPed()->m_damage_bits & (1 << 8)))
					Pointers::pSendNetworkDamage((CEntity*)g_PlayerService->GetSelf()->GetPed(), (CEntity*)player.second->GetPed(), player.second->GetPed()->m_navigation->get_position(),
						0, true, "weapon_explosion"_J, 10000.f, 2, 0, (1 << 4), 0, 0, 0, false, false, true, true, nullptr);

				if (player.second->m_FreezePlayer && player.second->GetPed()->m_net_object)
					Pointers::pClearPedTasksNetworked(player.second->GetPed(), true);

				if (player.second->m_RagdollPlayer && player.second->GetPed()->m_net_object)
					Pointers::pRequestRagdoll(player.second->GetPed()->m_net_object->m_object_id);

				if (player.second->m_ExplodePlayer)
					Target::BlameExplodePlayer(player.second, player.second, EXP_TAG_SUBMARINE_BIG, 9999.f, true, false, 0.f);

				if (player.second->m_BurnPlayer)
				{
					static Timer burnTimer(0ms);
					burnTimer.SetDelay(std::chrono::seconds(5));
					if (burnTimer.Update())
						Target::BurnPlayer(player.second);
				}

				if (player.second->m_FlameRay)
					Target::BlameExplodePlayer(player.second, player.second, DIR_FLAME, 5.f, true, false, 0.f);

				if (player.second->m_WaterHydrant)
					Target::BlameExplodePlayer(player.second, player.second, DIR_WATER_HYDRANT, 5.f, true, false, 0.f);

				if (player.second->m_ShakeCamera)
					Target::BlameExplodePlayer(player.second, player.second, EXP_TAG_SUBMARINE_BIG, 0.f, false, true, 9999.f);

				if (player.second->m_GlitchPhysics)
				{
					static Timer glitchTimer(0ms);
					glitchTimer.SetDelay(std::chrono::seconds(1));
					if (glitchTimer.Update())
					{
						const auto Model = 0xD5C0BC07;
						const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
						const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
						const auto Guitar = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(Coords.x, Coords.y, Coords.z, 5.f, Model, FALSE, FALSE, FALSE);
						if (!Guitar || !ENTITY::DOES_ENTITY_EXIST(Guitar) || !ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(Guitar, Ped)) {
							while (!STREAMING::HAS_MODEL_LOADED(Model))
							{
								STREAMING::REQUEST_MODEL(Model);
								Script::Current()->Yield(5ms);
							}
							System::ModelSpawnBypass(true);
							const auto CreatedGuitar = OBJECT::CREATE_OBJECT(Model, Coords.x, Coords.y, Coords.z, TRUE, TRUE, FALSE);
							System::ModelSpawnBypass(false);
							if (ENTITY::DOES_ENTITY_EXIST(CreatedGuitar))
							{
								ENTITY::SET_ENTITY_VISIBLE(CreatedGuitar, FALSE, FALSE);
								ENTITY::ATTACH_ENTITY_TO_ENTITY(CreatedGuitar, Ped, PED::GET_PED_BONE_INDEX(Ped, 23553), 0.5f, -0.2f, 0.f, 0.f, 75.0f, 0.f, TRUE, TRUE, FALSE, TRUE, 1, TRUE, NULL);
								ENTITY::SET_ENTITY_COLLISION(CreatedGuitar, TRUE, TRUE);
							}
						}
					}
				}

				if (player.second->m_RainRockets)
				{
					static Timer rocketTimer(0ms);
					rocketTimer.SetDelay(std::chrono::milliseconds(250));
					if (rocketTimer.Update())
					{
						const auto WeaponHash = Joaat("WEAPON_AIRSTRIKE_ROCKET");
						const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
						const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

						while (!WEAPON::HAS_WEAPON_ASSET_LOADED(WeaponHash))
						{
							WEAPON::REQUEST_WEAPON_ASSET(WeaponHash, 31, NULL);
							Script::Current()->Yield(5ms);
						}

						const auto RandomFloat = MISC::GET_RANDOM_FLOAT_IN_RANGE(-20.f, 20.f);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(Coords.x + RandomFloat, Coords.y + RandomFloat, Coords.z + 50.f, Coords.x + RandomFloat, Coords.y + RandomFloat, Coords.z, 250, TRUE, WeaponHash, PLAYER::PLAYER_PED_ID(), TRUE, FALSE, -1.f, PLAYER::PLAYER_PED_ID(), NULL);
						WEAPON::REMOVE_WEAPON_ASSET(WeaponHash);
					}
				}

				if (player.second->m_CloneSpam)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_PlayerService->GetSelected()->ID());
					static Timer cloneTimer(0ms);
					cloneTimer.SetDelay(std::chrono::milliseconds(750));
					if (cloneTimer.Update())
					{
						PED::CLONE_PED(Ped, TRUE, FALSE, TRUE);
					}
				}

				if (player.second->m_GiveWantedLevel)
				{
					PLAYER::REPORT_CRIME(player.second->ID(), 8, PLAYER::GET_WANTED_LEVEL_THRESHOLD(player.second->m_WantedLevelToGive));
				}

				if (player.second->GetPed()->m_net_object && player.second->m_Glitchfield)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
					FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 0x08, FLT_MAX, FALSE, TRUE, 0.f, TRUE);
				}
			}
		});
	}

	void PlayersFeatures::IterateShootingOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->GetPed()->m_net_object && player.second->m_LoopShoot)
				{
					const auto WeaponHash = Lists::g_ShootBulletHashes[g_PlayerService->GetSelected()->m_ShootBulletPos];

					static Timer shootTimer(0ms);
					shootTimer.SetDelay(std::chrono::milliseconds(750));
					if (shootTimer.Update())
						Target::ShootPlayer(g_PlayerService->GetSelected(), WeaponHash);
				}
			}
		});
	}

	void PlayersFeatures::IterateActionDenyingOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->GetPed()->m_net_object)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					const auto CPed = player.second->GetPed();
					if (player.second->m_DenyVehicles)
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
						{
							Target::SendRemoteEvent(player.second, 3);
						}
					}

					if (player.second->m_DenyPlanes)
					{
						if (PED::IS_PED_IN_ANY_PLANE(Ped))
						{
							Target::SendRemoteEvent(player.second, 3);
						}
					}

					if (player.second->m_DenyHelicopters)
					{
						if (PED::IS_PED_IN_ANY_HELI(Ped))
						{
							Target::SendRemoteEvent(player.second, 3);
						}
					}

					if (player.second->m_DenyBoats)
					{
						if (PED::IS_PED_IN_ANY_BOAT(Ped))
						{
							Target::SendRemoteEvent(player.second, 3);
						}
					}

					if (player.second->m_DenyAiming)
					{
						if (Weapons::IsPedAiming(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyShooting)
					{
						if (Weapons::IsPedShooting(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyCover)
					{
						if (PED::IS_PED_IN_COVER(Ped, FALSE))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyReloading)
					{
						if (PED::IS_PED_RELOADING(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyWeaponSwitch)
					{
						if (PED::IS_PED_SWITCHING_WEAPON(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenySkydiving)
					{
						if (PED::IS_PED_IN_PARACHUTE_FREE_FALL(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyJumping)
					{
						if (PED::IS_PED_JUMPING(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenySwimming)
					{
						if (PED::IS_PED_SWIMMING(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenySprinting)
					{
						if (TASK::IS_PED_RUNNING(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyWalking)
					{
						if (TASK::IS_PED_WALKING(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyRagdoll)
					{
						if (PED::IS_PED_RAGDOLL(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}

					if (player.second->m_DenyClimbing)
					{
						if (PED::IS_PED_CLIMBING(Ped))
							Pointers::pClearPedTasksNetworked(CPed, true);
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateExplosionCreatorOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_OnPlayer)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					const auto Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);

					if (player.second->m_TakeBlame)
						FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, player.second->m_ExplosionTagPos, player.second->m_DamageScale, player.second->m_Audible, !player.second->m_Visible, player.second->m_CameraShake, FALSE);
					else
						Target::BlameExplodeCoord(player.second, Coords, (eExplosionTag)player.second->m_ExplosionTagPos, player.second->m_DamageScale, player.second->m_Audible, !player.second->m_Visible, player.second->m_CameraShake);
				}

				if (player.second->m_WeaponImpact)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					NativeVector3 Coords{};
					if (Weapons::IsPedShooting(Ped) && WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Ped, &Coords))
					{
						if (player.second->m_TakeBlame)
							FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, player.second->m_ExplosionTagPos, player.second->m_DamageScale, player.second->m_Audible, !player.second->m_Visible, player.second->m_CameraShake, FALSE);
						else
							Target::BlameExplodeCoord(player.second, Coords, (eExplosionTag)player.second->m_ExplosionTagPos, player.second->m_DamageScale, player.second->m_Audible, !player.second->m_Visible, player.second->m_CameraShake);
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateRemoteOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_LoopedRemoteEvents)
				{
					static Timer eventTimer(0ms);
					eventTimer.SetDelay(std::chrono::milliseconds(750));
					if (eventTimer.Update())
						Target::SendRemoteEvent(player.second, player.second->m_RemoteEventPos);
				}
			}
		});
	}

	void PlayersFeatures::IterateEntitySpammer()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_EntitySpammerEnable)
				{
					static Timer spamTimer(0ms);
					spamTimer.SetDelay(std::chrono::milliseconds(player.second->m_EntitySpammerDelay));
					if (spamTimer.Update())
					{
						const auto Target = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
						const auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Target, 0.f, 0.f, 2.f);
						const auto Pos = player.second->m_EntitySpammerPos;

						// Mexicans
						if (Pos == 0)
						{
							const auto Hash = Lists::g_MexicanHashes[rand() % Lists::g_MexicanHashes.size() + 1];
							while (!STREAMING::HAS_MODEL_LOADED(Hash))
							{
								STREAMING::REQUEST_MODEL(Hash);
								Script::Current()->Yield(5ms);
							}

							System::ModelSpawnBypass(true);
							const auto CreatedPed = PED::CREATE_PED(26, Hash, Coords.x, Coords.y, Coords.z, ENTITY::GET_ENTITY_HEADING(Target), TRUE, FALSE);
							System::ModelSpawnBypass(false);

							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
							player.second->m_CreatedEntities.push_back(CreatedPed);
						}
						// Blacks
						else if (Pos == 1)
						{
							const auto Hash = Lists::g_BlackHashes[rand() % Lists::g_BlackHashes.size() + 1];
							while (!STREAMING::HAS_MODEL_LOADED(Hash))
							{
								STREAMING::REQUEST_MODEL(Hash);
								Script::Current()->Yield(5ms);
							}

							System::ModelSpawnBypass(true);
							const auto CreatedPed = PED::CREATE_PED(26, Hash, Coords.x, Coords.y, Coords.z, ENTITY::GET_ENTITY_HEADING(Target), TRUE, FALSE);
							System::ModelSpawnBypass(false);

							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
							player.second->m_CreatedEntities.push_back(CreatedPed);
						}
						// Clones
						else if (Pos == 2)
						{
							System::ModelSpawnBypass(true);
							const auto CreatedPed = PED::CLONE_PED(Target, TRUE, FALSE, FALSE);
							System::ModelSpawnBypass(false);

							player.second->m_CreatedEntities.push_back(CreatedPed);
						}
						// Vehicles
						else if (Pos > 2)
						{
							Hash EntityHashes{};
							switch (Pos)
							{
							case 3:
								EntityHashes = Lists::g_PlaneHashes[rand() % Lists::g_PlaneHashes.size() + 1];
								break;
							case 4:
								EntityHashes = Lists::g_HelicopterHashes[rand() % Lists::g_HelicopterHashes.size() + 1];
								break;
							case 5:
								EntityHashes = Lists::g_BoatHashes[rand() % Lists::g_BoatHashes.size() + 1];
								break;
							case 6:
								EntityHashes = Lists::g_SuperHashes[rand() % Lists::g_SuperHashes.size() + 1];
								break;
							case 7:
								EntityHashes = Lists::g_MilitaryHashes[rand() % Lists::g_MilitaryHashes.size() + 1];
								break;
							}

							while (!STREAMING::HAS_MODEL_LOADED(EntityHashes))
							{
								STREAMING::REQUEST_MODEL(EntityHashes);
								Script::Current()->Yield(5ms);
							}

							System::ModelSpawnBypass(true);
							const auto CreatedVehicle = VEHICLE::CREATE_VEHICLE(EntityHashes, Coords.x, Coords.y, Coords.z, ENTITY::GET_ENTITY_HEADING(Target), TRUE, FALSE, FALSE);
							System::ModelSpawnBypass(false);
							Script::Current()->Yield();

							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(EntityHashes);

							if (*Pointers::pIsSessionStarted)
							{
								DECORATOR::DECOR_SET_INT(CreatedVehicle, "MPBitset", 0);
								ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(CreatedVehicle, TRUE);
								const auto NID = NETWORK::VEH_TO_NET(CreatedVehicle);
								if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(CreatedVehicle))
									NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
								VEHICLE::SET_VEHICLE_IS_STOLEN(CreatedVehicle, FALSE);
							}

							player.second->m_CreatedEntities.push_back(CreatedVehicle);
						}
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateChatSpammer()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_StartSpamming)
				{
					PLAYER::SET_PLAYER_TEAM(PLAYER::PLAYER_ID(), 1);
					PLAYER::SET_PLAYER_TEAM(player.second->ID(), 1);

					static Timer spamTimer(0ms);
					spamTimer.SetDelay(std::chrono::milliseconds(player.second->m_SpamDelay));
					if (spamTimer.Update())
					{
						const auto Message = player.second->m_ChatMessage;
						if (const auto GamePlayer = GTA::GetNetworkPlayerMgr()->m_local_net_player; GamePlayer)
						{
							if (Hooking::GetHook<DetourHook>("SCM")->Original<decltype(&GTA::SendChatMessage)>()(*Pointers::pSendChatMessagePointer, GamePlayer->get_net_data(), Message, true))
							{
								if (player.second->m_DrawLocally)
									System::DrawChat(Message, GamePlayer->get_name(), true);
							}
						}
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateSoundSpammer()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_EnableSoundSpammer)
				{
					static Timer spamTimer(0ms);
					spamTimer.SetDelay(std::chrono::milliseconds(player.second->m_SoundSpammerDelay));
					if (spamTimer.Update())
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
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateAngryPlanes()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_EnableAngryPlanes)
					g_AngryPlanes.Tick(player.second);
			}
		});
	}

	void PlayersFeatures::IterateFriendlyOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_SemiGod)
				{
					const CPed* PlayerPed = player.second->GetPed();

					if (PlayerPed->m_health < PlayerPed->m_maxhealth)
					{
						Pointers::pGivePickupRewards(1 << player.second->ID(), rage::joaat("REWARD_HEALTH"));
					}
				}

				if (player.second->m_UnbreakableArmor)
				{
					const CPed* PlayerPed = player.second->GetPed();

					if (PlayerPed->m_armor < PLAYER::GET_PLAYER_MAX_ARMOUR(player.second->ID()))
					{
						Pointers::pGivePickupRewards(1 << player.second->ID(), rage::joaat("REWARD_ARMOUR"));
					}
				}

				if (player.second->m_NeverWanted)
				{
					if (PLAYER::GET_PLAYER_WANTED_LEVEL(player.second->ID()) > 0)
						Target::ClearPlayerWantedLevel(player.second->ID());
				}

				if (player.second->m_OffRadar)
				{
					*ScriptGlobal(2657589).At(player.second->ID(), 466).Add(210).As<std::int32_t*>() = 1;
					int Bitset = *ScriptGlobal(2672505).Add(57).As<std::int32_t*>() = NETWORK::GET_NETWORK_TIME() + 60;
					MISC::SET_BIT(&Bitset, player.second->ID());
				}

				if (player.second->m_RevealPlayers)
				{
					*ScriptGlobal(2657589).At(player.second->ID(), 466).Add(213).As<std::int32_t*>() = 1;
					int Bitset = *ScriptGlobal(2672505).Add(58).As<std::int32_t*>() = NETWORK::GET_NETWORK_TIME() + 60;
					MISC::SET_BIT(&Bitset, player.second->ID());
				}
			}
		});
	}

	void PlayersFeatures::IterateDropManager()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_StartDropping)
				{
					Hash ModelHash = Lists::g_MoneyDropHashes[player.second->m_DropModelPos];
					NativeVector3 Coords{};
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());		

					switch (player.second->m_DropStylePos)
					{
					case 0:
						Coords = ENTITY::GET_ENTITY_COORDS(Ped, TRUE);
						break;
					case 1:
						Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Ped, MISC::GET_RANDOM_FLOAT_IN_RANGE(-3.f, 3.f), MISC::GET_RANDOM_FLOAT_IN_RANGE(-3.f, 3.f), MISC::GET_RANDOM_FLOAT_IN_RANGE(-3.f, 3.f));
						break;
					case 2:
						Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Ped, 0.f, 0.f, 2.f);
						break;
					case 3:
						Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Ped, 0.f, 2.f, 0.f);
						break;
					}

					static Timer dropTimer(0ms);
					dropTimer.SetDelay(std::chrono::milliseconds(player.second->m_DropDelay));
					if (dropTimer.Update())
					{
						if (player.second->m_DropTypePos == 0)
						{
							while (!STREAMING::HAS_MODEL_LOADED(ModelHash))
							{
								STREAMING::REQUEST_MODEL(ModelHash);
								Script::Current()->Yield(5ms);
							}

							OBJECT::CREATE_AMBIENT_PICKUP(0x65948212, Coords.x, Coords.y, Coords.z, 0, 2500, ModelHash, FALSE, TRUE);
						}
						else if (player.second->m_DropTypePos == 1)
						{
							const auto RPHash = Joaat("vw_prop_vw_colle_alien");
							while (!STREAMING::HAS_MODEL_LOADED(RPHash))
							{
								STREAMING::REQUEST_MODEL(RPHash);
								Script::Current()->Yield(5ms);
							}

							OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, Coords.x, Coords.y, Coords.z, 0, 10, RPHash, FALSE, TRUE);
						}
						else if (player.second->m_DropTypePos == 2)
						{
							while (!STREAMING::HAS_MODEL_LOADED(ModelHash))
							{
								STREAMING::REQUEST_MODEL(ModelHash);
								Script::Current()->Yield(5ms);
							}

							OBJECT::CREATE_AMBIENT_PICKUP(0xE175C698, Coords.x, Coords.y, Coords.z, 0, 0, ModelHash, FALSE, TRUE);
						}
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateForcefield()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_EnableForcefield)
				{
					const auto PlayerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					const auto TheirCoords = ENTITY::GET_ENTITY_COORDS(PlayerPed, TRUE);
					const auto Pitch = ENTITY::GET_ENTITY_PITCH(PlayerPed);
					const auto Yaw = ENTITY::GET_ENTITY_HEADING(PlayerPed);
					STREAMING::SET_FOCUS_POS_AND_VEL(TheirCoords.x, TheirCoords.y, TheirCoords.z, 0.f, 0.f, 0.f);
					if (player.second->m_ForcefieldSphere)
						GRAPHICS::DRAW_MARKER_SPHERE(TheirCoords.x, TheirCoords.y, TheirCoords.z, player.second->m_ForcefieldRange, g_SelfFeatures->m_SuperPowerMarkers.r, g_SelfFeatures->m_SuperPowerMarkers.g, g_SelfFeatures->m_SuperPowerMarkers.b, g_SelfFeatures->m_SuperPowerMarkers.a / 255.f);
					for (const auto Entity : Entities::GetEntities(player.second->m_ForcefieldVehicles, player.second->m_ForcefieldPeds, false))
					{
						const auto Coords = ENTITY::GET_ENTITY_COORDS(Entity, TRUE);
						if (Math::DistanceBetweenVectors(Coords, TheirCoords) < player.second->m_ForcefieldRange && Math::DistanceBetweenVectors(Coords, TheirCoords) >= 1.f)
						{
							NativeVector3 Velocity{};

							Velocity.x = TheirCoords.x + (1000 * cos(Pitch) * cos(Yaw)) - Coords.x;
							Velocity.y = TheirCoords.y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.y;
							Velocity.z = TheirCoords.z + (1000 * sin(Pitch)) - Coords.z;

							Entities::ChangeNetworkObjectOwner(Entity, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
							ENTITY::SET_ENTITY_VELOCITY(Entity, Velocity.x, Velocity.y, Velocity.z);
						}
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateWeaponOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_BulletRedirection)
				{
					const auto PlayerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					Hash WeaponHash{};

					if (Weapons::IsPedShooting(PlayerPed) && WEAPON::GET_CURRENT_PED_WEAPON(PlayerPed, &WeaponHash, TRUE))
						Target::ShootPlayer(player.second, WeaponHash);
				}

				if (player.second->m_PedGun)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					NativeVector3 Coords{};
					if (Weapons::IsPedShooting(Ped) && WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Ped, &Coords))
					{
						PED::CREATE_RANDOM_PED(Coords.x, Coords.y, Coords.z);
					}
				}

				if (player.second->m_AirstrikeGun)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					NativeVector3 Coords{};
					if (Weapons::IsPedShooting(Ped) && WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Ped, &Coords))
					{
						const auto Airstrike = Joaat("WEAPON_AIRSTRIKE_ROCKET");
						WEAPON::REQUEST_WEAPON_ASSET(Airstrike, 31, FALSE);
						while (!WEAPON::HAS_WEAPON_ASSET_LOADED(Airstrike))
							Script::Current()->Yield(5ms);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 50.f, Coords.x, Coords.y, Coords.z, 250, 1, Airstrike, PLAYER::PLAYER_PED_ID(), 1, 0, -1.0);
						WEAPON::REMOVE_WEAPON_ASSET(Airstrike);
					}
				}

				if (player.second->m_TeleportGun)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					NativeVector3 Coords{};
					if (Weapons::IsPedShooting(Ped) && WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Ped, &Coords))
					{
						ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z, FALSE, FALSE, FALSE, FALSE);
					}
				}

				if (player.second->m_MoneyGun)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					NativeVector3 Coords{};
					if (Weapons::IsPedShooting(Ped) && WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Ped, &Coords))
					{
						const auto ModelHash = 0x113FD533;
						while (!STREAMING::HAS_MODEL_LOADED(ModelHash))
						{
							STREAMING::REQUEST_MODEL(ModelHash);
							Script::Current()->Yield(5ms);
						}

						OBJECT::CREATE_AMBIENT_PICKUP(0x65948212, Coords.x, Coords.y, Coords.z, 0, 2500, ModelHash, FALSE, TRUE);
					}
				}

				if (player.second->m_FakeMoneyGun)
				{
					const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
					NativeVector3 Coords{};
					if (Weapons::IsPedShooting(Ped) && WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(Ped, &Coords))
					{
						const auto ModelHash = Joaat("prop_money_bag_01");
						while (!STREAMING::HAS_MODEL_LOADED(ModelHash))
						{
							STREAMING::REQUEST_MODEL(ModelHash);
							Script::Current()->Yield(5ms);
						}

						System::ModelSpawnBypass(true);
						OBJECT::CREATE_OBJECT(ModelHash, Coords.x, Coords.y, Coords.z, TRUE, TRUE, FALSE);
						System::ModelSpawnBypass(false);
					}
				}
			}
		});
	}

	void PlayersFeatures::IterateVehicleOptions()
	{
		if (!*Pointers::pIsSessionStarted)
			return;

		g_PlayerService->Iterate([](const PlayerStructEntry& player)
		{
			const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
			const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(Ped, FALSE);

			if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE) && g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
			{
				if (player.second->m_VehicleInvincibility)
				{
					static Timer godTimer(0ms);
					godTimer.SetDelay(std::chrono::seconds(1));
					if (godTimer.Update()) {
						Entities::RequestControl(Vehicle);
						ENTITY::SET_ENTITY_INVINCIBLE(Vehicle, TRUE);
					}
				}

				if (player.second->m_DisorientHandling)
				{
					static bool Switch = false;

					static Timer switchTimer(0ms);
					switchTimer.SetDelay(std::chrono::seconds(1));
					if (switchTimer.Update()) {
						Switch = !Switch;
						Entities::RequestControl(Vehicle);
						VEHICLE::SET_VEHICLE_STEER_BIAS(Vehicle, Switch ? 1.0f : -1.0f);
					}
				}

				if (player.second->m_NoCollision)
				{
					static Timer collisionTimer(0ms);
					collisionTimer.SetDelay(std::chrono::seconds(1));
					if (collisionTimer.Update()) {
						Entities::RequestControl(Vehicle);
						ENTITY::SET_ENTITY_COLLISION(Vehicle, FALSE, FALSE);
					}
				}

				if (player.second->m_SlippyWheels)
				{
					static Timer slipTimer(0ms);
					slipTimer.SetDelay(std::chrono::seconds(1));
					if (slipTimer.Update()) {
						Entities::RequestControl(Vehicle);
						VEHICLE::SET_VEHICLE_REDUCE_GRIP(Vehicle, TRUE);
					}
				}
			}
		});
	}
}