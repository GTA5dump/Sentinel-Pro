#pragma once 
#include "../GTA/Invoker/Natives.hpp"
#include "../Pointers/Pointers.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../Services/Players/PlayerService.hpp"
#include "../Utility/All.hpp"

// I'm sorry yuh
namespace Sentinel
{
	struct AngryPlane
	{
		int m_ID;
		int m_Ped;
	};

	class AngryPlanes
	{
	public:
		void Tick(PlayerStructPointer target)
		{
			Push(target, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID()), ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target->ID()), TRUE), -1275859404);
			SpawnPlanes();
		}
		void Destroy()
		{
			for (auto& i : m_SpawnedPlanes)
			{
				if (ENTITY::DOES_ENTITY_EXIST(i.m_Ped))
				{
					Entities::RequestControl(i.m_Ped);
					Entities::ClearPedTasks(i.m_Ped);
					Entities::DeleteEntity(i.m_Ped);
				}
			}
			for (auto& i : m_SpawnedPlanes)
			{
				if (ENTITY::DOES_ENTITY_EXIST(i.m_ID))
				{
					Entities::RequestControl(i.m_ID);
					Entities::DeleteEntity(i.m_ID);
				}
			}
			m_SpawnedPlanes.resize(0);
		}
	public:
		float m_Range = 400.f;
		NativeVector3 m_Coords;
	private:
		void Push(PlayerStructPointer target, Ped ped, NativeVector3 coords, std::uint32_t enemy_hash, int planes = 15)
		{
			this->m_Player = target;
			this->m_PlayerPed = ped;
			this->m_Coords = coords;
			this->m_EnemyHash = enemy_hash;
			this->m_Planes = planes;

			if (this->m_PlayerPed)
				this->m_Initialized = true;
		}

		Ped CreateEnemy(std::uint32_t hash, NativeVector3 coords)
		{
			if (this->m_Initialized)
			{
				if (STREAMING::IS_MODEL_VALID(hash) && STREAMING::IS_MODEL_IN_CDIMAGE(hash))
				{
					while (!STREAMING::HAS_MODEL_LOADED(hash))
					{
						STREAMING::REQUEST_MODEL(hash);
						Script::Current()->Yield(5ms);
					}

					System::ModelSpawnBypass(true);
					const auto Ped = PED::CREATE_PED(26, this->m_EnemyHash, coords.x + 10.f, coords.y + 10.f, coords.z + 10.f, 0, TRUE, TRUE);
					System::ModelSpawnBypass(false);

					if (m_Player->m_SpawnInvincible)
						ENTITY::SET_ENTITY_INVINCIBLE(Ped, TRUE);

					if (m_Player->m_SpawnInvisible)
					{
						ENTITY::SET_ENTITY_VISIBLE(Ped, FALSE, FALSE);
						NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Ped, TRUE);
					}

					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
					PED::SET_PED_AS_ENEMY(Ped, TRUE);
					PED::SET_PED_COMBAT_ATTRIBUTES(Ped, 1, TRUE);
					PED::SET_PED_COMBAT_ATTRIBUTES(Ped, 46, TRUE);
					PED::SET_PED_COMBAT_ATTRIBUTES(Ped, 16, TRUE);
					PED::SET_PED_COMBAT_ABILITY(Ped, 2);
					PED::SET_PED_FLEE_ATTRIBUTES(Ped, 0, FALSE);

					return Ped;
				}
			}

			return 0;
		}

		Vehicle SpawnPlane(std::uint32_t hash)
		{
			if (this->m_Initialized)
			{
				if (STREAMING::IS_MODEL_VALID(hash) && STREAMING::IS_MODEL_IN_CDIMAGE(hash) && VEHICLE::IS_THIS_MODEL_A_PLANE(hash))
				{
					while (!STREAMING::HAS_MODEL_LOADED(hash))
					{
						STREAMING::REQUEST_MODEL(hash);
						Script::Current()->Yield(5ms);
					}

					NativeVector3 Coords;
					Coords.x = this->m_Coords.x + MISC::GET_RANDOM_FLOAT_IN_RANGE(-this->m_Range, this->m_Range);
					Coords.y = this->m_Coords.y + MISC::GET_RANDOM_FLOAT_IN_RANGE(-this->m_Range, this->m_Range);
					Coords.z = this->m_Coords.z + 150.f;

					if (const auto Enemy = this->CreateEnemy(this->m_EnemyHash, m_Coords))
					{
						const auto Heading = ENTITY::GET_ENTITY_HEADING(this->m_PlayerPed);
						System::ModelSpawnBypass(true);
						const auto Plane = VEHICLE::CREATE_VEHICLE(hash, Coords.x, Coords.y, Coords.z, Heading, TRUE, TRUE, 0);
						System::ModelSpawnBypass(true);

						if (m_Player->m_SpawnInvincible)
							ENTITY::SET_ENTITY_INVINCIBLE(Plane, TRUE);

						if (m_Player->m_SpawnInvisible)
						{
							ENTITY::SET_ENTITY_VISIBLE(Plane, FALSE, FALSE);
							NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(Plane, TRUE);
						}

						Script::Current()->Yield(5ms);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
						if (*Pointers::pIsSessionStarted)
						{
							DECORATOR::DECOR_SET_INT(Plane, "MPBitset", 0);
							ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(Plane, TRUE);
							auto NID = NETWORK::VEH_TO_NET(Plane);
							if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(Plane))
								NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
							VEHICLE::SET_VEHICLE_IS_STOLEN(Plane, FALSE);
						}

						if (Plane)
						{
							VEHICLE::SET_VEHICLE_ENGINE_ON(Plane, TRUE, TRUE, FALSE);
							VEHICLE::SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(Plane, TRUE);
							PED::SET_PED_INTO_VEHICLE(Enemy, Plane, -1);
							ENTITY::APPLY_FORCE_TO_ENTITY(Plane, 1, 0, 10.5f, 0., 0, 0, 0, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
							ENTITY::SET_ENTITY_HEADING(Plane, 45.f);
							TASK::TASK_PLANE_MISSION(Enemy, Plane,
								PED::IS_PED_IN_ANY_VEHICLE(this->m_PlayerPed, TRUE) ? PED::GET_VEHICLE_PED_IS_USING(this->m_PlayerPed) : 0, this->m_PlayerPed,
								this->m_Coords.x, this->m_Coords.y, this->m_Coords.z,
								4, 20.f, -1, -1, 0, 100, FALSE);

							if (m_Player->m_SpawnWithRockets)
							{
								TASK::TASK_SHOOT_AT_ENTITY(Enemy, this->m_PlayerPed, 2000, Joaat("FIRING_PATTERN_FULL_AUTO"));
								TASK::TASK_COMBAT_PED(Enemy, this->m_PlayerPed, 0, 16);
							}

							AngryPlane Struct;
							Struct.m_Ped = Enemy;
							Struct.m_ID = Plane;
							this->m_SpawnedPlanes.push_back(Struct);
						}

						return Plane;
					}
				}
			}

			return 0;
		}

		void CheckForAlivePlanes()
		{
			int Alive = 0;

			for (auto i : this->m_SpawnedPlanes)
			{
				if (!ENTITY::DOES_ENTITY_EXIST(i.m_Ped) ||
					!ENTITY::DOES_ENTITY_EXIST(i.m_ID) ||
					ENTITY::GET_ENTITY_HEALTH(i.m_Ped) == 0 ||
					ENTITY::GET_ENTITY_HEALTH(i.m_ID) == 0)
				{
					this->m_SpawnedPlanes.erase(this->m_SpawnedPlanes.begin() + Alive);
				}

				Alive++;
			}
		}

		void SpawnPlanes()
		{
			this->CheckForAlivePlanes();

			if (m_SpawnedPlanes.size() != this->m_Planes)
			{
				const auto rand = Math::RandomNumberInRange(0, (int)this->m_PlaneHashes.size());
				this->SpawnPlane(this->m_PlaneHashes[rand]);
			}
		}
	private:
		std::vector<std::uint32_t> m_PlaneHashes
		{
			0x2F03547B,
			0x31F0B376,
			0x46699F47,
			0xAD6065C0,
			0x64DE07A1,
			0x39D6E83F,
			0xB39B0AE6,
			0x64DE07A1,
			0xC5DD6967
		};
	private:
		std::vector<AngryPlane> m_SpawnedPlanes{};
		PlayerStructPointer m_Player{};
		Ped m_PlayerPed{};
		bool m_Initialized{};
		std::uint32_t m_EnemyHash{};
		int m_Planes = 15;
	};

	inline AngryPlanes g_AngryPlanes;
}