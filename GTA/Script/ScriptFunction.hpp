#pragma once 
#include "../Include.hpp"
#include "../../Memory/Pattern.hpp"

namespace Sentinel
{
	class ScriptFunction
	{
		rage::joaat_t m_Script;
		const Memory::Pattern m_Pattern;
		std::int32_t m_Offset;
		std::int32_t m_IP;
		std::string m_Name;

	public:
		ScriptFunction(const std::string& name, const rage::joaat_t script, const std::string& pattern, int32_t offset);
		void PopulateIP();
		void Call(rage::scrThread* thread, rage::scrProgram* program, std::initializer_list<std::uint64_t> args);
		
		// Util
		static rage::scrProgram* FindScriptProgram(rage::joaat_t hash);
		static const std::optional<std::uint32_t> GetCodeLocationByPattern(rage::scrProgram* program, const Memory::Pattern& pattern);

		void operator()(std::initializer_list<std::uint64_t> args);
	};

	namespace ScriptFunctions
	{
		static inline ScriptFunction JoinCEO("JC", rage::joaat("freemode"), "2D 04 1D 00 00 5D", 0);
	}
}