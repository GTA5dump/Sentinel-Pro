#include "../Common.hpp"
#include "Batch.hpp"
#include "Range.hpp"

#include <future> // std::async

static std::mutex s_EntryMutex;
static std::vector<std::future<bool>> g_Futures;

namespace Memory
{
    void Batch::Add(std::string name, Pattern pattern, std::function<void(Handle)> callback)
    {
        m_Entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
    }

	bool ScanPattern(Range region, Memory::Batch::Entry entry)
	{
		if (auto Result = region.Scan(entry.m_Pattern))
		{
			if (entry.m_Callback)
			{
				std::lock_guard<std::mutex> lock(s_EntryMutex); // Acquire a lock on the mutex to synchronize access

				std::invoke(std::move(entry.m_Callback), Result);
				LOG(INFO) << "[Pointers] Found: " << entry.m_Name;

				return true;
			}
		}

		LOG(WARNING) << "[Pointers] Failed to find: " << entry.m_Name;

		return false;
	}

	bool Batch::Run(Range region)
    {
		bool Done = true;

		for (auto& Entry : m_Entries)
		{
			g_Futures.emplace_back(
				std::async(&ScanPattern, region, Entry));
		}

		m_Entries.clear();
		g_Futures.clear();

		return Done;
    }
}