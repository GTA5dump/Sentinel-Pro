#include "ExceptionHandler.hpp"
#include "StackTrace.hpp"
#include <Zydis/Zydis.h>

namespace Sentinel
{
	inline auto HashStackTrace(std::vector<std::uint64_t> Trace)
	{
		const auto Data = reinterpret_cast<const char*>(Trace.data());
		const std::size_t Size = Trace.size() * sizeof(std::uint64_t);

		return std::hash<std::string_view>()({ Data, Size });
	}

	ExceptionHandler::ExceptionHandler()
	{
		m_OldErrorMode = SetErrorMode(0);
		m_ExceptionHandler = SetUnhandledExceptionFilter(&VectoredExceptionHandler);
	}

	ExceptionHandler::~ExceptionHandler()
	{
		SetErrorMode(m_OldErrorMode);
		SetUnhandledExceptionFilter(reinterpret_cast<decltype(&VectoredExceptionHandler)>(m_ExceptionHandler));
	}

	inline static StackTrace g_Trace;
	LONG VectoredExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo)
	{
		const auto ExceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
		if (ExceptionCode == EXCEPTION_BREAKPOINT || ExceptionCode == DBG_PRINTEXCEPTION_C || ExceptionCode == DBG_PRINTEXCEPTION_WIDE_C)
			return EXCEPTION_CONTINUE_SEARCH;

		static std::set<std::size_t> LoggedExceptions;

		g_Trace.NewStackTrace(ExceptionInfo);
		const auto TraceHash = HashStackTrace(g_Trace.FramePointers());
		if (const auto it = LoggedExceptions.find(TraceHash); it == LoggedExceptions.end())
		{
			LOG(FATAL) << g_Trace;
			//Logger::FlushQueue();

			LoggedExceptions.insert(TraceHash);
		}

		ZyanU64 OpcodeAddress = ExceptionInfo->ContextRecord->Rip;
		ZydisDisassembledInstruction Instruction;
		ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, OpcodeAddress, reinterpret_cast<void*>(OpcodeAddress), 32, &Instruction);

		ExceptionInfo->ContextRecord->Rip += Instruction.info.length;

		return EXCEPTION_CONTINUE_EXECUTION;
	}
}