#pragma once
#include "../GTA/Script/ScriptHook.hpp"

namespace Sentinel
{
	class NativeHooks final
	{
		using NativeDetour = std::pair<rage::scrNativeHash, rage::scrNativeHandler>;

		std::unordered_map<rage::joaat_t, std::vector<NativeDetour>> m_NativeRegistrations;
		std::unordered_map<rage::scrProgram*, std::unique_ptr<ScriptHook>> m_ScriptHooks;

	public:
		NativeHooks();
		~NativeHooks();

		NativeHooks(const NativeHooks&) = delete;
		NativeHooks(NativeHooks&&) noexcept = delete;
		NativeHooks& operator=(const NativeHooks&) = delete;
		NativeHooks& operator=(NativeHooks&&) noexcept = delete;

		/**
		 * @brief Add a detour for all script threads
		 *
		 * @param hash Hash of the native to detour
		 * @param detour Detour Function
		 */
		void AddNativeDetour(const std::string_view name, rage::scrNativeHash hash, rage::scrNativeHandler detour);
		/**
		 * @brief Add a detour for a specifik script
		 *
		 * @param script_hash Hash of the script to detour
		 * @param hash Hash of the native to detour
		 * @param detour Detour Function
		 */
		void AddNativeDetour(const std::string_view name, rage::joaat_t script_hash, rage::scrNativeHash hash, rage::scrNativeHandler detour);

		void HookProgram(rage::scrProgram* program);

		void UnhookProgram(rage::scrProgram* program);
	};

	inline NativeHooks* g_NativeHooks{};
}