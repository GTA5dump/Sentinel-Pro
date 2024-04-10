#include "../../GTA/Invoker/Natives.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../HookTable.hpp"

namespace Sentinel
{
	void Native::SetWarningMessageWithHeader(rage::scrNativeCallContext* src)
	{
		if (auto entry_line = src->get_arg<const char*>(1); !strcmp(entry_line, "CTALERT_F_2") && g_ProtectionScript->m_DisableTransactionError)
		{
			*ScriptGlobal(4536673).As<int*>() = 0;

			return;
		}

		HUD::SET_WARNING_MESSAGE_WITH_HEADER(src->get_arg<const char*>(0), src->get_arg<const char*>(1), src->get_arg<int>(2), src->get_arg<const char*>(3), src->get_arg<BOOL>(4), src->get_arg<Any>(5), src->get_arg<Any*>(6), src->get_arg<Any*>(7), src->get_arg<BOOL>(8), src->get_arg<Any>(9));
	}
}