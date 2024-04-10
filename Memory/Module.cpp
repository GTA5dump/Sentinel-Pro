#include "../Common.hpp"
#include "Module.hpp"

namespace Memory
{
	Module::Module(const std::string_view name) :
		Range(nullptr, 0),
		m_Name(name),
		m_Loaded(false)
	{
		TryModule();
	}

	Handle Module::GetExport(std::string_view symbol_name)
	{
		if (!m_Loaded)
			return nullptr;

		const auto dosHeader = m_Base.As<IMAGE_DOS_HEADER*>();
		const auto ntHeader = m_Base.Add(dosHeader->e_lfanew).As<IMAGE_NT_HEADERS*>();
		const auto imageDataDirectory = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		const auto exportDirectory = m_Base.Add(imageDataDirectory.VirtualAddress).As<IMAGE_EXPORT_DIRECTORY*>();

		const auto nameOffsetArray = m_Base.Add(exportDirectory->AddressOfNames).As<DWORD*>();
		const auto ordinalArray = m_Base.Add(exportDirectory->AddressOfNameOrdinals).As<DWORD*>();
		const auto functionOffsetArray = m_Base.Add(exportDirectory->AddressOfFunctions).As<DWORD*>();

		for (std::size_t i = 0; i < exportDirectory->NumberOfFunctions; i++)
		{
			const auto functionName = m_Base.Add(nameOffsetArray[i]).As<const char*>();
			if (strcmp(functionName, symbol_name.data()))
				continue;

			return functionOffsetArray + ordinalArray[i];
		}
		return nullptr;
	}

	bool Module::Loaded() const
	{
		return m_Loaded;
	}

	bool Module::WaitForModule(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		const auto giveup_time = time.has_value()
			? std::make_optional(std::chrono::high_resolution_clock::now() + time.value())
			: std::nullopt;

		while (!TryModule())
		{
			if (giveup_time.has_value() && giveup_time <= std::chrono::high_resolution_clock::now())
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		return m_Loaded;
	}

	bool Module::TryModule()
	{
		if (m_Loaded)
			return m_Loaded;

		const auto mod = GetModuleHandleA(m_Name.data());
		if (!mod)
			return false;
		m_Loaded = true;

		m_Base = mod;
		const auto dosHeader = m_Base.As<IMAGE_DOS_HEADER*>();
		const auto ntHeader = m_Base.Add(dosHeader->e_lfanew).As<IMAGE_NT_HEADERS*>();

		m_Size = ntHeader->OptionalHeader.SizeOfImage;

		return m_Loaded;
	}
}