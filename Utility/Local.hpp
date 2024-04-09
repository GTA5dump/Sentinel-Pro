#pragma once 
#include "../Common.hpp"
#include "Entities.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Include.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../GTA/enums.hpp"
#include "../Features/Self.hpp"
#include "../Features/Vehicle.hpp"
#include "../Pointers/Pointers.hpp"

namespace Sentinel::Local
{
	inline void ParticleOnBone(const char* dict, const char* particle, ePedBones bone, float scale, bool color = false, float r = 1.f, float g = 1.f, float b = 1.f, bool ped = true, Entity entity = PLAYER::PLAYER_PED_ID())
	{
		STREAMING::REQUEST_NAMED_PTFX_ASSET(dict);
		GRAPHICS::USE_PARTICLE_FX_ASSET(dict);
		ped ? GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE(particle, entity, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.0f, bone, scale, TRUE, TRUE, TRUE) : GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY_BONE(particle, entity, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.0f, bone, scale, TRUE, TRUE, TRUE);
		if (color)
			GRAPHICS::SET_PARTICLE_FX_NON_LOOPED_COLOUR(r, g, b);
		STREAMING::REMOVE_PTFX_ASSET();
	}

	inline void RunParticle(const char* particle, const char* id)
	{
		STREAMING::REQUEST_NAMED_PTFX_ASSET(particle);
		GRAPHICS::USE_PARTICLE_FX_ASSET(particle);
		GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_ENTITY(id, PLAYER::PLAYER_PED_ID(), 0.f, 0.f, -0.5f, 0.f, 0.f, 0.f, g_SelfFeatures->m_ParticleOnPedScale, FALSE, FALSE, FALSE);
	}

	inline void RunParticleLooped(const char* particle, const char* id)
	{
		STREAMING::REQUEST_NAMED_PTFX_ASSET(particle);
		GRAPHICS::USE_PARTICLE_FX_ASSET(particle);
		const auto Particle = GRAPHICS::START_NETWORKED_PARTICLE_FX_LOOPED_ON_ENTITY(id, PLAYER::PLAYER_PED_ID(), 0.f, 0.f, -0.5f, 0.f, 0.f, 0.f, g_SelfFeatures->m_ParticleOnPedScale, FALSE, FALSE, FALSE, NULL, NULL, NULL, NULL);
		Script::Current()->Yield(std::chrono::milliseconds(100));
		GRAPHICS::STOP_PARTICLE_FX_LOOPED(Particle, FALSE);
	}

	inline void ParseParticle(int type, int pos)
	{
		switch (type)
		{
		case 0:
			switch (pos)
			{
			case 0:
				RunParticle("scr_indep_fireworks", "scr_indep_firework_trailburst");
				break;
			case 1:
				RunParticle("scr_indep_fireworks", "scr_indep_firework_starburst");
				break;
			case 2:
				RunParticle("scr_indep_fireworks", "scr_indep_firework_shotburst");
				break;
			case 3:
				RunParticle("scr_indep_fireworks", "scr_indep_firework_fountain");
				break;
			case 4:
				RunParticle("proj_xmas_firework", "scr_firework_xmas_ring_burst_rgw");
				break;
			}
			break;
		case 1:
			switch (pos)
			{
			case 0:
				RunParticle("scr_rcbarry2", "scr_clown_appears");
				break;
			case 1:
				RunParticle("scr_rcbarry2", "scr_clown_death");
				break;
			case 2:
				RunParticle("scr_rcbarry2", "scr_exp_clown");
				break;
			}
			break;
		case 2:
			switch (pos)
			{
			case 0:
				RunParticle("scr_rcbarry1", "scr_alien_teleport");
				break;
			case 1:
				RunParticle("scr_rcbarry1", "scr_alien_disintegrate");
				break;
			case 2:
				RunParticleLooped("scr_rcbarry1", "scr_alien_impact");
				break;
			}
			break;
		case 3:
			switch (pos)
			{
			case 0:
				RunParticle("scr_agencyheistb", "scr_agency3b_elec_box");
				break;
			case 1:
				RunParticle("scr_martin1", "ent_amb_elec_crackle_sp");
				break;
			case 2:
				RunParticle("core_snow", "cs_mich1_spade_dirt_throw");
				break;
			case 3:
				RunParticle("scr_reburials", "scr_burial_dirt");
				break;
			case 4:
				RunParticle("scr_familyscenem", "scr_meth_pipe_smoke");
				break;
			}
			break;
		case 4:
			switch (pos)
			{
			case 0:
				RunParticle("core", "liquid_splash_blood");
				break;
			case 1:
				RunParticle("scr_michael2", "scr_mich2_blood_stab");
				break;
			case 2:
				RunParticle("scr_michael2", "scr_abattoir_ped_sliced");
				break;
			case 3:
				RunParticle("scr_martin1", "scr_sol1_sniper_impact");
				break;
			}
			break;
		case 5:
			switch (pos)
			{
			case 0:
				RunParticle("scr_rcpaparazzo1", "scr_rcpap1_camera");
				break;
			case 1:
				RunParticle("scr_ornate_heist", "scr_heist_ornate_banknotes");
				break;
			case 2:
				RunParticle("scr_fbi5a", "scr_fbi5_ped_water_splash");
				break;
			case 3:
				RunParticle("scr_fbi4", "scr_fbi4_trucks_crash");
				break;
			}
			break;
		}
	}

