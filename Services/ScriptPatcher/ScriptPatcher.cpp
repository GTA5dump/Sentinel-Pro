#include "ScriptPatcher.hpp"

namespace Sentinel
{
	ScriptPatcher::ScriptPatcher()
	{
		g_ScriptPatcher = this;
	}

	ScriptPatcher::~ScriptPatcher()
	{
		m_ScriptData.clear();
		m_ScriptPatches.clear();
		g_ScriptPatcher = nullptr;
	}

	ScriptData* ScriptPatcher::GetDataForScript(rage::joaat_t script)
	{
		for (auto& Patch : m_ScriptData)
		{
			if (Patch.first == script)
			{
				return Patch.second.get();
			}
		}

		return nullptr;
	}

	bool ScriptPatcher::DoesScriptHavePatches(rage::joaat_t script)
	{
		for (auto& Patch : m_ScriptPatches)
		{
			if (Patch.GetScript() == script)
				return true;
		}

		return false;
	}

	void ScriptPatcher::CreateDataForScript(rage::scrProgram* program)
	{
		auto Pages = new std::uint8_t * [program->get_num_code_pages()];

		for (auto i = 0u; i < program->get_num_code_pages(); i++)
		{
			Pages[i] = new std::uint8_t[program->get_code_page_size(i)];
			std::memcpy(Pages[i], program->get_code_page(i), program->get_code_page_size(i));
		}

		m_ScriptData.emplace(program->m_name_hash, std::make_unique<ScriptData>(program->m_code_size, Pages, program->get_num_code_pages()));
	}

	void ScriptPatcher::UpdateAllPatchesForScript(rage::joaat_t script)
	{
		auto Data = GetDataForScript(script);

		for (auto& Patch : m_ScriptPatches)
			if (Patch.GetScript() == script)
				Patch.Update(Data);
	}

	void ScriptPatcher::AddPatch(ScriptPatch&& patch)
	{
		m_ScriptPatches.push_back(std::move(patch));
	}

	void ScriptPatcher::OnScriptLoad(rage::scrProgram* program)
	{
		if (GetDataForScript(program->m_name_hash) == nullptr && DoesScriptHavePatches(program->m_name_hash))
		{
			CreateDataForScript(program);
			UpdateAllPatchesForScript(program->m_name_hash);
		}
	}

	std::uint8_t** ScriptPatcher::GetScriptBytecode(rage::joaat_t script)
	{
		if (auto Data = GetDataForScript(script))
			return Data->m_ByteCode;

		return nullptr;
	}

	void ScriptPatcher::Update()
	{
		for (auto& Patch : m_ScriptPatches)
		{
			auto Data = GetDataForScript(Patch.GetScript());
			if (Data)
			{
				Patch.Update(Data);
			}
		}
	}
}