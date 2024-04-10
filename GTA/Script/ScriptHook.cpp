#include "../../Common.hpp"
#include "ScriptHook.hpp"
#include "../Invoker/CrossMap.hpp"
#include "../../Pointers/Pointers.hpp"

namespace Sentinel
{
	inline std::unordered_map<rage::scrProgram*, ScriptHook*> ScriptHook::s_Map;

	static bool MapNative(rage::scrNativeHash* hash)
	{
		for (auto const& mapping : gCrossMap)
		{
			if (mapping.first == *hash)
			{
				*hash = mapping.second;
				return true;
			}
		}

		return false;
	}

	ScriptHook::ScriptHook(rage::joaat_t script_hash, std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> replacements) :
		m_ScriptHash(script_hash),
		m_NativeReplacementsPointer(std::move(replacements))
	{
		Ensure();
	}

	ScriptHook::ScriptHook(rage::scrProgram* program, std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> replacements) :
		m_NativeReplacementsPointer(std::move(replacements))
	{
		HookInstance(program);
	}

	ScriptHook::~ScriptHook()
	{
		if (m_Program)
		{
			for (auto [hash, handler_ptr] : m_NativeHandlerPointer)
			{
				auto og_handler = Pointers::pGetNativeHandler(Pointers::pNativeRegistrationTable, hash);
				*handler_ptr = og_handler;
			}
		}

		if (m_VMTHook)
		{
			m_VMTHook->Disable();

			if (m_Program)
				s_Map.erase(m_Program);
		}
	}

	void ScriptHook::Ensure()
	{
		if (m_VMTHook)
			return;

		if (auto program = Pointers::pScriptProgramTable->find_script(m_ScriptHash))
		{
			if (program->is_valid())
			{
				HookInstance(program);
				LOG(INFO) << "Hooked: " << program->m_name;
			}
		}
	}

	void ScriptHook::HookInstance(rage::scrProgram* program)
	{
		m_Program = program;
		s_Map.emplace(m_Program, this);
		m_VMTHook = std::make_unique<VMTHook>("ScriptHook", (void***)m_Program);
		m_VMTHook->Hook(0, &scrProgramDetour);

		for (auto [replacement_hash, replacement_handler] : m_NativeReplacementsPointer)
		{
			auto hash = replacement_hash;
			MapNative(&hash);

			auto og_handler = Pointers::pGetNativeHandler(Pointers::pNativeRegistrationTable, hash);
			if (!og_handler)
				continue;

			auto handler_ptr = m_Program->get_address_of_native_entrypoint(og_handler);
			if (!handler_ptr)
				continue;

			m_NativeHandlerPointer.emplace(hash, reinterpret_cast<rage::scrNativeHandler*>(handler_ptr));
			*handler_ptr = replacement_handler;
		}
	}

	void ScriptHook::scrProgramDetour(rage::scrProgram* this_, bool free_memory)
	{
		if (auto it = s_Map.find(this_); it != s_Map.end())
		{
			auto hook = it->second;

			hook->m_Program = nullptr;
			s_Map.erase(it);

			auto og_func = hook->m_VMTHook->Original<decltype(&scrProgramDetour)>(0);
			hook->m_VMTHook->Disable();
			hook->m_VMTHook.reset();

			og_func(this_, free_memory);
		}
	}
}