#pragma once 
#include "../Common.hpp"
#include "../Utility/Lists.hpp"

namespace Sentinel
{
	class VehicleFeatures
	{
	public:
		void Tick()
		{
			GodMode(m_GodMode);
			RenderBurnt(m_RenderBurnt);
			AutoRepair(m_AutoRepair);
			AutoWash(m_AutoWash);
			AutoFlip(m_AutoFlip);
			AntiLockon(m_AntiLockon);
			AntiStickyBomb(m_AntiStickyBomb);
			EngineAlwaysOn(m_EngineAlwaysOn);
			DisableFullCollision(m_DisableFullCollision);
			CollisionManager();
			NoClip(m_NoClip);
			VehicleFly(m_VehicleFly);
			Hydraulics(m_Hydraulics);
			DriftMode(m_DriftMode);
			TumbleDryer(m_TumbleDryer);
			StickyTires(m_StickyTires);
			SlippyWheels(m_SlippyWheels);
			NoGravity(m_NoGravity);
			DriveOnWater(m_DriveOnWater);
			DriveThroughWalls(m_DriveThroughWalls);
			DriveOnAir(m_DriveOnAir);
			BypassMaxSpeed(m_BypassMaxSpeed);
			NoTurbulence(m_NoTurbulence);
			HandBrake(m_HandBrake);
			InstantlyEnter(m_InstantlyEnter);
			InstantlyExit(m_InstantlyExit);
			EnableParticleTrail(m_EnableParticleTrail);
			EnableBoost(m_EnableBoost);
			EnableVehicleWeapon(m_EnableVehicleWeapon);
			AcrobaticOnHorn(m_AcrobaticOnHorn);
			InfiniteBoost(m_InfiniteBoost);
			AllowBoost(m_AllowBoost);
			AllowJumping(m_AllowJumping);
			AllowParachute(m_AllowParachute);
			InfiniteMissiles(m_InfiniteMissiles);
			InfiniteBombs(m_InfiniteBombs);
			InfiniteCounterMeasures(m_InfiniteCounterMeasures);
			IndicatorLights(m_IndicatorLights);
			MuteSiren(m_MuteSiren);
			PlateSpeedo(m_PlateSpeedo);
			ScreenSpeedo(m_ScreenSpeedo);
			EnableVehicleAlpha(m_EnableVehicleAlpha);
			VehicleInvisibility(m_VehicleInvisibility);
			EnableRainbowPaint(m_EnableRainbowPaint);
			EnableRainbowTireSmoke(m_EnableRainbowTireSmoke);
			EnableRainbowNeons(m_EnableRainbowNeons);
			RainbowHeadlights(m_RainbowHeadlights);
			RandomMods(m_RandomMods);
			RandomAcrobatics(m_RandomAcrobatics);
			EnableTireWidth(m_EnableTireWidth);
			EnableTireHeight(m_EnableTireHeight);
			EnableTireScale(m_EnableTireScale);
			EnableTireTracks(m_EnableTireTracks);
		}

