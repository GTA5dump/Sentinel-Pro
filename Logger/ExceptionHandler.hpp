#pragma once
#include "../Common.hpp"

namespace Sentinel
{
	class ExceptionHandler final
	{
	public:
		ExceptionHandler();
		virtual ~ExceptionHandler();

	private:
		void* m_ExceptionHandler;
		std::uint32_t m_OldErrorMode;
	};

	extern LONG VectoredExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo);
}