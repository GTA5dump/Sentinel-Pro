#include "ScriptFunction.hpp"
#include "../GTA.hpp"
#include "../../Pointers/Pointers.hpp"
#include <GTAV-Classes/script/scrProgram.hpp>
#include <GTAV-Classes/script/scrProgramTable.hpp>
#include <GTAV-Classes/script/tlsContext.hpp>

namespace Sentinel
{
	ScriptFunction::ScriptFunction(const std::string& name, const rage::joaat_t script, const std::string& pattern, int32_t offset) :
		m_Name(name),
		m_Script(script),
		m_Pattern(pattern),
		m_Offset(offset),
		m_IP(0)
	{
	}

	void ScriptFunction::PopulateIP()
	{
		if (m_IP == 0)
		{
			const auto Program = FindScriptProgram(m_Script);

			if (!Program)
				return;

			const auto Location = GetCodeLocationByPattern(Program, m_Pattern);

			if (!Location)
				LOG(FATAL) << std::format("[Scripts] Failed to find pattern: {} in script: {}", m_Name, Program->m_name);

			m_IP = Location.value() + m_Offset;
		}
	}

	void ScriptFunction::Call(rage::scrThread* thread, rage::scrProgram* program, std::initializer_list<std::uint64_t> args)
	{
		const auto tlsContext = rage::tlsContext::get();
		const auto Stack = (uint64_t*)thread->m_stack;
		const auto OriginalThread = tlsContext->m_script_thread;

		tlsContext->m_script_thread = thread;
		tlsContext->m_is_script_thread_active = true;

		rage::scrThreadContext threadContext = thread->m_context;

		for (auto& arg : args)
			Stack[threadContext.m_stack_pointer++] = arg;

		Stack[threadContext.m_stack_pointer++] = 0;
		threadContext.m_instruction_pointer = m_IP;
		threadContext.m_state = rage::eThreadState::idle;

		Pointers::pScriptVM(Stack, (std::uint64_t**)Pointers::pScriptGlobals, program, &threadContext);

		tlsContext->m_script_thread = OriginalThread;
		tlsContext->m_is_script_thread_active = OriginalThread != nullptr;
	}

	void ScriptFunction::operator()(std::initializer_list<std::uint64_t> args)
	{
		PopulateIP();

		if (m_IP == 0)
			return;

		auto Thread = GTA::FindScriptThread(m_Script);
		auto Program = FindScriptProgram(m_Script);

		if (Thread && Program)
		{
			Call(Thread, Program, args);
		}
	}

	rage::scrProgram* ScriptFunction::FindScriptProgram(rage::joaat_t hash)
	{
		for (auto& Script : *Pointers::pScriptProgramTable)
		{
			if (Script.m_program && Script.m_program->m_name_hash == hash)
				return Script.m_program;
		}

		return nullptr;
	}

	const std::optional<std::uint32_t> ScriptFunction::GetCodeLocationByPattern(rage::scrProgram* program, const Memory::Pattern& pattern)
	{
		std::uint32_t CodeSize = program->m_code_size;
		for (std::uint32_t i = 0; i < (CodeSize - pattern.m_Bytes.size()); i++)
		{
			for (std::uint32_t j = 0; j < pattern.m_Bytes.size(); j++)
				if (pattern.m_Bytes[j].has_value())
					if (pattern.m_Bytes[j].value() != *program->get_code_address(i + j))
						goto incorrect;

			return i;
		incorrect:
			continue;
		}

		return std::nullopt;
	}
}