#include "FeatureManager.hpp"
#include "Self.hpp"
#include "Players.hpp"
#include "Network.hpp"
#include "ProtectionScript.hpp"
#include "Weapon.hpp"
#include "Vehicle.hpp"
#include "Misc.hpp"
#include "../GTA/Script/Script.hpp"
#include "../GTA/Script/ScriptManager.hpp"
#include "../GTA/Script/ScriptPatches.hpp"
#include "../Utility/Local.hpp"
#include "../Utility/Weapons.hpp"
#include "../Utility/VehicleSpawner.hpp"

namespace Sentinel
{
	// Local Ticks
	static void ScriptPatcherTick()
	{
		ScriptPatches::RegisterScriptPatches();
		while (true)
		{
			Script::Current()->Yield();
		}
	}

	static void SelfFeaturesTick()
	{
		g_SelfFeatures->m_HandTrailsColor[0] = 99.f / 255.f;
		g_SelfFeatures->m_HandTrailsColor[1] = 66.f / 255.f;
		g_SelfFeatures->m_HandTrailsColor[2] = 245.f / 255.f;
		while (true)
		{
			g_SelfFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	static void PlayersFeaturesTick()
	{
		while (true)
		{
			g_PlayersFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	static void IterateTrollingOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateTrollingOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateShootingOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateShootingOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateActionDenyingOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateActionDenyingOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateExplosionCreatorOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateExplosionCreatorOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateRemoteOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateRemoteOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateEntitySpammerTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateEntitySpammer();
			Script::Current()->Yield();
		}
	}

	static void IterateChatSpammerTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateChatSpammer();
			Script::Current()->Yield();
		}
	}

