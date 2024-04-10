#pragma once
#include <functional>
#include <vector>
#include "Pattern.hpp"

namespace Memory
{
	class Batch
	{
	public:
		explicit Batch() = default;
		~Batch() noexcept = default;

		void Add(std::string name, Pattern pattern, std::function<void(Handle)> callback);
		void Add(std::uintptr_t address);
		bool Run(Range region);

		struct Entry
		{
			std::string m_Name;
			Pattern m_Pattern;
			std::function<void(Handle)> m_Callback;

			explicit Entry(std::string name, Pattern pattern, std::function<void(Handle)> callback) :
				m_Name(std::move(name)),
				m_Pattern(std::move(pattern)),
				m_Callback(std::move(callback))
			{}
		};

	private:
		std::vector<Entry> m_Entries;
	};
}