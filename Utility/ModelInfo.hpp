#pragma once
#include <GTAV-Classes/rage/joaat.hpp>
#include "../Pointers/Pointers.hpp"
#include <GTAV-Classes/vehicle/CVehicleModelInfo.hpp>

namespace Sentinel
{
#pragma pack(push, 1)
	class ModelInfo
	{
	public:
		static bool DoesModelExist(const rage::joaat_t hash)
		{
			if (const auto Model = ModelInfo::GetModel(hash); Model)
				return true;
			return false;
		}

		template<typename T = CBaseModelInfo*>
		static T GetModel(const rage::joaat_t hash)
		{
			const auto ModelTable = Pointers::pModelTable;
			for (auto i = ModelTable->m_lookup_table[hash % ModelTable->m_lookup_key]; i; i = i->m_next)
			{
				if (i->m_hash == hash)
				{
					if (const auto Model = ModelTable->m_data[i->m_idx]; Model)
					{
						return reinterpret_cast<T>(Model);
					}
				}
			}
			return nullptr;
		}

		static CVehicleModelInfo* GetVehicleModel(const rage::joaat_t hash)
		{
			if (const auto Model = ModelInfo::GetModel<CVehicleModelInfo*>(hash); Model && Model->m_model_type == eModelType::Vehicle)
				return Model;
			return nullptr;
		}

		template<typename T, typename... Args>
		static bool IsModelOfType(const rage::joaat_t hash, const T arg, const Args... args)
		{
			bool Return = false;
			if (const auto Model = ModelInfo::GetModel(hash))
			{
				Return = Model->m_model_type == arg;
				(
					[&Return, &Model](eModelType type) {
						Return |= Model->m_model_type == type;
					}(args),
						...);
			}
			return Return;
		}
	};
#pragma pack(pop)
}