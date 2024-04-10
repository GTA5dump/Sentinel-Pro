#include "../HookTable.hpp"
#include "../../Hooking/NativeHooks.hpp"
#include "../../Services/ScriptPatcher/ScriptPatcher.hpp"

namespace Sentinel
{
	bool GTA::InitializeNativeTables(rage::scrProgram* program)
	{
		const auto Return = Hooking::GetHook<DetourHook>("INT")->Original<decltype(&InitializeNativeTables)>()(program);
		g_ScriptPatcher->OnScriptLoad(program);
		g_NativeHooks->HookProgram(program);

		return Return;
	}
}