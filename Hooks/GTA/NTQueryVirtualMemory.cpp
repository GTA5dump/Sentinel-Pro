#include "../HookTable.hpp"

namespace Sentinel
{
	int GTA::NTQueryVirtualMemory(void* _this, HANDLE handle, PVOID base, int iclass, MEMORY_BASIC_INFORMATION* info, int size, std::size_t* length)
	{
		return 1;
	}
}