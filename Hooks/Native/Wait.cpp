#include "../../GTA/Invoker/Natives.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../GTA/Script/ScriptManager.hpp"
#include "../HookTable.hpp"

namespace Sentinel
{
	namespace // Hook Functions
	{
		std::uint32_t gHookFrameCount{};
	}

	void Native::Wait(rage::scrNativeCallContext* src)
	{
		const auto ms = src->get_arg<int>(0);

		if (gRunning && gHookFrameCount != *Pointers::pFrameCount)
		{
			gHookFrameCount = *Pointers::pFrameCount;
			ScriptManager::Tick();
		}

		SYSTEM::WAIT(ms);
	}
}