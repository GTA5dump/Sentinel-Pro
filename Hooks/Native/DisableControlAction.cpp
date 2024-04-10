#include "../../GTA/Invoker/Natives.hpp"
#include "../../Features/Weapon.hpp"
#include "../HookTable.hpp"

namespace Sentinel
{
	void Native::DisableControlAction(rage::scrNativeCallContext* src)
	{
		const auto control = src->get_arg<int>(0);
		const auto action = src->get_arg<int>(1);
		const auto disableRelatedActions = src->get_arg<BOOL>(2);

		if (g_WeaponFeatures->m_InteriorWeapons || g_WeaponFeatures->m_PassiveWeapons)
		{
			switch (action)
			{
			// case eControllerInputs::INPUT_JUMP: TODO: add as separate feature
			case eControllerInputs::INPUT_ATTACK:
			case eControllerInputs::INPUT_AIM:
			case eControllerInputs::INPUT_DUCK:
			case eControllerInputs::INPUT_SELECT_WEAPON:
			case eControllerInputs::INPUT_COVER:
			case eControllerInputs::INPUT_TALK:
			case eControllerInputs::INPUT_DETONATE:
			case eControllerInputs::INPUT_WEAPON_SPECIAL:
			case eControllerInputs::INPUT_WEAPON_SPECIAL_TWO:
			case eControllerInputs::INPUT_VEH_AIM:
			case eControllerInputs::INPUT_VEH_ATTACK:
			case eControllerInputs::INPUT_VEH_ATTACK2:
			case eControllerInputs::INPUT_VEH_HEADLIGHT:
			case eControllerInputs::INPUT_VEH_NEXT_RADIO:
			case eControllerInputs::INPUT_VEH_PREV_RADIO:
			case eControllerInputs::INPUT_VEH_NEXT_RADIO_TRACK:
			case eControllerInputs::INPUT_VEH_PREV_RADIO_TRACK:
			case eControllerInputs::INPUT_VEH_RADIO_WHEEL:
			case eControllerInputs::INPUT_VEH_PASSENGER_AIM:
			case eControllerInputs::INPUT_VEH_PASSENGER_ATTACK:
			case eControllerInputs::INPUT_VEH_SELECT_NEXT_WEAPON:
			case eControllerInputs::INPUT_VEH_SELECT_PREV_WEAPON:
			case eControllerInputs::INPUT_VEH_ROOF:
			case eControllerInputs::INPUT_VEH_JUMP:
			case eControllerInputs::INPUT_VEH_FLY_ATTACK:
			case eControllerInputs::INPUT_MELEE_ATTACK_LIGHT:
			case eControllerInputs::INPUT_MELEE_ATTACK_HEAVY:
			case eControllerInputs::INPUT_MELEE_ATTACK_ALTERNATE:
			case eControllerInputs::INPUT_MELEE_BLOCK:
			case eControllerInputs::INPUT_SELECT_WEAPON_UNARMED:
			case eControllerInputs::INPUT_SELECT_WEAPON_MELEE:
			case eControllerInputs::INPUT_SELECT_WEAPON_HANDGUN:
			case eControllerInputs::INPUT_SELECT_WEAPON_SHOTGUN:
			case eControllerInputs::INPUT_SELECT_WEAPON_SMG:
			case eControllerInputs::INPUT_SELECT_WEAPON_AUTO_RIFLE:
			case eControllerInputs::INPUT_SELECT_WEAPON_SNIPER:
			case eControllerInputs::INPUT_SELECT_WEAPON_HEAVY:
			case eControllerInputs::INPUT_SELECT_WEAPON_SPECIAL:
			case eControllerInputs::INPUT_ATTACK2:
			case eControllerInputs::INPUT_MELEE_ATTACK1:
			case eControllerInputs::INPUT_MELEE_ATTACK2:
			case eControllerInputs::INPUT_VEH_GUN_LEFT:
			case eControllerInputs::INPUT_VEH_GUN_RIGHT:
			case eControllerInputs::INPUT_VEH_GUN_UP:
			case eControllerInputs::INPUT_VEH_GUN_DOWN:
			case eControllerInputs::INPUT_VEH_HYDRAULICS_CONTROL_TOGGLE:
			case eControllerInputs::INPUT_VEH_MELEE_HOLD:
			case eControllerInputs::INPUT_VEH_MELEE_LEFT:
			case eControllerInputs::INPUT_VEH_MELEE_RIGHT:
			case eControllerInputs::INPUT_VEH_CAR_JUMP:
			case eControllerInputs::INPUT_VEH_ROCKET_BOOST:
			case eControllerInputs::INPUT_VEH_FLY_BOOST:
			case eControllerInputs::INPUT_VEH_PARACHUTE:
			case eControllerInputs::INPUT_VEH_BIKE_WINGS:
			case eControllerInputs::INPUT_VEH_TRANSFORM: 
				return;
			}
		}

		PAD::DISABLE_CONTROL_ACTION(control, action, disableRelatedActions);
	}
}