	inline void PlayAnimation(const char* dict, const char* name, bool once = true)
	{
		int Flag = eAnimationFlags::eAnimationRepeat;
		if (g_SelfFeatures->m_ControllableAnimation)
			Flag |= eAnimationFlags::eAnimationEnablePlayerControl | eAnimationFlags::eAnimationUpperBody;
		if (g_SelfFeatures->m_ContortAnimation)
			Flag |= eAnimationFlags::eAnimationCreature;

		while (!STREAMING::HAS_ANIM_DICT_LOADED(dict)) {
			STREAMING::REQUEST_ANIM_DICT(dict);
			Script::Current()->Yield(100ms);
		}

		if (once)
			Entities::ClearPedTasks(PLAYER::PLAYER_PED_ID());
		TASK::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, name, 8.f, 8.f, -1, Flag, 1.f, FALSE, FALSE, FALSE);
	}

	inline void StartClipset(bool weapon, const char* clipset)
	{
		while (!STREAMING::HAS_CLIP_SET_LOADED(clipset)) {
			STREAMING::REQUEST_CLIP_SET(clipset);
			Script::Current()->Yield(100ms);
		}

		if (weapon) {
			PED::RESET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), 1.f);
			PED::SET_PED_WEAPON_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), clipset);
		}
		else {
			PED::RESET_PED_WEAPON_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID());
			PED::SET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), clipset, 1.f);
		}
	}

	inline bool ChangeModel(const Hash hash)
	{
		if (STREAMING::IS_MODEL_IN_CDIMAGE(hash))
		{
			for (std::uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
				STREAMING::REQUEST_MODEL(hash);
			Script::Current()->Yield();
			if (!STREAMING::HAS_MODEL_LOADED(hash))
				return false;
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), hash);
			Script::Current()->Yield();
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			for (int i = 0; i < 12; i++) {
				PED::SET_PED_COMPONENT_VARIATION
				(
					PLAYER::PLAYER_PED_ID(),
					i,
					PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), i),
					PED::GET_PED_TEXTURE_VARIATION(PLAYER::PLAYER_PED_ID(), i),
					PED::GET_PED_PALETTE_VARIATION(PLAYER::PLAYER_PED_ID(), i)
				);
			}
			return true;
		}
		else
			return false;
	}

	inline Vehicle CloneVehicle(Vehicle veh)
	{
		Vehicle SpawnedVehicle{};
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			const auto Model = ENTITY::GET_ENTITY_MODEL(veh);
			while (!STREAMING::HAS_MODEL_LOADED(Model))
			{
				STREAMING::REQUEST_MODEL(Model);
				Script::Current()->Yield(5ms);
			}

			const auto Coords = ENTITY::GET_ENTITY_COORDS(veh, FALSE);
			System::ModelSpawnBypass(true);
			SpawnedVehicle = VEHICLE::CREATE_VEHICLE(Model, Coords.x + 10, Coords.y, Coords.z, ENTITY::GET_ENTITY_HEADING(veh), TRUE, TRUE, FALSE);
			System::ModelSpawnBypass(false);
			Script::Current()->Yield(5ms);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
			if (ENTITY::DOES_ENTITY_EXIST(SpawnedVehicle)) {
				if (*Pointers::pIsSessionStarted)
				{
					DECORATOR::DECOR_SET_INT(SpawnedVehicle, "MPBitset", 0);
					ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(SpawnedVehicle, TRUE);
					const auto NID = NETWORK::VEH_TO_NET(SpawnedVehicle);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(SpawnedVehicle))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NID, TRUE);
					VEHICLE::SET_VEHICLE_IS_STOLEN(SpawnedVehicle, FALSE);
				}

				VEHICLE::SET_VEHICLE_DIRT_LEVEL(SpawnedVehicle, VEHICLE::GET_VEHICLE_DIRT_LEVEL(veh));
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(SpawnedVehicle, VEHICLE::GET_VEHICLE_DIRT_LEVEL(veh));
				VEHICLE::SET_VEHICLE_MOD_KIT(SpawnedVehicle, 0);

				for (int i = MOD_SPOILER; i <= MOD_ARMOR; i++) {
					if (VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) > 0) {
						VEHICLE::SET_VEHICLE_MOD(SpawnedVehicle, i, VEHICLE::GET_VEHICLE_MOD(veh, i), 0);
					}
				}

				for (int i = MOD_PLATEHOLDER; i <= MOD_LIVERY; i++) {
					if (VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) > 0) {
						VEHICLE::SET_VEHICLE_MOD(SpawnedVehicle, i, VEHICLE::GET_VEHICLE_MOD(veh, i), 0);
					}
				}

				VEHICLE::SET_VEHICLE_WHEEL_TYPE(SpawnedVehicle, VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh));
				VEHICLE::SET_VEHICLE_MOD(SpawnedVehicle, MOD_FRONTWHEELS, VEHICLE::GET_VEHICLE_MOD(veh, MOD_FRONTWHEELS), VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, MOD_FRONTWHEELS));
				VEHICLE::SET_VEHICLE_MOD(SpawnedVehicle, MOD_BACKWHEELS, VEHICLE::GET_VEHICLE_MOD(veh, MOD_BACKWHEELS), VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, MOD_FRONTWHEELS));

				for (int i = MOD_UNK17; i <= MOD_XENONLIGHTS; i++) {
					VEHICLE::TOGGLE_VEHICLE_MOD(SpawnedVehicle, VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, i), true);
				}

				int Colors[2];
				int ExtraColors[2];
				RGB TireSmoke;
				RGB CustomPrimary;
				RGB CustomSecondary;
				RGB Neons;

				VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &CustomPrimary.r, &CustomPrimary.g, &CustomPrimary.b);
				VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &CustomSecondary.r, &CustomSecondary.g, &CustomSecondary.b);
				VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &Neons.r, &Neons.g, &Neons.b);
				VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &TireSmoke.r, &TireSmoke.g, &TireSmoke.b);
				VEHICLE::GET_VEHICLE_COLOURS(veh, &Colors[0], &Colors[1]);
				VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &ExtraColors[0], &ExtraColors[1]);

				VEHICLE::SET_VEHICLE_COLOURS(SpawnedVehicle, Colors[0], Colors[1]);
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(SpawnedVehicle, ExtraColors[0], ExtraColors[1]);
				VEHICLE::SET_VEHICLE_WINDOW_TINT(SpawnedVehicle, VEHICLE::GET_VEHICLE_WINDOW_TINT(veh));

				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(SpawnedVehicle, TireSmoke.r, TireSmoke.g, TireSmoke.b);
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(SpawnedVehicle, CustomPrimary.r, CustomPrimary.g, CustomPrimary.b);
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(SpawnedVehicle, CustomSecondary.r, CustomSecondary.g, CustomSecondary.b);

				for (int Iterator = 0; Iterator < 4; Iterator++) {
					VEHICLE::SET_VEHICLE_NEON_ENABLED(SpawnedVehicle, Iterator, VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, Iterator));
				}

				VEHICLE::SET_VEHICLE_NEON_COLOUR(SpawnedVehicle, Neons.r, Neons.g, Neons.b);
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(SpawnedVehicle, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh));
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(SpawnedVehicle, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh));

				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), SpawnedVehicle, -1);
			}
		}

		return SpawnedVehicle;
	}

	inline Object CreateAttachedRamp(Entity target, Hash model, int pos, bool opacitated)
	{
		Entities::RequestControl(target);
		NativeVector3 MinimumModelSize{};
		NativeVector3 MaximumModelSize{};
		const auto Coords = ENTITY::GET_ENTITY_COORDS(target, FALSE);

		MISC::GET_MODEL_DIMENSIONS(model, &MinimumModelSize, &MaximumModelSize);
		const Vector3 Dimensions = { MaximumModelSize.x - MinimumModelSize.x, MaximumModelSize.y - MinimumModelSize.y, MaximumModelSize.z - MinimumModelSize.z };
		System::ModelSpawnBypass(true);
		const auto Ramp = OBJECT::CREATE_OBJECT(model, Coords.x, Coords.y, Coords.z, TRUE, TRUE, FALSE);
		System::ModelSpawnBypass(false);

		switch (pos)
		{
		case 0:
			ENTITY::ATTACH_ENTITY_TO_ENTITY(Ramp, target, 0, 0.f, Dimensions.y, -0.3f, 0.f, 0.f, 180.f, TRUE, TRUE, TRUE, FALSE, 2, TRUE, NULL);
			break;
		case 1:
			ENTITY::ATTACH_ENTITY_TO_ENTITY(Ramp, target, 0, 0.f, (-1) * Dimensions.y, -0.3f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE, 2, TRUE, NULL);
			break;
		case 2:
			ENTITY::ATTACH_ENTITY_TO_ENTITY(Ramp, target, 0, (-1) * Dimensions.x, 0.f, -0.3f, 0.f, 0.f, 270.f, TRUE, TRUE, TRUE, FALSE, 2, TRUE, NULL);
			break;
		case 3:
			ENTITY::ATTACH_ENTITY_TO_ENTITY(Ramp, target, 0, Dimensions.x, 0.f, -0.3f, 0.f, 0.f, 90.f, TRUE, TRUE, TRUE, FALSE, 2, TRUE, NULL);
			break;
		}

		if (opacitated) {
			ENTITY::SET_ENTITY_ALPHA(Ramp, 102, TRUE);
		}

		return Ramp;
	}

	inline const char* GetDisplayNameOfVehicle(Vehicle target)
	{
		return VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(ENTITY::GET_ENTITY_MODEL(target));
	}

	inline const char* GetDisplayNameOfVehicleModel(Hash model)
	{
		return VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model);
	}

	namespace TireTracks
	{
		inline CWheelVfx* m_Cache;
		inline void SetCache()
		{
			m_Cache = (CWheelVfx*)malloc((sizeof(CWheelVfx) * Pointers::pVFXWheel.second) * 4);
			memcpy(m_Cache, Pointers::pVFXWheel.first, (sizeof(CWheelVfx) * Pointers::pVFXWheel.second) * 4);
		}
	}

	namespace TireScaling
	{
		inline Vector2 GetTireScale()
		{
			const std::uint64_t PedFactory = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Pointers::pPedFactory);
			if (PedFactory != NULL) {
				const std::uint64_t Entity = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(PedFactory + 0x8);
				if (Entity != NULL) {
					const std::uint64_t Vehicle = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Entity + 0xD10);
					if (Vehicle != NULL) {
						const std::uint64_t DrawHandler = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Vehicle + 0x48);
						if (DrawHandler != NULL) {
							const std::uint64_t StreamRender = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(DrawHandler + 0x370);
							if (StreamRender != NULL) {
								Vector2 ReturnValue = { reinterpret_cast<VehicleStreamRender*>(StreamRender)->m_tire_width, reinterpret_cast<VehicleStreamRender*>(StreamRender)->m_tire_height };
								return ReturnValue;
							}
						}
					}
				}
			}
		}

		inline void SetTireWidth(float size)
		{
			const std::uint64_t PedFactory = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Pointers::pPedFactory);
			if (PedFactory != NULL) {
				const std::uint64_t Entity = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(PedFactory + 0x8);
				if (Entity != NULL) {
					const std::uint64_t Vehicle = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Entity + 0xD10);
					if (Vehicle != NULL) {
						const std::uint64_t DrawHandler = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Vehicle + 0x48);
						if (DrawHandler != NULL) {
							const std::uint64_t StreamRender = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(DrawHandler + 0x370);
							if (StreamRender != NULL) {
								reinterpret_cast<VehicleStreamRender*>(StreamRender)->m_tire_width = size;
							}
						}
					}
				}
			}
		}

		inline void SetTireHeight(float size)
		{
			const std::uint64_t PedFactory = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Pointers::pPedFactory);
			if (PedFactory != NULL) {
				const std::uint64_t Entity = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(PedFactory + 0x8);
				if (Entity != NULL) {
					const std::uint64_t Vehicle = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Entity + 0xD10);
					if (Vehicle != NULL) {
						const std::uint64_t DrawHandler = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(Vehicle + 0x48);
						if (DrawHandler != NULL) {
							const std::uint64_t StreamRender = *reinterpret_cast<std::add_pointer_t<std::uint64_t>>(DrawHandler + 0x370);
							if (StreamRender != NULL) {
								reinterpret_cast<VehicleStreamRender*>(StreamRender)->m_tire_height = size;
							}
						}
					}
				}
			}
		}
	}

	namespace Dispatch
	{
		inline bool IsToggled(eDispatchType service) 
		{
			if (Pointers::pDispatchServiceTable) {
				auto Table = *(std::uint64_t*)(Pointers::pDispatchServiceTable + (service * 8));
				if (Table) {
					return *(bool*)(Table + 0x20);
				}
			}

			return false;
		}

		inline void SetToggle(eDispatchType service, bool toggle)
		{
			if (Pointers::pDispatchServiceTable) {
				std::uint64_t Table = *(std::uint64_t*)(Pointers::pDispatchServiceTable + (service * 8));
				if (Table) {
					*(bool*)(Table + 0x20) = !toggle;
				}
			}
		}
	}

	namespace Ocean // Thanks Inef for your class
	{
		inline void NoOcean(bool enable)
		{
			for (int Iterator = 0; Iterator < Pointers::pWaterQuads.m_oceanQuads->m_quadCount; Iterator++)
			{
				const std::uint64_t QuadStruct = Pointers::pWaterQuads.m_oceanQuads->m_quadPool + (Iterator * 0x1C);
				const auto Height = *(float*)(QuadStruct + 0x14);
				if (enable && Height == 0.f)
					*(float*)(QuadStruct + 0x14) = -1000.f;
				else if (!enable && Height == -1000.f)
					*(float*)(QuadStruct + 0x14) = 0.f;
			}
		}

		inline void RaiseOcean(bool enable)
		{
			for (int Iterator = 0; Iterator < Pointers::pWaterQuads.m_oceanQuads->m_quadCount; Iterator++)
			{
				const std::uint64_t QuadStruct = Pointers::pWaterQuads.m_oceanQuads->m_quadPool + (Iterator * 0x1C);
				const auto Height = *(float*)(QuadStruct + 0x14);
				if (enable && Height == 0.f)
					*(float*)(QuadStruct + 0x14) = 10.f;
				else if (!enable && Height == 10.f)
					*(float*)(QuadStruct + 0x14) = 0.f;
			}
		}

		inline void TransparentOcean(bool enable)
		{
			for (int Iterator = 0; Iterator < Pointers::pWaterQuads.m_oceanQuads->m_quadCount; Iterator++)
			{
				const std::uint64_t QuadStruct = Pointers::pWaterQuads.m_oceanQuads->m_quadPool + (Iterator * 0x1C);
				if (enable)
					*(int*)(QuadStruct + 0x8) = 0;
				else
					*(int*)(QuadStruct + 0x8) = 0x1A1A1A1A;
			}
		}
	}
}