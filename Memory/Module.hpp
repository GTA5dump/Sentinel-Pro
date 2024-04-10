#pragma once
#include <chrono>
#include "Range.hpp"

namespace Memory
{
	class Module : public Range
	{
	public:
		explicit Module(const std::string_view name);

		/**
		 * @brief Get the export address of the current module given a symbol name
		 *
		 * @param symbol_name
		 * @return memory::handle
		 */
		Handle GetExport(std::string_view symbol_name);

		bool Loaded() const;
		/**
		 * @brief Waits till the given module is loaded.
		 *
		 * @param time Time to wait before giving up.
		 * @return true
		 * @return false
		 */
		bool WaitForModule(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);

	protected:
		bool TryModule();

	private:
		const std::string_view m_Name;
		bool m_Loaded;

	};
}