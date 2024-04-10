#pragma once 
#include "../Common.hpp"
#include "../Utility/Lists.hpp"
#include "../GTA/Include.hpp"

namespace Sentinel
{
	class MiscFeatures
	{
	public:
		void Tick()
		{
			AntiCrashCamera(m_AntiCrashCamera);
			UpdateSkyVFX();
			EnableFogOnMap(m_EnableFogOnMap);
			EnableWaterOnMap(m_EnableWaterOnMap);
			AerialCamera(m_AerialCamera);
			EnableFieldOfView(m_EnableFieldOfView);
			EnableWorldLuminance(m_EnableWorldLuminance);
			EnableBlackHole(m_EnableBlackHole);
			FreezeTime(m_FreezeTime);
			SyncTime(m_SyncTime);
			DisablePhone(m_DisablePhone);
			DisablePhoneCalls(m_DisablePhoneCalls);
			DisableHUD(m_DisableHUD);
			DisableMiniMap(m_DisableMiniMap);
			DisableHelpMessages(m_DisableHelpMessages);
			DisableMapMessages(m_DisableMapMessages);
			DisableWorldBoundaries(m_DisableWorldBoundaries);
			DisableLoadingPrompts(m_DisableLoadingPrompts);
			DisableCinematicCamera(m_DisableCinematicCamera);
			DisableAFKCamera(m_DisableAFKCamera);
			DisableCutscenes(m_DisableCutscenes);
			DisableRadio(m_DisableRadio);
			RainbowPrecipitation(m_RainbowPrecipitation);
		}

