#pragma once 
#include "../../Common.hpp"
#include "ScriptManager.hpp"

namespace Sentinel
{
	class ScriptQueue
	{
	public:
		explicit ScriptQueue(std::size_t count);
		~ScriptQueue();

		void Queue(std::function<void()> function);
	private:
		void FiberTick();
		static void FiberFunction();
	private:
		std::recursive_mutex m_Mutex;
		std::stack<std::function<void()>> m_Jobs;
	};

	inline ScriptQueue* g_ScriptQueue{};
}