#include "NativeHooks.hpp"
#include "../Pointers/Pointers.hpp"
#include "../Utility/Joaat.hpp"
#include "../Hooks/HookTable.hpp"

namespace Sentinel
{
	auto ALL_SCRIPT_HASH = Joaat("ALL_SCRIPTS");

	NativeHooks::NativeHooks()
	{
		AddNativeDetour("WAIT", Joaat("main_persistent"), 0x4EDE34FBADD967A6, Native::Wait);
		AddNativeDetour("SCPW", 0xADF692B254977C0C, Native::SetCurrentPedWeapon);
		AddNativeDetour("DCA", 0xFE99B66D079CF6BC, Native::DisableControlAction);
		AddNativeDetour("HFWW", 0xEB354E5376BC81A7, Native::HudForceWeaponWheel);
		AddNativeDetour("SWMWH", Joaat("shop_controller"), 0xDC38CC1E35B6A5D7, Native::SetWarningMessageWithHeader);

		for (auto& entry : *Pointers::pScriptProgramTable)
			if (entry.m_program)
				HookProgram(entry.m_program);

		g_NativeHooks = this;
	}

	NativeHooks::~NativeHooks()
	{
		m_ScriptHooks.clear();
		g_NativeHooks = nullptr;
	}

	void NativeHooks::AddNativeDetour(const std::string_view name, rage::scrNativeHash hash, rage::scrNativeHandler detour)
	{
		AddNativeDetour(name, ALL_SCRIPT_HASH, hash, detour);
	}

	void NativeHooks::AddNativeDetour(const std::string_view name, rage::joaat_t script_hash, rage::scrNativeHash hash, rage::scrNativeHandler detour)
	{
		if (const auto& it = m_NativeRegistrations.find(script_hash); it != m_NativeRegistrations.end())
		{
			it->second.emplace_back(hash, detour);
			return;
		}

		m_NativeRegistrations.emplace(script_hash, std::vector<NativeDetour>({ { hash, detour } }));

		LOG(INFO) << "[Natives] Hooked: " << name;
	}

	void NativeHooks::HookProgram(rage::scrProgram* program)
	{
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> native_replacements;
		const auto script_hash = program->m_name_hash;

		// Functions that need to be detoured for all scripts
		if (const auto& pair = m_NativeRegistrations.find(ALL_SCRIPT_HASH); pair != m_NativeRegistrations.end())
			for (const auto& native_hook_reg : pair->second)
				native_replacements.insert(native_hook_reg);

		// Functions that only need to be detoured for a specific script
		if (const auto& pair = m_NativeRegistrations.find(script_hash); pair != m_NativeRegistrations.end())
			for (const auto& native_hook_reg : pair->second)
				native_replacements.insert(native_hook_reg);

		if (!native_replacements.empty())
		{
			m_ScriptHooks.emplace(
				program,
				std::make_unique<ScriptHook>(program, native_replacements)
			);
		}
	}

	void NativeHooks::UnhookProgram(rage::scrProgram* program)
	{
		m_ScriptHooks.erase(program);
	}
}