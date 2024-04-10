#include "StackTrace.hpp"
#include "../Memory/Module.hpp"
#include <DbgHelp.h>
#include <winternl.h>

// https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64
// https://github.com/KjellKod/g3log
namespace Sentinel
{
	StackTrace::StackTrace() :
		m_FramePointers(32)
	{
		SymInitialize(GetCurrentProcess(), nullptr, true);
	}

	StackTrace::~StackTrace()
	{
		SymCleanup(GetCurrentProcess());
	}

	const std::vector<std::uint64_t>& StackTrace::FramePointers()
	{
		return m_FramePointers;
	}

	void StackTrace::NewStackTrace(EXCEPTION_POINTERS* exception_info)
	{
		static std::mutex m;
		std::lock_guard lock(m);

		m_ExceptionInfo = exception_info;

		m_Dump << ExceptionCodeToString(exception_info->ExceptionRecord->ExceptionCode) << '\n';

		DumpModuleInfo();
		DumpRegisters();
		DumpStackTrace();

		m_Dump << "\n--------End Of Exception--------\n";
	}

	std::string StackTrace::String() const
	{
		return m_Dump.str();
	}

	void StackTrace::DumpModuleInfo()
	{
		// Modules cached already
		if (m_Modules.size())
			return;

		m_Dump << "Dumping Modules:\n";

		const auto PEB = reinterpret_cast<PPEB>(NtCurrentTeb()->ProcessEnvironmentBlock);
		if (!PEB)
			return;

		const auto LDR = PEB->Ldr;
		if (!LDR)
			return;

		const auto ModuleList = &LDR->InMemoryOrderModuleList;
		auto ModuleEntry = ModuleList->Flink;
		for (; ModuleList != ModuleEntry; ModuleEntry = ModuleEntry->Flink)
		{
			const auto TableEntry = CONTAINING_RECORD(ModuleEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			if (!TableEntry)
				continue;

			if (TableEntry->FullDllName.Buffer)
			{
				auto ModuleInfoVar = ModuleInfo(TableEntry->FullDllName.Buffer, TableEntry->DllBase);

				m_Dump << ModuleInfoVar.m_Path.filename().string() << " Base Address: " << HEX_TO_UPPER(ModuleInfoVar.m_Base)
					<< " Size: " << ModuleInfoVar.m_Size << '\n';

				m_Modules.emplace_back(std::move(ModuleInfoVar));
			}
		}
	}

	void StackTrace::DumpRegisters()
	{
		const auto Context = m_ExceptionInfo->ContextRecord;

		m_Dump << "Dumping Registers:\n"
			<< "RAX: " << HEX_TO_UPPER(Context->Rax) << '\n'
			<< "RCX: " << HEX_TO_UPPER(Context->Rcx) << '\n'
			<< "RDX: " << HEX_TO_UPPER(Context->Rdx) << '\n'
			<< "RBX: " << HEX_TO_UPPER(Context->Rbx) << '\n'
			<< "RSI: " << HEX_TO_UPPER(Context->Rsi) << '\n'
			<< "RDI: " << HEX_TO_UPPER(Context->Rdi) << '\n'
			<< "RSP: " << HEX_TO_UPPER(Context->Rsp) << '\n'
			<< "RBP: " << HEX_TO_UPPER(Context->Rbp) << '\n'
			<< "R8:  " << HEX_TO_UPPER(Context->R8) << '\n'
			<< "R9:  " << HEX_TO_UPPER(Context->R9) << '\n'
			<< "R10: " << HEX_TO_UPPER(Context->R10) << '\n'
			<< "R11: " << HEX_TO_UPPER(Context->R11) << '\n'
			<< "R12: " << HEX_TO_UPPER(Context->R12) << '\n'
			<< "R13: " << HEX_TO_UPPER(Context->R13) << '\n'
			<< "R14: " << HEX_TO_UPPER(Context->R14) << '\n'
			<< "R15: " << HEX_TO_UPPER(Context->R15) << '\n';
	}

	void StackTrace::DumpStackTrace()
	{
		m_Dump << "Dumping Stack Trace:";
		GrabStackTrace();

		// Allocate Once
		char Buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
		auto Symbol = reinterpret_cast<SYMBOL_INFO*>(Buffer);
		Symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		Symbol->MaxNameLen = MAX_SYM_NAME;

		DWORD64 Displacement64;
		DWORD Displacement;

		IMAGEHLP_LINE64 Line;
		Line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		for (std::size_t Iterator = 0; Iterator < m_FramePointers.size() && m_FramePointers[Iterator]; ++Iterator)
		{
			const auto Address = m_FramePointers[Iterator];

			m_Dump << "\n[" << Iterator << "]\t";
			if (SymFromAddr(GetCurrentProcess(), Address, &Displacement64, Symbol))
			{
				if (SymGetLineFromAddr64(GetCurrentProcess(), Address, &Displacement, &Line))
				{
					m_Dump << Line.FileName << " L: " << Line.LineNumber << " " << std::string_view(Symbol->Name, Symbol->NameLen);

					continue;
				}
				const auto ModuleInfoVar = GetModuleByAddress(Address);
				m_Dump << ModuleInfoVar->m_Path.filename().string() << " " << std::string_view(Symbol->Name, Symbol->NameLen);

				continue;
			}
			const auto ModuleInfoVar = GetModuleByAddress(Address);
			m_Dump << ModuleInfoVar->m_Path.filename().string() << "+" << HEX_TO_UPPER(Address - ModuleInfoVar->m_Base) << " " << HEX_TO_UPPER(Address);
		}
	}

	void StackTrace::GrabStackTrace()
	{
		CONTEXT Context = *m_ExceptionInfo->ContextRecord;

		STACKFRAME64 Frame{};
		Frame.AddrPC.Mode = AddrModeFlat;
		Frame.AddrFrame.Mode = AddrModeFlat;
		Frame.AddrStack.Mode = AddrModeFlat;
		Frame.AddrPC.Offset = Context.Rip;
		Frame.AddrFrame.Offset = Context.Rbp;
		Frame.AddrStack.Offset = Context.Rsp;

		for (std::size_t Iterator = 0; Iterator < m_FramePointers.size(); ++Iterator)
		{
			if (!StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &Frame, &Context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
			{
				break;
			}
			m_FramePointers[Iterator] = Frame.AddrPC.Offset;
		}
	}

	const StackTrace::ModuleInfo* StackTrace::GetModuleByAddress(std::uint64_t address) const
	{
		for (auto& ModuleInfoVar : m_Modules)
		{
			if (ModuleInfoVar.m_Base < address && address < ModuleInfoVar.m_Base + ModuleInfoVar.m_Size)
			{
				return &ModuleInfoVar;
			}
		}
		return nullptr;
	}

	std::string StackTrace::ExceptionCodeToString(const DWORD code)
	{
#define MAP_PAIR_STRINGIFY(x) \
	{                         \
		x, #x                 \
	}
		static const std::map<DWORD, std::string> Exceptions = { MAP_PAIR_STRINGIFY(EXCEPTION_ACCESS_VIOLATION), MAP_PAIR_STRINGIFY(EXCEPTION_ARRAY_BOUNDS_EXCEEDED), MAP_PAIR_STRINGIFY(EXCEPTION_DATATYPE_MISALIGNMENT), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_DENORMAL_OPERAND), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_DIVIDE_BY_ZERO), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_INEXACT_RESULT), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_INEXACT_RESULT), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_INVALID_OPERATION), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_OVERFLOW), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_STACK_CHECK), MAP_PAIR_STRINGIFY(EXCEPTION_FLT_UNDERFLOW), MAP_PAIR_STRINGIFY(EXCEPTION_ILLEGAL_INSTRUCTION), MAP_PAIR_STRINGIFY(EXCEPTION_IN_PAGE_ERROR), MAP_PAIR_STRINGIFY(EXCEPTION_INT_DIVIDE_BY_ZERO), MAP_PAIR_STRINGIFY(EXCEPTION_INT_OVERFLOW), MAP_PAIR_STRINGIFY(EXCEPTION_INVALID_DISPOSITION), MAP_PAIR_STRINGIFY(EXCEPTION_NONCONTINUABLE_EXCEPTION), MAP_PAIR_STRINGIFY(EXCEPTION_PRIV_INSTRUCTION), MAP_PAIR_STRINGIFY(EXCEPTION_STACK_OVERFLOW), MAP_PAIR_STRINGIFY(EXCEPTION_BREAKPOINT), MAP_PAIR_STRINGIFY(EXCEPTION_SINGLE_STEP) };

		if (const auto& Iterator = Exceptions.find(code); Iterator != Exceptions.end())
			return Iterator->second;

		return "UNKNOWN_EXCEPTION";
	}
}