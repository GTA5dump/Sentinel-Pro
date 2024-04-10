#include "../../GTA/Invoker/Natives.hpp"
#include "../../Features/Weapon.hpp"
#include "../HookTable.hpp"

namespace Sentinel
{
	void Native::SetCurrentPedWeapon(rage::scrNativeCallContext* src)
	{
		const auto aPed = src->get_arg<Ped>(0);
		const auto weaponHash = src->get_arg<Hash>(1);
		const auto bForceInHand = src->get_arg<BOOL>(2);

		if (aPed == PLAYER::PLAYER_PED_ID() && g_WeaponFeatures->m_InteriorWeapons || aPed == PLAYER::PLAYER_PED_ID() && g_WeaponFeatures->m_PassiveWeapons)
			return;

		WEAPON::SET_CURRENT_PED_WEAPON(aPed, weaponHash, bForceInHand);
	}
}