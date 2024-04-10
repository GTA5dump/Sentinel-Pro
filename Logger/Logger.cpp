#pragma once
#include "Logger.hpp"
#include "../Memory/Module.hpp"

namespace Sentinel
{
	template<typename TP>
	std::time_t ToTimeT(TP tp)
	{
		using namespace std::chrono;
		const auto Temp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
		return system_clock::to_time_t(Temp);
	}

	Log::Log(std::string_view console_title, std::filesystem::path file, std::filesystem::path backup_file, bool attach_console) :
		m_AttachConsole(attach_console),
		m_DidConsoleExist(false),
		m_ConsoleLogger(&Log::FormatConsole),
		m_ConsoleTitle(console_title),
		m_OriginalConsoleMode(0),
		m_ConsoleHandle(nullptr),
		m_File(file),
		m_BackupFile(backup_file)
	{
		auto ModuleHandle = Memory::Module("ntdll.dll");
		if (const auto NoColor = std::getenv("NO_COLOR"); ModuleHandle.GetExport("wine_get_version") || (NoColor && strlen(NoColor)))
		{
			LOG(VERBOSE) << "[Main] Using simple logger";
			m_ConsoleLogger = &Log::FormatConsoleSimple;
		}

		Initialize();

		g_Log = this;
	}

	Log::~Log()
	{
		g_Log = nullptr;
	}

	void Log::Initialize()
	{
		if (m_AttachConsole)
		{
			if (m_DidConsoleExist = ::AttachConsole(GetCurrentProcessId()); !m_DidConsoleExist)
				AllocConsole();

			if (m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); m_ConsoleHandle != nullptr)
			{
				SetConsoleTitleA(m_ConsoleTitle.data());
				SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 235, LWA_ALPHA); // Transparent Background
				SetConsoleOutputCP(CP_UTF8);

				DWORD ConsoleMode;
				GetConsoleMode(m_ConsoleHandle, &ConsoleMode);
				m_OriginalConsoleMode = ConsoleMode;

				// Terminal like behaviour enable full color support
				ConsoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
				// Prevent clicking in terminal from suspending our main thread
				ConsoleMode &= ~(ENABLE_QUICK_EDIT_MODE);

				SetConsoleMode(m_ConsoleHandle, ConsoleMode);
			}
		}
		OpenOutstreams();

		Logger::Init();
		Logger::AddSink([this](LogMessagePtr msg) {
			(this->*m_ConsoleLogger)(std::move(msg));
		});
		Logger::AddSink([this](LogMessagePtr msg) {
			FormatFile(std::move(msg));
		});
	}

	void Log::Destroy()
	{
		CreateBackup();
		Logger::Destroy();
		CloseOutstreams();

		if (m_DidConsoleExist)
			SetConsoleMode(m_ConsoleHandle, m_OriginalConsoleMode);

		if (!m_DidConsoleExist && m_AttachConsole)
			FreeConsole();

		std::filesystem::remove(m_File);
	}

	void Log::CreateBackup()
	{
		if (std::filesystem::exists(m_File))
		{
			const auto FileTime = std::filesystem::last_write_time(m_File);
			const auto TimeT = ToTimeT(FileTime);
			const auto LocalTime = std::localtime(&TimeT);

			const auto FileTimestamp = std::format("{:0>2}-{:0>2}-{}-{:0>2}-{:0>2}-{:0>2} Cheat.log", LocalTime->tm_mon + 1, LocalTime->tm_mday, LocalTime->tm_year + 1900, LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec);

			std::filesystem::copy_file(m_File, m_BackupFile / FileTimestamp);
		}
	}

	void Log::OpenOutstreams()
	{
		if (m_AttachConsole)
			m_ConsoleOut.open("CONOUT$", std::ios_base::out | std::ios_base::app);

		m_FileOut.open(m_File, std::ios_base::out | std::ios_base::trunc);
	}

	void Log::CloseOutstreams()
	{
		if (m_AttachConsole)
			m_ConsoleOut.close();

		m_FileOut.close();
	}

	const eLogColor GetColor(const eLogLevel level)
	{
		switch (level)
		{
		case VERBOSE: return eLogColor::BLUE;
		case INFO: return eLogColor::CYAN;
		case WARNING: return eLogColor::YELLOW;
		case FATAL: return eLogColor::RED;
		}
		return eLogColor::WHITE;
	}

	const char* GetLevelString(const eLogLevel level)
	{
		constexpr std::array<const char*, 4> Levelstrings = { {{"Debug"}, {"Info"}, {"Warn"}, {"Error"}} };

		return Levelstrings[level];
	}

	void Log::FormatConsole(const LogMessagePtr msg)
	{
		const auto Color = GetColor(msg->Level());

		const auto Timestamp = std::format("{0:%H:%M:%S}", msg->Timestamp());
		const auto& location = msg->Location();
		const auto Level = msg->Level();

		const auto File = std::filesystem::path(location.file_name()).filename().string();

		m_ConsoleOut << ADD_COLOR_TO_STREAM(Color) << "[" << GetLevelString(Level) << "] " << RESET_STREAM_COLOR << msg->Message() << std::flush;
	}

	void Log::FormatConsoleSimple(const LogMessagePtr msg)
	{
		const auto Timestamp = std::format("{0:%H:%M:%S}", msg->Timestamp());
		const auto& Location = msg->Location();
		const auto Level = msg->Level();

		const auto File = std::filesystem::path(Location.file_name()).filename().string();

		m_ConsoleOut << "[" << Timestamp << "]"
			<< "[" << GetLevelString(Level) << "/" << File << ":" << Location.line() << "] " << msg->Message() << std::flush;
	}

	void Log::FormatFile(const LogMessagePtr msg)
	{
		if (!m_FileOut.is_open())
			return;

		const auto Timestamp = std::format("{0:%H:%M:%S}", msg->Timestamp());
		const auto& Location = msg->Location();
		const auto Level = msg->Level();

		const auto File = std::filesystem::path(Location.file_name()).filename().string();

		m_FileOut << "[" << Timestamp << "]"
			<< " [" << GetLevelString(Level) << "]" << " [" << File << ":" << Location.line() << "] " << msg->Message() << std::flush;
	}
}