	static void IterateSoundSpammerTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateSoundSpammer();
			Script::Current()->Yield();
		}
	}

	static void IterateAngryPlanesTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateAngryPlanes();
			Script::Current()->Yield();
		}
	}

	static void IterateFriendlyOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateFriendlyOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateDropManagerTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateDropManager();
			Script::Current()->Yield();
		}
	}

	static void IterateForcefieldTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateForcefield();
			Script::Current()->Yield();
		}
	}

	static void IterateWeaponOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateWeaponOptions();
			Script::Current()->Yield();
		}
	}

	static void IterateVehicleOptionsTick()
	{
		while (true)
		{
			g_PlayersFeatures->IterateVehicleOptions();
			Script::Current()->Yield();
		}
	}

	static void NetworkFeaturesTick()
	{
		while (true)
		{
			g_NetworkFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	static void WeaponFeaturesTick()
	{
		g_WeaponFeatures->m_BulletTracerColor[0] = 99.f / 255.f;
		g_WeaponFeatures->m_BulletTracerColor[1] = 66.f / 255.f;
		g_WeaponFeatures->m_BulletTracerColor[2] = 245.f / 255.f;

		g_WeaponFeatures->m_PaintGunColor[0] = 99.f / 255.f;
		g_WeaponFeatures->m_PaintGunColor[1] = 66.f / 255.f;
		g_WeaponFeatures->m_PaintGunColor[2] = 245.f / 255.f;

		g_WeaponFeatures->m_CrosshairColor[0] = 99.f / 255.f;
		g_WeaponFeatures->m_CrosshairColor[1] = 66.f / 255.f;
		g_WeaponFeatures->m_CrosshairColor[2] = 245.f / 255.f;

		while (true)
		{
			g_WeaponFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	static void VehicleFeaturesTick()
	{
		SpawnVehicles::AllocateVectors();
		LOG(INFO) << "[Vehicles] Allocated List Pool Vectors";

		Local::TireTracks::SetCache();
		g_VehicleFeatures->m_TireTracksColor[0] = 99.f / 255.f;
		g_VehicleFeatures->m_TireTracksColor[1] = 66.f / 255.f;
		g_VehicleFeatures->m_TireTracksColor[2] = 245.f / 255.f;
		while (true)
		{
			g_VehicleFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	static void MiscFeaturesTick()
	{
		for (int Iterator = 0; Iterator < sizeof(g_MiscFeatures->m_SkyVFXQuadrant); Iterator++) 
		{
			g_MiscFeatures->m_SkyVFXIntensity[Iterator] = 1.f;
			g_MiscFeatures->m_SkyVFXColor[Iterator][0] = 99.f / 255.f;
			g_MiscFeatures->m_SkyVFXColor[Iterator][1] = 66.f / 255.f;
			g_MiscFeatures->m_SkyVFXColor[Iterator][2] = 245.f / 255.f;
		}
		g_MiscFeatures->m_RainIntensity = MISC::GET_RAIN_LEVEL();
		g_MiscFeatures->m_RainIntensityCache = MISC::GET_RAIN_LEVEL();
		g_MiscFeatures->m_WaveIntensity = WATER::GET_DEEP_OCEAN_SCALER();
		g_MiscFeatures->m_WaveIntensityCache = WATER::GET_DEEP_OCEAN_SCALER();
		g_MiscFeatures->m_WindSpeed = MISC::GET_WIND_SPEED();
		g_MiscFeatures->m_WindSpeedCache = MISC::GET_WIND_SPEED();
		g_MiscFeatures->m_WindDirection = MISC::GET_WIND_DIRECTION().x;
		g_MiscFeatures->m_WindDirectionCache = MISC::GET_WIND_DIRECTION().x;
		g_MiscFeatures->m_FogOnMap = *(float*)Pointers::pFOGOnMap;
		g_MiscFeatures->m_FogOnMapCache = *(float*)Pointers::pFOGOnMap;
		g_MiscFeatures->m_WaterOnMap = *(float*)Pointers::pWaterOnMap;
		g_MiscFeatures->m_WaterOnMapCache = *(float*)Pointers::pWaterOnMap;
		g_MiscFeatures->m_FieldOfView = 50.f;
		g_MiscFeatures->m_FieldOfViewCache = 50.f;

		g_MiscFeatures->m_WorldLuminanceColor[0] = 99.f / 255.f;
		g_MiscFeatures->m_WorldLuminanceColor[1] = 66.f / 255.f;
		g_MiscFeatures->m_WorldLuminanceColor[2] = 245.f / 255.f;

		for (int Iterator = 0; Iterator < sizeof(g_MiscFeatures->m_HUDColors); Iterator++) {
			int R, G, B, A;
			HUD::GET_HUD_COLOUR(Iterator, &R, &G, &B, &A);
			g_MiscFeatures->m_ModifyHUDColor[Iterator][0] = R / 255.f;
			g_MiscFeatures->m_ModifyHUDColor[Iterator][1] = G / 255.f;
			g_MiscFeatures->m_ModifyHUDColor[Iterator][2] = B / 255.f;
		}

		g_MiscFeatures->m_DisablePoliceAutomobile = !Local::Dispatch::IsToggled(DT_PoliceAutomobile);
		g_MiscFeatures->m_DisablePoliceHelicopter = !Local::Dispatch::IsToggled(DT_PoliceHelicopter);
		g_MiscFeatures->m_DisablePoliceRiders = !Local::Dispatch::IsToggled(DT_PoliceRiders);
		g_MiscFeatures->m_DisablePoliceVehicleRequest = !Local::Dispatch::IsToggled(DT_PoliceVehicleRequest);
		g_MiscFeatures->m_DisablePoliceRoadblock = !Local::Dispatch::IsToggled(DT_PoliceRoadBlock);
		g_MiscFeatures->m_DisablePoliceBoat = !Local::Dispatch::IsToggled(DT_PoliceBoat);
		g_MiscFeatures->m_DisablSwatAutomobile = !Local::Dispatch::IsToggled(DT_SwatAutomobile);
		g_MiscFeatures->m_DisableSwatHelicopter = !Local::Dispatch::IsToggled(DT_SwatHelicopter);
		g_MiscFeatures->m_DisableFireDepartment = !Local::Dispatch::IsToggled(DT_FireDepartment);
		g_MiscFeatures->m_DisableAmbulanceDepartment = !Local::Dispatch::IsToggled(DT_AmbulanceDepartment);
		g_MiscFeatures->m_DisableGangs = !Local::Dispatch::IsToggled(DT_Gangs);
		g_MiscFeatures->m_DisableArmyVehicles = !Local::Dispatch::IsToggled(DT_ArmyVehicle);
		g_MiscFeatures->m_DisableBikers = !Local::Dispatch::IsToggled(DT_BikerBackup);

		for (std::uint32_t Iterator = 0; Iterator < 0xA; Iterator++) 
		{
			CWeatherVfx* CurrentShader = &Pointers::pWeatherVfx[Iterator];
			if (!System::GetShaderName(CurrentShader->m_name_hash).empty())
			{
				float Luminance = 1.f;
				g_MiscFeatures->m_NewShaders.push_back({ CurrentShader, CurrentShader->m_color.x * Luminance, CurrentShader->m_color.y * Luminance, CurrentShader->m_color.z * Luminance, Luminance });
			}
		}

		for (int Iterator = 0; Iterator < sizeof(g_MiscFeatures->m_LiquidVFXTypes); Iterator++)
		{
			const RGB Color = { Pointers::pLiquidVFX[Iterator].r, Pointers::pLiquidVFX[Iterator].g, Pointers::pLiquidVFX[Iterator].b };
			g_MiscFeatures->m_LiquidVFXColor[Iterator][0] = Color.r / 255.f;
			g_MiscFeatures->m_LiquidVFXColor[Iterator][1] = Color.g / 255.f;
			g_MiscFeatures->m_LiquidVFXColor[Iterator][2] = Color.b / 255.f;
		}

		while (true)
		{
			g_MiscFeatures->Tick();
			Script::Current()->Yield();
		}
	}

	void FeatureManager::Initialize()
	{
		if (*Pointers::pIsSessionStarted)
			ScriptManager::Initialize(std::make_unique<Script>(&ScriptPatcherTick));

		g_ProtectionScript = std::make_unique<ProtectionScript>();

		g_SelfFeatures = std::make_unique<SelfFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&SelfFeaturesTick));

		g_PlayersFeatures = std::make_unique<PlayersFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&PlayersFeaturesTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateTrollingOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateShootingOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateActionDenyingOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateExplosionCreatorOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateRemoteOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateEntitySpammerTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateChatSpammerTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateSoundSpammerTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateAngryPlanesTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateFriendlyOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateDropManagerTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateForcefieldTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateWeaponOptionsTick));
		ScriptManager::Initialize(std::make_unique<Script>(&IterateVehicleOptionsTick));

		g_NetworkFeatures = std::make_unique<NetworkFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&NetworkFeaturesTick));

		g_WeaponFeatures = std::make_unique<WeaponFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&Weapons::RaycastTick));
		ScriptManager::Initialize(std::make_unique<Script>(&WeaponFeaturesTick));

		g_VehicleFeatures = std::make_unique<VehicleFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&VehicleFeaturesTick));

		g_MiscFeatures = std::make_unique<MiscFeatures>();
		ScriptManager::Initialize(std::make_unique<Script>(&MiscFeaturesTick));
	}

	void FeatureManager::Destroy()
	{
		g_MiscFeatures.reset();
		g_VehicleFeatures.reset();
		g_WeaponFeatures.reset();
		g_NetworkFeatures.reset();
		g_PlayersFeatures.reset();
		g_SelfFeatures.reset();

		g_ProtectionScript.reset();
	}
}