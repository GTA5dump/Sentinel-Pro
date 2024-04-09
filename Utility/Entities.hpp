#pragma once 
#include "../Common.hpp"
#include "Target.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../Pointers/Pointers.hpp"

namespace Sentinel::Entities
{
	inline void ClearPedTasks(Ped ped)
	{
		TASK::CLEAR_PED_TASKS(ped);
		TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
		TASK::CLEAR_PED_SECONDARY_TASK(ped);
	}

	inline bool ChangeNetworkObjectOwner(std::int32_t script_index, CNetGamePlayer* owner)
	{
		std::uint64_t NetworkObjectMgrInterface = *(std::uint64_t*)(Pointers::pNetworkObjectMgrInterface);
		if (NetworkObjectMgrInterface == NULL)
			return false;

		if (!ENTITY::DOES_ENTITY_EXIST(script_index))
			return false;

		std::uint64_t Entity = Pointers::pGetEntityFromScript(script_index);
		if (Entity == NULL)
			return false;

		std::uint64_t NetObject = *(std::uint64_t*)(Entity + 0xD0);
		if (NetObject == NULL)
			return false;

		if (*(std::uint16_t*)(NetObject + 0x8) == 11)
			return false;

		Pointers::pChangeNetworkObjectOwner(NetworkObjectMgrInterface, NetObject, owner, 0ui64);

		return true;
	}

	inline bool RequestControl(Entity entity)
	{
		if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity))
			return true;

		const auto NID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);

		for (std::uint8_t i = 0; !NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && !NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(NID) && i < 30; i++)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(NID);
			Script::Current()->Yield(1ms);
		}

		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity))
			return false;
		NETWORK::SET_NETWORK_ID_CAN_MIGRATE(NID, TRUE);
		return true;
	}

	inline std::vector<Entity> GetEntities(bool vehicles, bool peds, bool objects)
	{
		std::vector<Entity> Targets;
		Targets.clear();
		const auto ReplayInterface = *Pointers::pReplayInterface;
		if (!ReplayInterface)
			return Targets;

		if (vehicles)
		{
			const auto VehicleInterface = ReplayInterface->m_vehicle_interface;
			for (int i = 0; i < VehicleInterface->m_max_vehicles; i++)
			{
				const auto VehiclePointer = VehicleInterface->get_vehicle(i);
				if (!VehiclePointer)
					continue;

				if (VehiclePointer == GTA::GetLocalPed()->m_vehicle)
					continue;

				const auto Vehicle = Pointers::pPointerToHandle(VehiclePointer);
				if (!Vehicle)
					break;

				if (Target::IsPlayerVehicle(Vehicle))
					continue;

				Targets.push_back(Vehicle);
			}
		}

		if (peds)
		{
			const auto PedInteface = ReplayInterface->m_ped_interface;
			for (int i = 0; i < PedInteface->m_max_peds; i++)
			{
				const auto PedPointer = PedInteface->get_ped(i);
				if (!PedPointer)
					continue;

				if (PedPointer == GTA::GetLocalPed())
					continue;

				const auto Ped = Pointers::pPointerToHandle(PedPointer);
				if (!Ped)
					break;

				if (PED::IS_PED_A_PLAYER(Ped))
					continue;

				Targets.push_back(Ped);
			}
		}

		if (objects)
		{
			const auto ObjectInterface = ReplayInterface->m_object_interface;
			for (int i = 0; i < ObjectInterface->m_max_objects; i++)
			{
				const auto ObjectPointer = ObjectInterface->get_object(i);
				if (!ObjectPointer)
					continue;

				if (ObjectPointer == GTA::GetLocalPed())
					continue;

				const auto Object = Pointers::pPointerToHandle(ObjectPointer);
				if (!Object)
					break;

				Targets.push_back(Object);
			}
		}

		return Targets;
	}

	inline void DeleteEntity(Entity ent)
	{
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ent, TRUE, TRUE);
		ENTITY::DELETE_ENTITY(&ent);
	}
}