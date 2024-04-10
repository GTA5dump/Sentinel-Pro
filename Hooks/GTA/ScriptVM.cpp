#include "../HookTable.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../Services/ScriptPatcher/ScriptPatcher.hpp"

#include <GTAV-Classes/script/scrProgram.hpp>

namespace Sentinel
{
	rage::eThreadState GTA::ScriptVM(std::uint64_t* start_stack, std::uint64_t** scr_globals, rage::scrProgram* program, rage::scrThreadContext* ctx)
	{
		std::uint8_t** OriginalByteCode = program->m_code_blocks;

		if (auto ByteCode = g_ScriptPatcher->GetScriptBytecode(program->m_name_hash))
			program->m_code_blocks = ByteCode;

		const auto Return = Hooking::GetHook<DetourHook>("SVM")->Original<decltype(&ScriptVM)>()(start_stack, scr_globals, program, ctx);

		program->m_code_blocks = OriginalByteCode;

		return Return;
	}
}