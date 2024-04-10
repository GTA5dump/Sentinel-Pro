#include "Misc.hpp"
#include "../GTA/GTA.hpp"
#include "../GTA/Kicks.hpp"
#include "../GTA/enums.hpp"
#include "../Pointers/Pointers.hpp"
#include "../GTA/Invoker/Natives.hpp"
#include "../GTA/Script/ScriptGlobal.hpp"
#include "../GTA/Script/ScriptLocal.hpp"
#include "../Utility/Timer.hpp"
#include "../Utility/Joaat.hpp"
#include "../Utility/Math.hpp"
#include "../GUI/GUI.hpp"
#include "../GUI/Overlays/Overlays.hpp"
#include "../Utility/All.hpp"

namespace Sentinel
{
	bool g_LastAntiCrashCamera = false;
	void MiscFeatures::AntiCrashCamera(bool enable)
	{
		static Cam AntiCrashCamHandle;
		if (enable)
		{
			if (!CAM::DOES_CAM_EXIST(AntiCrashCamHandle)) {
				AntiCrashCamHandle = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_CAMERA", 9999.f, 9999.f, 9999.f, 9999.f, 9999.f, 9999.f, 9999.f, true, 1);
				CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 1, FALSE, FALSE, NULL);
				CAM::SET_CAM_ACTIVE(AntiCrashCamHandle, TRUE);
				PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), FALSE, 0);
			}
		}
		else if (enable != g_LastAntiCrashCamera)
		{
			if (CAM::DOES_CAM_EXIST(AntiCrashCamHandle)) {
				CAM::SET_CAM_ACTIVE(AntiCrashCamHandle, false);
				CAM::RENDER_SCRIPT_CAMS(FALSE, TRUE, 1000, TRUE, FALSE, NULL);
				CAM::DESTROY_CAM(AntiCrashCamHandle, FALSE);
				PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), TRUE, 0);
			}
		}
		g_LastAntiCrashCamera = enable;
	}

	void MiscFeatures::UpdateSkyVFX()
	{
		for (int Iterator = 0; Iterator < sizeof(g_MiscFeatures->m_SkyVFXQuadrant); Iterator++)
		{
			if (m_EnableSkyVFX[Iterator])
			{
				if (!gResetVFX) // Apply patches once when any of the VFX Modifiers is enabled
				{
					std::fill_n(Pointers::pEastAzimuthPatch, 4, '\x90');
					std::fill_n(Pointers::pWestAzimuthPatch, 4, '\x90');
					std::fill_n(Pointers::pAzimuthTransitionPatch, 7, '\x90');
					std::fill_n(Pointers::pZenithPatch, 7, '\x90');
					std::fill_n(Pointers::pZenithTransitionPatch, 7, '\x90');
					std::fill_n(Pointers::pCloudMidPatch, 7, '\x90');
					std::fill_n(Pointers::pCloudBasePatch, 7, '\x90');
					gResetVFX = true;
				}

				if (m_RainbowSkyVFX[Iterator])
				{
					m_SkyVFXColor[Iterator][0] = System::g_SlowRainbowColor.r / 255.f;
					m_SkyVFXColor[Iterator][1] = System::g_SlowRainbowColor.g / 255.f;
					m_SkyVFXColor[Iterator][2] = System::g_SlowRainbowColor.b / 255.f;
				}

				switch (Iterator)
				{
				case 0:
					*(float*)Pointers::pEastAzimuthColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pEastAzimuthColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pEastAzimuthColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 1:
					*(float*)Pointers::pWestAzimuthColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pWestAzimuthColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pWestAzimuthColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 2:
					*(float*)Pointers::pAzimuthTransitionColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pAzimuthTransitionColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pAzimuthTransitionColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 3:
					*(float*)Pointers::pZenithColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 4:
					*(float*)Pointers::pZenithTransitionColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithTransitionColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithTransitionColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 5:
					*(float*)Pointers::pCloudMidColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudMidColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudMidColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 6:
					*(float*)Pointers::pCloudBaseColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudBaseColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudBaseColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 7:
					*(float*)Pointers::pSunColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 8:
					*(float*)Pointers::pSunHDRColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunHDRColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunHDRColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 9: 
					*(float*)Pointers::pSunDiscHDRColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunDiscHDRColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunDiscHDRColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				case 10:
					*(float*)Pointers::pEastAzimuthColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pEastAzimuthColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pEastAzimuthColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pWestAzimuthColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pWestAzimuthColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pWestAzimuthColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pAzimuthTransitionColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pAzimuthTransitionColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pAzimuthTransitionColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pZenithColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pZenithTransitionColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithTransitionColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pZenithTransitionColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pCloudMidColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudMidColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudMidColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pCloudBaseColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudBaseColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pCloudBaseColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pSunColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pSunHDRColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunHDRColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunHDRColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];

					*(float*)Pointers::pSunDiscHDRColor = m_SkyVFXColor[Iterator][0] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunDiscHDRColor, 0x4) = m_SkyVFXColor[Iterator][1] * m_SkyVFXIntensity[Iterator];
					*(float*)Pointers::Add(Pointers::pSunDiscHDRColor, 0x8) = m_SkyVFXColor[Iterator][2] * m_SkyVFXIntensity[Iterator];
					break;
				}
			}
		}
	}

	bool g_LastAerialCamera = false;
	void MiscFeatures::AerialCamera(bool enable)
	{
		static Cam AerialCameraHandle = NULL;
		if (enable)
		{
			if (CAM::DOES_CAM_EXIST(AerialCameraHandle))
			{
				auto Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.f, (float)(m_AerialCamerDistance / 2.f) * -1.f, (float)m_AerialCamerDistance);
				Coords.z = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE).z + m_AerialCamerDistance;
				CAM::SET_CAM_COORD(AerialCameraHandle, Coords.x, Coords.y, Coords.z);
				CAM::POINT_CAM_AT_ENTITY(AerialCameraHandle, PLAYER::PLAYER_PED_ID(), 0.f, 0.f, 0.f, TRUE);
			}
			else
			{
				AerialCameraHandle = CAM::CREATE_CAMERA(26379945, TRUE);
				CAM::SET_CAM_ACTIVE(AerialCameraHandle, TRUE);
				CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 1000, TRUE, FALSE, NULL);
			}

		}
		else if (enable != g_LastAerialCamera && CAM::DOES_CAM_EXIST(AerialCameraHandle))
		{
			CAM::DESTROY_CAM(AerialCameraHandle, TRUE);
			CAM::RENDER_SCRIPT_CAMS(FALSE, TRUE, 1000, TRUE, FALSE, NULL);
		}
		g_LastAerialCamera = enable;
	}

	bool g_LastFieldOfView = false;
	void MiscFeatures::EnableFieldOfView(bool enable)
	{
		if (enable) {
			DWORD64 Address = Pointers::GetCamera();
			std::uintptr_t Data = *(std::uintptr_t*)(Address + 0x10);
			*(float*)(Data + 0x24) = m_FieldOfView;
		}
		else if (enable != g_LastFieldOfView) {
			m_FieldOfView = m_FieldOfViewCache;
			DWORD64 Address = Pointers::GetCamera();
			std::uintptr_t Data = *(std::uintptr_t*)(Address + 0x10);
			*(float*)(Data + 0x24) = m_FieldOfViewCache;
		}
		g_LastFieldOfView = enable;
	}

	bool g_LastEnableFogOnMap = false;
	void MiscFeatures::EnableFogOnMap(bool enable)
	{
		if (enable)
			*(float*)Pointers::pFOGOnMap = m_FogOnMap;
		else if (enable != g_LastEnableFogOnMap) {
			m_FogOnMap = m_FogOnMapCache;
			*(float*)Pointers::pFOGOnMap = m_FogOnMapCache;
		}

		g_LastEnableFogOnMap = enable;
	}

	bool g_LastEnableWaterOnMap = false;
	void MiscFeatures::EnableWaterOnMap(bool enable)
	{
		if (enable)
			*(float*)Pointers::pWaterOnMap = m_WaterOnMap;
		else if (enable != g_LastEnableWaterOnMap) {
			m_WaterOnMap = m_WaterOnMapCache;
			*(float*)Pointers::pWaterOnMap = m_WaterOnMapCache;
		}

		g_LastEnableWaterOnMap = enable;
	}

	void MiscFeatures::EnableWorldLuminance(bool enable)
	{
		if (enable)
		{
			const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
			GRAPHICS::DRAW_LIGHT_WITH_RANGE(Coords.x, Coords.y, Coords.z, m_WorldLuminanceColor[0] * 255.f, m_WorldLuminanceColor[1] * 255.f, m_WorldLuminanceColor[2] * 255.f, m_WorldLuminanceRange, m_WorldLuminanceIntensity);

			if (m_WorldLuminanceRainbow)
			{	
				m_WorldLuminanceColor[0] = System::g_SlowRainbowColor.r / 255.f;
				m_WorldLuminanceColor[1] = System::g_SlowRainbowColor.g / 255.f;
				m_WorldLuminanceColor[2] = System::g_SlowRainbowColor.b / 255.f;
			}
		}
	}

	void MiscFeatures::EnableBlackHole(bool enable)
	{
		if (enable)
		{
			GRAPHICS::DRAW_MARKER_SPHERE(m_BlackHoleX, m_BlackHoleY, m_BlackHoleZ, 2.5f, 0, 0, 0, 0.8f);
			for (const auto Entity : Entities::GetEntities(m_BlackHoleVehicles, m_BlackHolePeds, m_BlackHoleObjects))
			{
				if (*Pointers::pIsSessionStarted)
					Entities::ChangeNetworkObjectOwner(Entity, Pointers::pGetNetPlayer(PLAYER::PLAYER_ID()));
				const auto Coords = ENTITY::GET_ENTITY_COORDS(Entity, FALSE);
				ENTITY::APPLY_FORCE_TO_ENTITY(Entity, 1, ((m_BlackHoleX - Coords.x) * 9.f), ((m_BlackHoleY - Coords.y) * 9.f), ((m_BlackHoleZ - Coords.z) * 9.f), 0.f,	0.f, 0.f, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE);
			}
		}
	}

	void MiscFeatures::FreezeTime(bool enable)
	{
		if (enable)
			CLOCK::PAUSE_CLOCK(TRUE);
	}

	void MiscFeatures::SyncTime(bool enable)
	{
		if (enable)
		{
			CLOCK::PAUSE_CLOCK(TRUE);
			std::int32_t Time[6];
			CLOCK::GET_LOCAL_TIME(&Time[0], &Time[1], &Time[2], &Time[3], &Time[4], &Time[5]);
			NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(Time[3], Time[4], Time[5]);
		}
	}

	void MiscFeatures::DisablePhone(bool enable)
	{
		if (enable)
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_PHONE, TRUE);
	}

	void MiscFeatures::DisablePhoneCalls(bool enable)
	{
		if (enable && AUDIO::IS_MOBILE_PHONE_CALL_ONGOING())
			AUDIO::STOP_SCRIPTED_CONVERSATION(FALSE);
	}

	void MiscFeatures::DisableHUD(bool enable)
	{
		if (enable)
			HUD::DISPLAY_HUD(FALSE);
	}

	void MiscFeatures::DisableMiniMap(bool enable)
	{
		if (enable)
			HUD::DISPLAY_RADAR(FALSE);
	}

	void MiscFeatures::DisableHelpMessages(bool enable)
	{
		if (enable)
			HUD::CLEAR_ALL_HELP_MESSAGES();
	}

	void MiscFeatures::DisableMapMessages(bool enable)
	{
		if (enable)
			HUD::THEFEED_HIDE_THIS_FRAME();
	}

	void MiscFeatures::DisableWorldBoundaries(bool enable)
	{
		if (enable)
		{
			PLAYER::EXTEND_WORLD_BOUNDARY_FOR_PLAYER(-9000.f, -11000.f, 30.f);
			PLAYER::EXTEND_WORLD_BOUNDARY_FOR_PLAYER(10000.f, 12000.f, 30.f);
		}
	}

	void MiscFeatures::DisableLoadingPrompts(bool enable)
	{
		if (enable && HUD::BUSYSPINNER_IS_ON())
			HUD::BUSYSPINNER_OFF();
	}

	void MiscFeatures::DisableCinematicCamera(bool enable)
	{
		if (enable)
			PAD::DISABLE_CONTROL_ACTION(0, INPUT_VEH_CIN_CAM, TRUE);
	}

	void MiscFeatures::DisableAFKCamera(bool enable)
	{
		if (enable)
		{
			CAM::INVALIDATE_IDLE_CAM();
			CAM::INVALIDATE_CINEMATIC_VEHICLE_IDLE_MODE();
		}
	}

	void MiscFeatures::DisableCutscenes(bool enable)
	{
		if (enable)
		{
			if (CUTSCENE::IS_CUTSCENE_ACTIVE() || CUTSCENE::IS_CUTSCENE_PLAYING())
				CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		}
	}

	bool g_LastDisableRadio = false;
	void MiscFeatures::DisableRadio(bool enable)
	{
		const auto Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE);
		if (enable)
			AUDIO::SET_VEHICLE_RADIO_ENABLED(Vehicle, FALSE);
		else if (enable != g_LastDisableRadio)
			AUDIO::SET_VEHICLE_RADIO_ENABLED(Vehicle, TRUE);

		g_LastDisableRadio = enable;
	}

	void MiscFeatures::RainbowPrecipitation(bool enable)
	{
		for (int Iterator = 0; Iterator < sizeof(m_PrecipitationTypes); Iterator++)
		{
			if (m_RainbowPrecipitation[Iterator])
			{
				g_MiscFeatures->m_NewShaders[Iterator].m_Drop->m_color.x = ((System::g_SlowRainbowColor.r / 255.f) * g_MiscFeatures->m_NewShaders[Iterator].m_Luminance);
				g_MiscFeatures->m_NewShaders[Iterator].m_Drop->m_color.y = ((System::g_SlowRainbowColor.g / 255.f) * g_MiscFeatures->m_NewShaders[Iterator].m_Luminance);
				g_MiscFeatures->m_NewShaders[Iterator].m_Drop->m_color.z = ((System::g_SlowRainbowColor.b / 255.f) * g_MiscFeatures->m_NewShaders[Iterator].m_Luminance);

				g_MiscFeatures->m_NewShaders[Iterator].m_Color[0] = (g_MiscFeatures->m_NewShaders[Iterator].m_Drop->m_color.x * g_MiscFeatures->m_NewShaders[Iterator].m_Luminance);
				g_MiscFeatures->m_NewShaders[Iterator].m_Color[1] = (g_MiscFeatures->m_NewShaders[Iterator].m_Drop->m_color.y * g_MiscFeatures->m_NewShaders[Iterator].m_Luminance);
				g_MiscFeatures->m_NewShaders[Iterator].m_Color[2] = (g_MiscFeatures->m_NewShaders[Iterator].m_Drop->m_color.z * g_MiscFeatures->m_NewShaders[Iterator].m_Luminance);
			}
		}
	}
}