#pragma once
#include "../Common.hpp"

namespace Sentinel
{
	class StackTrace
	{
	public:
		StackTrace();
		virtual ~StackTrace();

		const std::vector<uint64_t>& FramePointers();
		void NewStackTrace(EXCEPTION_POINTERS* exception_info);
		std::string String() const;

		friend std::ostream& operator<<(std::ostream& os, const StackTrace& st);
		friend std::ostream& operator<<(std::ostream& os, const StackTrace* st);

	private:
		struct ModuleInfo
		{
			ModuleInfo(std::filesystem::path path, void* base) :
				m_Path(path),
				m_Base(reinterpret_cast<std::uintptr_t>(base))
			{
				const auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
				const auto ntHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(m_Base + dosHeader->e_lfanew);

				m_Size = ntHeader->OptionalHeader.SizeOfCode;
			}

			std::filesystem::path m_Path;
			std::uintptr_t m_Base;
			std::size_t m_Size;
		};

	private:
		void DumpModuleInfo();
		void DumpRegisters();
		void DumpStackTrace();
		void GrabStackTrace();
		const ModuleInfo* GetModuleByAddress(std::uint64_t address) const;

		static std::string ExceptionCodeToString(const DWORD code);

	private:
		EXCEPTION_POINTERS* m_ExceptionInfo;

		std::stringstream m_Dump;
		std::vector<std::uint64_t> m_FramePointers;

		inline static std::vector<ModuleInfo> m_Modules;
	};

	inline std::ostream& operator<<(std::ostream& os, const StackTrace& st)
	{
		os << st.String();

		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const StackTrace* st)
	{
		os << st->String();

		return os;
	}
}