		bool m_GodMode = false;
		bool m_RenderBurnt = false;
		bool m_AutoRepair = false;
		bool m_AutoWash = false;
		bool m_AutoFlip = false;
		bool m_AntiLockon = false;
		bool m_AntiStickyBomb = false;
		bool m_EngineAlwaysOn = false;
		bool m_DisableFullCollision = false;
		bool m_DisableCollisionWithVehicles = false;
		bool m_DisableCollisionWithPeds = false;
		bool m_DisableCollisionWithObjects = false;
		bool m_NoClip = false;
		float m_NoClipSpeed = 1.f;
		bool m_VehicleFly = false;
		bool m_Hydraulics = false;
		bool m_DriftMode = false;
		bool m_TumbleDryer = false;
		bool m_StickyTires = false;
		bool m_SlippyWheels = false;
		bool m_NoGravity = false;
		bool m_SuperGrip = false;
		bool m_DriveOnWater = false;
		bool m_DriveThroughWalls = false;
		bool m_DriveOnAir = false;
		bool m_BypassMaxSpeed = false;
		bool m_NoTurbulence = false;
		bool m_HandBrake = false;
		bool m_InstantlyEnter = false;
		bool m_InstantlyExit = false;
		const char* const m_VehicleParticles[7]
		{
			"RPG Muzzle",
			"Alien Teleport",
			"Alien Disintegrate",
			"Clown Death",
			"Clown Appears",
			"Electric Sphere",
			"Blue Spark"
		};
		int m_VehicleParticlesPos = 0;
		bool m_ActiveVehicleBones[std::size(Lists::g_VehicleBones)];
		float m_ParticleTrailScale = 0.1f;
		bool m_ParticleTrailPlayStationary = true;
		bool m_EnableParticleTrail = false;
		int m_BoostSpeed = 50;
		bool m_BoostEffect = true;
		bool m_BoostSound = true;
		bool m_BoostForwards = true;
		bool m_BoostBackwards = false;
		bool m_EnableBoost = false;
		const char* const m_VehicleWeaponBullet[14]
		{
			"Firework",
			"Grenade",
			"Molotov",
			"Vehicle Rocket",
			"RPG",
			"Tank Rocket",
			"Red Laser",
			"Green Laser",
			"Ball",
			"Snowball",
			"Smoke Grenade",
			"Flare",
			"Sticky Bomb",
			"Proximity Mine"
		};
		int m_VehicleWeaponBulletPos = 0;
		const char* const m_VehicleWeaponDirection[3]
		{
			"Point",
			"Cross",
			"Straight"
		};
		int m_VehicleWeaponDirectionPos = 0;
		float m_VehicleWeaponSpeed = 100.f;
		bool m_VehicleWeaponAudible = true;
		bool m_VehicleWeaponVisible = true;
		bool m_ViewVehicleWeaponDirection = true;
		bool m_EnableVehicleWeapon = false;
		const char* const m_VehicleAcrobatics[10]
		{
			"Wheelie",
			"Kickflip",
			"Double Kickflip",
			"Quadruple Kickflip",
			"Leap",
			"Slingshot",
			"Backflip",
			"Frontflip",
			"Right Jump",
			"Left Jump"
		};
		int m_VehicleAcrobaticsPos = 0;
		bool m_AcrobaticOnHorn = false;
		const char* const m_VehicleRampTypes[3]
		{
			"Small",
			"Medium",
			"Large"
		};
		int m_VehicleRampTypesPos = 0;
		const char* const m_VehicleRampLocation[4]
		{
			"Front",
			"Rear",
			"Left",
			"Right"
		};
		int m_VehicleRampLocationPos = 0;
		bool m_TransparentRamps = false;
		bool m_InfiniteBoost = false;
		bool m_AllowBoost = false;
		bool m_AllowJumping = false;
		bool m_AllowParachute = false;
		bool m_InfiniteMissiles = false;
		bool m_InfiniteBombs = false;
		bool m_InfiniteCounterMeasures = false;
		bool m_IndicatorLights = false;
		bool m_MuteSiren = false;
		const char* const m_SpeedoMeterMetric[2]
		{
			"MPH",
			"KMH"
		};
		int m_SpeedoMeterMetricPos = 0;
		bool m_PlateSpeedo = false;
		bool m_ScreenSpeedo = false;
		const char* const m_VehicleAlphaLevel[6]
		{
			"0%",
			"20%",
			"40%",
			"60%",
			"80%",
			"100%"
		};
		int m_VehicleAlphaLevelPos = 5;
		bool m_EnableVehicleAlpha = false;
		bool m_VehicleInvisibility = false;
		const char* const m_RainbowMode[2]
		{
			"Spastic",
			"Smooth"
		};
		int m_RainbowPaintModePos = 0;
		bool m_EnableRainbowPaint = false;
		int m_RainbowTireSmokeModePos = 0;
		bool m_EnableRainbowTireSmoke = false;
		int m_RainbowNeonsModePos = 0;
		bool m_EnableRainbowNeons = false;
		bool m_RainbowHeadlights = false;
		bool m_RandomMods = false;
		bool m_RandomAcrobatics = false;
		float m_TireWidth = 1.f;
		bool m_EnableTireWidth = false;
		float m_TireHeight = 1.f;
		bool m_EnableTireHeight = false;
		float m_TireScale = 1.f;
		bool m_EnableTireScale = false;
		const char* const m_EngineSounds[9]
		{
			"Electric Engine",
			"Tank Engine",
			"Super Engine",
			"Muscle Engine",
			"SUV Engine",
			"Bike Engine",
			"Truck Engine",
			"Off-Road Engine",
			"Formula 1 Engine"
		};
		int m_EngineSoundsPos = 0;
		const char* const m_VehicleWindows[8]
		{
			"Front Left",
			"Front Right",
			"Rear Left",
			"Rear Right",
			"Front Windscreen",
			"Front Windscreen",
			"Mid Left",
			"Mid Right"
		};
		int m_VehicleWindowsPos = 0;
		const char* const m_VehicleSeats[4] // You can go up to 16, but I ain't doing all of those cause it's pretty useless
		{
			"Driver",
			"Passenger",
			"Rear Left",
			"Rear Right"
		};
		int m_VehicleSeatsPos = 0;
		float m_TireTracksColor[3];
		bool m_TireTracksRainbow = false;
		bool m_EnableTireTracks = false;
	public:
		std::int32_t m_PreviousVehicleHash{};

