#pragma once
#include "../../Common.hpp"

namespace Sentinel
{
	class ScriptData
	{
		std::uint32_t m_PageCount;

	public:
		std::uint32_t m_CodeSize;
		std::uint8_t** m_ByteCode;

		ScriptData(std::uint32_t size, std::uint8_t** bytes, std::uint32_t pages) :
			m_CodeSize(size),
			m_ByteCode(bytes),
			m_PageCount(pages)
		{
		}

		ScriptData(const ScriptData& that) = delete;
		ScriptData& operator=(const ScriptData& that) = delete;

		~ScriptData()
		{
			for (auto i = 0u; i < m_PageCount; i++)
			{
				delete[] m_ByteCode[i];
			}

			delete[] m_ByteCode;
			m_ByteCode = nullptr;
		}
	};
}