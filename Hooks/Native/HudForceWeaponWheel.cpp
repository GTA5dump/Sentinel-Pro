#include "../../GTA/Invoker/Natives.hpp"
#include "../../Features/Weapon.hpp"
#include "../HookTable.hpp"

namespace Sentinel
{
	void Native::HudForceWeaponWheel(rage::scrNativeCallContext* src)
	{
		const auto show = src->get_arg<BOOL>(0);

		if (g_WeaponFeatures->m_InteriorWeapons && !show || g_WeaponFeatures->m_PassiveWeapons || !show)
			return;

		HUD::HUD_FORCE_WEAPON_WHEEL(show);
	}
}