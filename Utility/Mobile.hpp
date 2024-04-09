#pragma once 
#include "../Common.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/enums.hpp"

namespace Sentinel::Mobile
{
	inline bool FixIndexMMS(int veh_idx, bool spawn_veh = false)
	{
		const bool CanBeFixed = GTA::Bits::HasBitsSet(
			ScriptGlobal(1586468).At(veh_idx, 142).Add(103).As<int*>(),
			eVehicleFlags::DESTROYED | eVehicleFlags::HAS_INSURANCE
		);

		if (CanBeFixed)
		{
			GTA::Bits::ClearBits(
				ScriptGlobal(1586468).At(veh_idx, 142).Add(103).As<int*>(),
				eVehicleFlags::DESTROYED | eVehicleFlags::IMPOUNDED | eVehicleFlags::UNK2
			);

			if (spawn_veh)
			{
				GTA::Bits::SetBits(
					ScriptGlobal(1586468).At(veh_idx, 142).Add(103).As<int*>(),
					eVehicleFlags::TRIGGER_SPAWN_TOGGLE | eVehicleFlags::SPAWN_AT_MORS_MUTUAL
				);
			}
		}
		return CanBeFixed;
	}

	inline int FixAllMMS()
	{
		int FixedCount = 0;

		for (int i = 0; i < *ScriptGlobal(1586468).As<int*>(); i++)
			if (FixIndexMMS(i, true))
				FixedCount++;

		return FixedCount;
	}
}