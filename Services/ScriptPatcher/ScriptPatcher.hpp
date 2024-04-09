#pragma once 
#include "ScriptData.hpp"
#include "ScriptPatch.hpp"

namespace Sentinel
{
	class ScriptPatcher
	{
		std::vector<ScriptPatch> m_ScriptPatches;
		std::unordered_map<rage::joaat_t, std::unique_ptr<ScriptData>> m_ScriptData;
		ScriptData* GetDataForScript(rage::joaat_t script);
		bool DoesScriptHavePatches(rage::joaat_t script);
		void CreateDataForScript(rage::scrProgram* program);
		void UpdateAllPatchesForScript(rage::joaat_t script);

	public:
		ScriptPatcher();
		~ScriptPatcher();

		void AddPatch(ScriptPatch&& patch);
		void OnScriptLoad(rage::scrProgram* program);
		std::uint8_t** GetScriptBytecode(rage::joaat_t script);
		void Update();
	};

	inline ScriptPatcher* g_ScriptPatcher;
}