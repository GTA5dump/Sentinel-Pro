#include "../Common.hpp"
#include "Range.hpp"
#include "Pattern.hpp"

namespace Memory
{
	Range::Range(Handle base, std::size_t size) :
		m_Base(base), m_Size(size)
	{
	}

	Handle Range::Begin()
	{
		return m_Base;
	}

	Handle Range::End()
	{
		return m_Base.Add(m_Size);
	}

	std::size_t Range::Size()
	{
		return m_Size;
	}

	bool Range::Contains(Handle h)
	{
		return h.As<std::uintptr_t>() >= Begin().As<std::uintptr_t>() && h.As<std::uintptr_t>() <= End().As<std::uintptr_t>();
	}

	// https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore%E2%80%93Horspool_algorithm
	// https://www.youtube.com/watch?v=AuZUeshhy-s
	Handle ScanPattern(const std::optional<std::uint8_t>* sig, std::size_t length, Handle begin, std::size_t module_size)
	{
		std::size_t maxShift = length;
		std::size_t max_idx = length - 1;

		// Get wildcard index, and store max shiftable byte count
		std::size_t wild_card_idx{ static_cast<size_t>(-1) };
		for (int i{ static_cast<int>(max_idx - 1) }; i >= 0; --i)
		{
			if (!sig[i])
			{
				maxShift = max_idx - i;
				wild_card_idx = i;
				break;
			}
		}

		// Store max shiftable bytes for non wildcards.
		std::size_t shift_table[UINT8_MAX + 1]{};
		for (std::size_t i{}; i <= UINT8_MAX; ++i)
		{
			shift_table[i] = maxShift;
		}

		// Fill shift table with sig bytes
		for (std::size_t i{ wild_card_idx + 1 }; i != max_idx; ++i)
		{
			shift_table[*sig[i]] = max_idx - i;
		}

		// Loop data
		const auto scan_end = module_size - length;
		for (std::size_t current_idx{}; current_idx <= scan_end;)
		{
			for (std::ptrdiff_t sig_idx{ (std::ptrdiff_t)max_idx }; sig_idx >= 0; --sig_idx)
			{
				if (sig[sig_idx] && *begin.Add(current_idx + sig_idx).As<uint8_t*>() != *sig[sig_idx])
				{
					current_idx += shift_table[*begin.Add(current_idx + max_idx).As<uint8_t*>()];
					break;
				}
				else if (sig_idx == NULL)
				{
					return begin.Add(current_idx);
				}
			}
		}
		return nullptr;
	}

	Handle Range::Scan(Pattern const& sig)
	{
		auto data = sig.m_Bytes.data();
		auto length = sig.m_Bytes.size();

		if (auto result = ScanPattern(data, length, m_Base, m_Size); result)
		{
			return result;
		}

		return nullptr;
	}

	bool PatternMatches(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length)
	{
		for (std::size_t i{}; i != length; ++i)
		{
			if (sig[i] && *sig[i] != target[i])
			{
				return false;
			}
		}

		return true;
	}

	std::vector<Handle> Range::ScanAll(Pattern const& sig)
	{
		std::vector<Handle> result{};
		auto data = sig.m_Bytes.data();
		auto length = sig.m_Bytes.size();

		const auto scan_end = m_Size - length;
		for (std::uintptr_t i{}; i != scan_end; ++i)
		{
			if (PatternMatches(m_Base.Add(i).As<std::uint8_t*>(), data, length))
			{
				result.push_back(m_Base.Add(i));
			}
		}

		return std::move(result);
	}
}