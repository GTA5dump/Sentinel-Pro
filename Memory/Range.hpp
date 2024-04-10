#pragma once
#include <vector>
#include "Declaration.hpp"
#include "Handle.hpp"

namespace Memory
{
	class Range
	{
	public:
		Range(Handle base, std::size_t size);

		Handle Begin();
		Handle End();
		std::size_t Size();

		bool Contains(Handle h);

		Handle Scan(Pattern const& sig);
		std::vector<Handle> ScanAll(Pattern const& sig);
	protected:
		Handle m_Base;
		std::size_t m_Size;
	};
}