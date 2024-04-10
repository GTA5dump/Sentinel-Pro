#pragma once
#include "Include.hpp"
#include "../Common.hpp"
#include "../Pointers/Pointers.hpp"
#include "../Utility/Joaat.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../Services/Players/PlayerService.hpp"
#include "ScriptHandler.hpp"

namespace Sentinel::GTA
{
	namespace Bits
	{
		template<typename T>
		bool HasBitsSet(T* address, T bits)
		{
			return (*address & bits) == bits;
		}

		template<typename T>
		inline constexpr void SetBit(T* value, T bit)
		{
			*value |= (1 << bit);
		}

		template<typename T, typename ...T2>
		inline constexpr void SetBits(T* value, T2... bit)
		{
			(SetBit<T>(value, std::forward<T>(bit)), ...);
		}

		template<typename T>
		inline constexpr void ClearBit(T* value, T bit)
		{
			*value &= ~(1 << bit);
		}

		template<typename T, typename ...T2>
		inline constexpr void ClearBits(T* value, T2... bit)
		{
			(ClearBits<T>(value, std::forward<T>(bit)), ...);
		}
	}

	namespace Misc 
	{
		template<typename T>
		inline bool IsValidPointer(T pointer)
		{
			std::uint64_t Address = (std::uint64_t)pointer;
			if (Address < 0x5000) 
				return false;
			if ((Address & 0xFFFFFFFF) == 0xFFFFFFFF) 
				return false;
			if ((Address & 0xFFFFFFFF) <= 0xFF)
				return false;
			if (Address > 0xFFFFFFFFFFFF) 
				return false;
			return true;
		}
	}

	namespace Offsets
	{
		inline std::uintptr_t GetMultilayerPointer(std::uintptr_t base_address, std::vector<DWORD> offsets)
		{
			std::uintptr_t Pointer = *(std::uintptr_t*)(base_address);
			if (!Pointer)
				return NULL;
			auto Level = offsets.size();

			for (auto i = 0; i < Level; i++)
			{
				if (i == Level - 1)
				{
					Pointer += offsets[i];
					if (!Pointer)
						return NULL;
					return Pointer;
				}
				else
				{
					Pointer = *(std::uint64_t*)(Pointer + offsets[i]);
					if (!Pointer)
						return NULL;
				}
			}

			return Pointer;
		}

		template <typename T>
		inline T GetValue(std::vector<DWORD> offsets)
		{
			std::uintptr_t Address = GetMultilayerPointer(Pointers::pWorldPointer, offsets);
			if (Address == NULL) 
				return NULL;

			return *((T*)Address);
		}

		template <typename T>
		inline void SetValue(std::vector<DWORD> offsets, T value)
		{
			std::uintptr_t Address = GetMultilayerPointer(Pointers::pWorldPointer, offsets);
			if (Address == NULL) 
				return;

			*reinterpret_cast<T*>(Address) = value;
		}
	}

	inline static GtaThread* FindScriptThread(JoaatType hash)
	{
		for (auto Thread : *Pointers::pScriptThreadArray)
		{
			if (Thread
				&& Thread->m_context.m_thread_id
				&& Thread->m_handler
				&& Thread->m_script_hash == hash)
			{
				return Thread;
			}
		}

		return nullptr;
	}

	inline CPed* GetLocalPed()
	{
		if (auto PedFactory = *Pointers::pPedFactory)
		{
			return PedFactory->m_local_ped;
		}

		return nullptr;
	}

	inline CPlayerInfo* GetLocalPlayerInfo()
	{
		if (auto PedFactory = *Pointers::pPedFactory)
		{
			if (auto Ped = PedFactory->m_local_ped)
			{
				return Ped->m_player_info;
			}
		}

		return nullptr;
	}

	inline CNetworkPlayerMgr* GetNetworkPlayerMgr()
	{
		return *Pointers::pNetworkPlayerMgr;
	}

	inline Network* GetNetwork()
	{
		return *Pointers::pNetwork;
	}

	inline std::uint64_t GetModelInfo(std::uint32_t hash)
	{
		std::int32_t Index = 0xFFFF;
		return Pointers::pGetModelInfo(hash, &Index);
	}

	inline bool ForceHostOfScript(PlayerStructPointer target, rage::joaat_t hash)
	{
		if (auto launcher = FindScriptThread(hash); launcher && launcher->m_net_component)
		{
			for (int i = 0; !((CGameScriptHandlerNetComponent*)launcher->m_net_component)->is_local_player_host(); i++)
			{
				if (i > 200)
					return false;

				((CGameScriptHandlerNetComponent*)launcher->m_net_component)->send_host_migration_event(target->GetNetPlayer());
				Script::Current()->Yield(10ms);

				if (!launcher->m_stack || !launcher->m_net_component)
					return false;
			}
		}

		return true;
	}
}