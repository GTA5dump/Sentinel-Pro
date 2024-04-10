#pragma once
#include "../Common.hpp"
#include <AsyncLogger/Logger.hpp>
#include <AsyncLogger/LogLevel.hpp>
using namespace al;
#define ADD_COLOR_TO_STREAM(color) "\x1b[" << int(color) << "m"
#define RESET_STREAM_COLOR "\x1b[" << int(eLogColor::RESET) << "m"
#define HEX_TO_UPPER(value) "0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase

namespace Sentinel
{
	enum class eLogColor
	{
		RESET,
		WHITE = 97,
		CYAN = 36,
		MAGENTA = 35,
		BLUE = 34,
		GREEN = 32,
		YELLOW = 33,
		RED = 31,
		BLACK = 30
	};

	class Log final
	{
	public:
		Log(std::string_view console_title, std::filesystem::path file, std::filesystem::path backup_file, bool attach_console = true);
		virtual ~Log();

		void Initialize();
		void Destroy();

	private:
		void CreateBackup();

		void OpenOutstreams();
		void CloseOutstreams();

		void FormatConsole(const LogMessagePtr msg);
		void FormatConsoleSimple(const LogMessagePtr msg);
		void FormatFile(const LogMessagePtr msg);

	private:
		bool m_AttachConsole;
		bool m_DidConsoleExist;

		void (Log::*m_ConsoleLogger)(const LogMessagePtr msg);

		std::string_view m_ConsoleTitle;
		DWORD m_OriginalConsoleMode;
		HANDLE m_ConsoleHandle;

		std::ofstream m_ConsoleOut;
		std::ofstream m_FileOut;

		std::filesystem::path m_File;		
		std::filesystem::path m_BackupFile;
	};

	inline Log* g_Log = nullptr;
}