		bool m_AntiCrashCamera = false;
		const char* const m_SkyVFXQuadrant[11]
		{
			"East Azimuth",
			"West Azimuth",
			"Azimuth Transition",
			"Zenith",
			"Zenith Transition",
			"Cloud Mid",
			"Cloud Base",
			"Sun Color",
			"Sun HDR Color",
			"Sun Disc HDR Color",
			"All Quadrants"
		};
		int m_SkyVFXQuadrantPos = 0;
		float m_SkyVFXIntensity[sizeof(m_SkyVFXQuadrant)];
		float m_SkyVFXColor[sizeof(m_SkyVFXQuadrant)][3];
		bool m_EnableSkyVFX[sizeof(m_SkyVFXQuadrant)];
		bool m_RainbowSkyVFX[sizeof(m_SkyVFXQuadrant)];
		float m_RainIntensity = 1.f;
		float m_RainIntensityCache = 1.f;
		float m_WaveIntensity = 1.f;
		float m_WaveIntensityCache = 1.f;
		float m_WindSpeed = 1.f;
		float m_WindSpeedCache = 1.f;
		float m_WindDirection = 1.f;
		float m_WindDirectionCache = 1.f;
		float m_FogOnMap = 1.f;
		float m_FogOnMapCache = 1.f;
		bool m_EnableFogOnMap = false;
		float m_WaterOnMap = 1.f;
		float m_WaterOnMapCache = 1.f;
		bool m_EnableWaterOnMap = false;
		int m_AerialCamerDistance = 100;
		bool m_AerialCamera = false;
		int m_FieldOfView = 0.01f;
		int m_FieldOfViewCache = 0.f;
		bool m_EnableFieldOfView = false;
		float m_WorldLuminanceRange = 100.f;
		float m_WorldLuminanceIntensity = 100.f;
		float m_WorldLuminanceColor[3];
		bool m_WorldLuminanceRainbow = false;
		bool m_EnableWorldLuminance = false;
		const char* const m_WeatherTypes[15]
		{
			"Extra Sunny",
			"Clear",
			"Clouds",
			"Smog",
			"Foggy",
			"Overcast",
			"Rain",
			"Thunder",
			"Clearing",
			"Neutral",
			"Snow",
			"Blizzard",
			"Snow Light",
			"Xmas",
			"Halloween"
		};
		int m_WeatherTypesPos = 0;
		const char* const m_CloudTypes[20]
		{
			"Cloudy",
			"Rain",
			"Horizon",
			"Horizon Band 1",
			"Horizon Band 2",
			"Horizon Band 3",
			"Puffs",
			"Wispy",
			"Stormy",
			"Clear",
			"Snowy",
			"Contrails",
			"Altostratus",
			"Nimbus",
			"Cirrus",
			"Cirrocumulus",
			"Stratocumulus",
			"Stripey",
			"Horsey",
			"Shower"
		};
		int m_CloudTypesPos = 0;
		float m_BlackHoleX = 1.f;
		float m_BlackHoleY = 1.f;
		float m_BlackHoleZ = 1.f;
		bool m_BlackHolePeds = true;
		bool m_BlackHoleVehicles = true;
		bool m_BlackHoleObjects = true;
		bool m_EnableBlackHole = false;
		int m_TimeManagerHour = 0;
		int m_TimeManagerMinute = 0;
		int m_TimeManagerSecond = 0;
		bool m_FreezeTime = false;
		bool m_SyncTime = false;
		const char* const m_HUDColors[224]
		{
			"Pure White",
			"White",
			"Black",
			"Grey",
			"Grey Light",
			"Grey Dark",
			"Red",
			"Red Light",
			"Red Dark",
			"Blue",
			"Blue Light",
			"Blue Dark",
			"Yellow",
			"Yellow Light",
			"Yellow Dark",
			"Orange",
			"Orange Light",
			"Orange Dark",
			"Green",
			"Green Light",
			"Green Dark",
			"Purple",
			"Purple Light",
			"Purple Dark",
			"Pink",
			"Radar Health",
			"Radar Armour",
			"Radar Damage",
			"Player 1",
			"Player 2",
			"Player 3",
			"Player 4",
			"Player 5",
			"Player 6",
			"Player 7",
			"Player 8",
			"Player 9",
			"Player 10",
			"Player 11",
			"Player 12",
			"Player 13",
			"Player 14",
			"Player 15",
			"Player 16",
			"Player 17",
			"Player 18",
			"Player 19",
			"Player 20",
			"Player 21",
			"Player 22",
			"Player 23",
			"Player 24",
			"Player 25",
			"Player 26",
			"Player 27",
			"Player 28",
			"Player 29",
			"Player 30",
			"Player 31",
			"Player 32",
			"Simple Blip Default",
			"Menu Blue",
			"Menu Grey Light",
			"Menu Blue Extra Dark",
			"Menu Yellow",
			"Menu Yellow Dark",
			"Menu Green",
			"Menu Grey",
			"Menu Grey Dark",
			"Menu Highlight",
			"Menu Standard",
			"Menu Dimmed",
			"Menu Extra Dimmed",
			"Brief Title",
			"Mid Grey MP",
			"Player 1 Dark",
			"Player 2 Dark",
			"Player 3 Dark",
			"Player 4 Dark",
			"Player 5 Dark",
			"Player 6 Dark",
			"Player 7 Dark",
			"Player 8 Dark",
			"Player 9 Dark",
			"Player 10 Dark",
			"Player 11 Dark",
			"Player 12 Dark",
			"Player 13 Dark",
			"Player 14 Dark",
			"Player 15 Dark",
			"Player 16 Dark",
			"Player 17 Dark",
			"Player 18 Dark",
			"Player 19 Dark",
			"Player 20 Dark",
			"Player 21 Dark",
			"Player 22 Dark",
			"Player 23 Dark",
			"Player 24 Dark",
			"Player 25 Dark",
			"Player 26 Dark",
			"Player 27 Dark",
			"Player 28 Dark",
			"Player 29 Dark",
			"Player 30 Dark",
			"Player 31 Dark",
			"Player 32 Dark",
			"Bronze",
			"Silver",
			"Gold",
			"Platinum",
			"Gang 1",
			"Gang 2",
			"Gang 3",
			"Gang 4",
			"Same Crew",
			"Freemode",
			"Pause BG",
			"Friendly",
			"Enemy",
			"Location",
			"Pickup",
			"Pause Singleplayer",
			"Freemode Dark",
			"Inactive Mission",
			"Damage",
			"Pink Light",
			"PM Item Highlight",
			"Script Variable",
			"Yoga",
			"Tennis",
			"Golf",
			"Shooting Range",
			"Flight School",
			"North Blue",
			"Social Club",
			"Platform Blue",
			"Platform Green",
			"Platform Grey",
			"Facebook Blue",
			"In-Game BG",
			"Darts",
			"Waypoint",
			"Michael",
			"Franklin",
			"Trevor",
			"Golf P1",
			"Golf P2",
			"Golf P3",
			"Golf P4",
			"Waypoint Light",
			"Waypoint Dark",
			"panel Light",
			"Michael Dark",
			"Franklin Dark",
			"Trevor Dark",
			"Objective Route",
			"Pause Map Tint",
			"Pause Deselect",
			"PM Weapons Purchasable",
			"PM Weapons Locked",
			"End Screen BG",
			"Chop",
			"Pausemap Tint Half",
			"North Blue Official",
			"Script Variable 2",
			"H",
			"H Dark",
			"T",
			"T Dark",
			"HS Hard",
			"Controller Michael",
			"Controller Franklin",
			"Controller Trevor",
			"Controller Chop",
			"Video Editor Video",
			"Video Editor Audio",
			"Video Editor Text",
			"HB Blue",
			"HB Yellow",
			"Video Editor Score",
			"Video Editor Audio Fadeout",
			"Video Editor Text Fadeout",
			"Video Editor Score Fadeout",
			"Heist Background",
			"Video Editor Ambient",
			"Video Editor Ambient Fadeout",
			"GB",
			"G",
			"B",
			"Low Flow",
			"Low Flow Dark",
			"G1",
			"G2",
			"G3",
			"G4",
			"G5",
			"G6",
			"G7",
			"G8",
			"G9",
			"G10",
			"G11",
			"G12",
			"G13",
			"G14",
			"G15",
			"Adversary",
			"Degen Red",
			"Degen Yellow",
			"Degen Green",
			"Degen Cyan",
			"Degen Blue",
			"Degen Magenta",
			"Stunt 1",
			"Stunt 2",
			"Special Race Series",
			"Special Race Series Dark",
			"CS",
			"CS Dark",
			"Tech Green",
			"Tech Green Dark",
			"Tech Red",
			"Tech Green Very Dark"
		};
		int m_HUDColorsPos = 0;
		float m_ModifyHUDColor[sizeof(m_HUDColors)][3];
		bool m_DisableAntiAfkKick = false;
		bool m_DisablePopulationLoadBalancing = false;
		bool m_DisableUnderneathMapDeath = true;
		bool m_DisablePhone = false;
		bool m_DisablePhoneCalls = false;
		bool m_DisableHUD = false;
		bool m_DisableMiniMap = false;
		bool m_DisableHelpMessages = false;
		bool m_DisableMapMessages = false;
		bool m_DisableWorldBoundaries = false;
		bool m_DisableLoadingPrompts = false;
		bool m_DisableCinematicCamera = false;
		bool m_DisableAFKCamera = false;
		bool m_DisableCutscenes = false;
		bool m_DisableRadio = false;
		bool m_DisablePoliceAutomobile = false;
		bool m_DisablePoliceHelicopter = false;
		bool m_DisablePoliceRiders = false;
		bool m_DisablePoliceVehicleRequest = false;
		bool m_DisablePoliceRoadblock = false;
		bool m_DisablePoliceBoat = false;
		bool m_DisablSwatAutomobile = false;
		bool m_DisableSwatHelicopter = false;
		bool m_DisableFireDepartment = false;
		bool m_DisableAmbulanceDepartment = false;
		bool m_DisableGangs = false;
		bool m_DisableArmyVehicles = false;
		bool m_DisableBikers = false;
		std::vector<Shader> m_NewShaders;
		std::vector<CWeatherVfx> m_OriginalShaders;
		const char* const m_PrecipitationTypes[10]
		{
			  "Rainstorm Drop", 
			  "Thunder Drop", 
			  "Snow Light Drop", 
			  "Snow Heavy Drop", 
			  "Blizzard Drop", 
			  "Underwater Drop", 
			  "Lake Fog Drop", 
			  "Woodland Pollen Drop", 
			  "Woodland Firefly Drop", 
			  "Wetland Flies Drop"
		};
		int m_PrecipitationTypesPos = 0;
		bool m_RainbowPrecipitation[sizeof(m_PrecipitationTypes)];
		const char* const m_LiquidVFXTypes[5]
		{
			"Puddle",
			"Blood",
			"Oil",
			"Petrol",
			"Mud"
		};
		int m_LiquidVFXTypesPos = 0;
		float m_LiquidVFXColor[sizeof(m_LiquidVFXTypes)][3];
	private:
		void AntiCrashCamera(bool enable);
		void UpdateSkyVFX();
		void EnableFogOnMap(bool enable);
		void EnableWaterOnMap(bool enable);
		void AerialCamera(bool enable);
		void EnableFieldOfView(bool enable);
		void EnableWorldLuminance(bool enable);
		void EnableBlackHole(bool enable);
		void FreezeTime(bool enable);
		void SyncTime(bool enable);
		void DisablePhone(bool enable);
		void DisablePhoneCalls(bool enable);
		void DisableHUD(bool enable);
		void DisableMiniMap(bool enable);
		void DisableHelpMessages(bool enable);
		void DisableMapMessages(bool enable);
		void DisableWorldBoundaries(bool enable);
		void DisableLoadingPrompts(bool enable);
		void DisableCinematicCamera(bool enable);
		void DisableAFKCamera(bool enable);
		void DisableCutscenes(bool enable);
		void DisableRadio(bool enable);
		void RainbowPrecipitation(bool enable);
	};

	inline std::unique_ptr<MiscFeatures> g_MiscFeatures;
}