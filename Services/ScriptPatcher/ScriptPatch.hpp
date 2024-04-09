#pragma once
#include "../../Common.hpp"
#include "../../GTA/Include.hpp"
#include "../../Memory/Pattern.hpp"

namespace Sentinel
{
	struct ScriptData;

	class ScriptPatch
	{
		rage::joaat_t m_Script;
		const Memory::Pattern m_Pattern;
		std::int32_t m_Offset;
		std::vector<std::uint8_t> m_Patch;
		std::vector<std::uint8_t> m_Original;
		bool* m_Bool;
		std::int32_t m_IP;

		static std::uint8_t* GetCodeAddress(ScriptData* data, std::uint32_t index);
		static const std::optional<std::uint32_t> GetCodeLocationByPattern(ScriptData* data, const Memory::Pattern& pattern);
		void Enable(ScriptData* data);
		void Disable(ScriptData* data);

	public:
		rage::joaat_t GetScript()
		{
			return m_Script;
		}

		ScriptPatch(rage::joaat_t script, const Memory::Pattern pattern, std::int32_t offset, std::vector<std::uint8_t> patch, bool* enable_bool);
		void Update(ScriptData* data);
	};
}