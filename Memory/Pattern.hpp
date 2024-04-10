#pragma once
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>
#include "Declaration.hpp"
#include "Handle.hpp"

namespace Memory
{
	class Pattern
	{
		friend PatternBatch;
		friend Range;
	public:
		Pattern(std::string_view ida_sig);
		explicit Pattern(const void* bytes, std::string_view mask);

		inline Pattern(const char* ida_sig) :
			Pattern(std::string_view(ida_sig))
		{}

		std::vector<std::optional<std::uint8_t>> m_Bytes;
	};
}