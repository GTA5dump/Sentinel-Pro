#include "ScriptPatch.hpp"
#include "ScriptData.hpp"

namespace Sentinel
{
	ScriptPatch::ScriptPatch(rage::joaat_t script, const Memory::Pattern pattern, std::int32_t offset, std::vector<std::uint8_t> patch, bool* enable_bool) :
		m_Script(script),
		m_Pattern(pattern),
		m_Offset(offset),
		m_Patch(std::move(patch)),
		m_Bool(enable_bool),
		m_IP(0)
	{
	}

	std::uint8_t* ScriptPatch::GetCodeAddress(ScriptData* data, std::uint32_t index)
	{
		return &data->m_ByteCode[index >> 14][index & 0x3FFF];
	}

	const std::optional<std::uint32_t> ScriptPatch::GetCodeLocationByPattern(ScriptData* data, const Memory::Pattern& pattern)
	{
		std::uint32_t Size = data->m_CodeSize;
		for (std::uint32_t i = 0; i < (Size - pattern.m_Bytes.size()); i++)
		{
			for (std::uint32_t j = 0; j < pattern.m_Bytes.size(); j++)
				if (pattern.m_Bytes[j].has_value())
					if (pattern.m_Bytes[j].value() != *GetCodeAddress(data, i + j))
						goto incorrect;

			return i;
		incorrect:
			continue;
		}

		return std::nullopt;
	}

	void ScriptPatch::Enable(ScriptData* data)
	{
		std::memcpy(GetCodeAddress(data, m_IP), m_Patch.data(), m_Patch.size());
	}

	void ScriptPatch::Disable(ScriptData* data)
	{
		std::memcpy(GetCodeAddress(data, m_IP), m_Original.data(), m_Original.size());
	}

	void ScriptPatch::Update(ScriptData* data)
	{
		if (m_IP == 0)
		{
			auto Result = GetCodeLocationByPattern(data, m_Pattern);
			if (!Result.has_value())
				LOG(FATAL) << "[Script Patcher] Failed to find some patterns.";

			m_IP = Result.value() + m_Offset;

			for (int i = 0; i < m_Patch.size(); i++)
				m_Original.push_back(*GetCodeAddress(data, m_IP + i));
		}

		if (!m_Bool || *m_Bool)
			Enable(data);
		else
			Disable(data);
	}
}