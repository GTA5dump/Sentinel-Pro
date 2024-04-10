#pragma once 
#include "../../Pointers/Pointers.hpp"
#include "../../Services/ScriptPatcher/ScriptPatcher.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../Features/Misc.hpp"
#include <GTAV-Classes/rage/joaat.hpp>

namespace Sentinel::ScriptPatches
{
	inline void RegisterScriptPatches()
	{
		g_ScriptPatcher->AddPatch({ rage::joaat("freemode"), "2D 00 03 00 00 5D ? ? ? 71 08", 5, { 0x2E, 0x00, 0x00 }, &g_MiscFeatures->m_DisableAntiAfkKick }); // Anti-Afk
		g_ScriptPatcher->AddPatch({ rage::joaat("freemode"),
			"2D 00 03 00 00 5D ? ? ? 56 ? ? 72 2E ? ? 62",
			5,
			{ 0x72, 0x2E, 0x00, 0x01 },
			&g_MiscFeatures->m_DisableAntiAfkKick }); // Anti-Afk 2
		g_ScriptPatcher->AddPatch({ rage::joaat("freemode"), "2D 00 07 00 00 7B", 5, { 0x2E, 0x00, 0x00 }, &g_MiscFeatures->m_DisablePopulationLoadBalancing }); // Disable population load balancing
		g_ScriptPatcher->AddPatch({ rage::joaat("freemode"), "2D 01 09 00 00 5D ? ? ? 56 ? ? 2E", 5, {0x2E, 0x01, 0x00}, &g_MiscFeatures->m_DisableUnderneathMapDeath }); // Disable death when undermap/spectating
		g_ScriptPatcher->AddPatch({ rage::joaat("freemode"), "2D 01 04 00 ? 2C ? ? ? 5D ? ? ? 71 57 ? ? 2C", 5, { 0x2E, 0x01, 0x00 }, &g_ProtectionScript->m_ScriptHostKick }); // Script host kick
		g_ScriptPatcher->AddPatch({ rage::joaat("freemode"), "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, { 0x2E, 0x00, 0x00 }, &g_ProtectionScript->m_EndSessionKick }); // End session kick protection
		//g_ScriptPatcher->AddPatch({ rage::joaat("freemode"), "2D 01 05 00 00 38 00 2C ? ? ? 39 03 38 03 2C ? ? ? 56 ? ?", 5, { 0x2E, 0x01, 0x00 }, &g_BlockCEOCreation });

		for (auto& Entry : *Pointers::pScriptProgramTable)
		{
			if (Entry.m_program)
				g_ScriptPatcher->OnScriptLoad(Entry.m_program);
		}
	}
}