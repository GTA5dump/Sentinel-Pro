#pragma once
#include <string_view>

namespace Sentinel
{
	class IHook
	{
	public:
		IHook() = default;
		virtual ~IHook() = default;

		IHook(IHook&& that) = delete;
		IHook& operator=(IHook&& that) = delete;
		IHook(IHook const&) = delete;
		IHook& operator=(IHook const&) = delete;

		virtual const std::string_view Name() const = 0;

		virtual bool Enable() = 0;
		virtual bool Disable() = 0;

		virtual bool IsEnabled() const = 0;
	};
}