#include "ScriptQueue.hpp"
#include "ScriptManager.hpp"

namespace Sentinel
{
	ScriptQueue::ScriptQueue(std::size_t num_fibers)
	{
		for (std::size_t i = 0; i < num_fibers; ++i)
		{
			ScriptManager::Initialize(std::make_unique<Script>(&FiberFunction));
		}

		g_ScriptQueue = this;
	}

	ScriptQueue::~ScriptQueue()
	{
		g_ScriptQueue = nullptr;
	}

	void ScriptQueue::Queue(std::function<void()> function)
	{
		if (function)
		{
			std::lock_guard lock(m_Mutex);
			m_Jobs.push(std::move(function));
		}
	}

	void ScriptQueue::FiberTick()
	{
		std::unique_lock lock(m_Mutex);
		if (!m_Jobs.empty())
		{
			const auto job = std::move(m_Jobs.top());
			m_Jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void ScriptQueue::FiberFunction()
	{
		while (true)
		{
			g_ScriptQueue->FiberTick();
			Script::Current()->Yield();
		}
	}
}