		std::vector<std::pair<std::int32_t, float>> m_OriginalTireWidthValues{};
		bool IsTireWidthValueChanged(std::int32_t hash)
		{
			return std::find_if(m_OriginalTireWidthValues.begin(), m_OriginalTireWidthValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalTireWidthValues.end();
		}

		float GetOriginalTireWidthValue(std::int32_t hash)
		{
			return std::find_if(m_OriginalTireWidthValues.begin(), m_OriginalTireWidthValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetTireWidthValue(std::int32_t hash)
		{
			return m_EnableTireWidth
				? m_TireWidth
				: GetOriginalTireWidthValue(hash);
		}

		std::vector<std::pair<std::int32_t, float>> m_OriginalTireHeightValues{};
		bool IsTireHeightValueChanged(std::int32_t hash)
		{
			return std::find_if(m_OriginalTireHeightValues.begin(), m_OriginalTireHeightValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalTireHeightValues.end();
		}

		float GetOriginalTireHeightValue(std::int32_t hash)
		{
			return std::find_if(m_OriginalTireHeightValues.begin(), m_OriginalTireHeightValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetTireHeightValue(std::int32_t hash)
		{
			return m_EnableTireHeight
				? m_TireHeight
				: GetOriginalTireHeightValue(hash);
		}

		Vector2 GetTireScaleValue(std::int32_t hash)
		{
			return m_EnableTireScale
				? Vector2{ m_TireScale, m_TireScale }
				: Vector2{ GetOriginalTireWidthValue(hash), GetOriginalTireHeightValue(hash) };
		}
	private:
		void GodMode(bool enable);
		void RenderBurnt(bool enable);
		void AutoRepair(bool enable);
		void AutoWash(bool enable);
		void AutoFlip(bool enable);
		void AntiLockon(bool enable);
		void AntiStickyBomb(bool enable);
		void EngineAlwaysOn(bool enable);
		void DisableFullCollision(bool enable);
		void CollisionManager();
		void NoClip(bool enable);
		void VehicleFly(bool enable);
		void Hydraulics(bool enable);
		void DriftMode(bool enable);
		void TumbleDryer(bool enable);
		void StickyTires(bool enable);
		void SlippyWheels(bool enable);
		void NoGravity(bool enable);
		void DriveOnWater(bool enable);
		void DriveThroughWalls(bool enable);
		void DriveOnAir(bool enable);
		void BypassMaxSpeed(bool enable);
		void NoTurbulence(bool enable);
		void HandBrake(bool enable);
		void InstantlyEnter(bool enable);
		void InstantlyExit(bool enable);
		void EnableParticleTrail(bool enable);
		void EnableBoost(bool enable);
		void EnableVehicleWeapon(bool enable);
		void AcrobaticOnHorn(bool enable);
		void InfiniteBoost(bool enable);
		void AllowBoost(bool enable);
		void AllowJumping(bool enable);
		void AllowParachute(bool enable);
		void InfiniteMissiles(bool enable);
		void InfiniteBombs(bool enable);
		void InfiniteCounterMeasures(bool enable);
		void IndicatorLights(bool enable);
		void MuteSiren(bool enable);
		void PlateSpeedo(bool enable);
		void ScreenSpeedo(bool enable);
		void EnableVehicleAlpha(bool enable);
		void VehicleInvisibility(bool enable);
		void EnableRainbowPaint(bool enable);
		void EnableRainbowTireSmoke(bool enable);
		void EnableRainbowNeons(bool enable);
		void RainbowHeadlights(bool enable);
		void RandomMods(bool enable);
		void RandomAcrobatics(bool enable);
		void EnableTireWidth(bool enable);
		void EnableTireHeight(bool enable);
		void EnableTireScale(bool enable);
		void EnableTireTracks(bool enable);
	};

	inline std::unique_ptr<VehicleFeatures> g_VehicleFeatures;
}