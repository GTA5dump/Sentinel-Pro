#include "../HookTable.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../GUI/Overlays/Overlays.hpp"

namespace Sentinel
{
	void* GTA::InfiniteTrainCrashPatch(std::uint64_t* carriage)
	{
		void* CurrentCarriage = carriage;
		int Count = 0;

		while (Pointers::pGetNextCarriage(CurrentCarriage))
		{
			if (++Count > 20) {
				return nullptr;
			}

			CurrentCarriage = Pointers::pGetNextCarriage(carriage);
		}

		return CurrentCarriage;
	}
}