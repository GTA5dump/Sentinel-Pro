#pragma once 
#include "../Common.hpp"

namespace Sentinel
{
	class WeaponFeatures
	{
	public:
		void Tick()
		{
			InfiniteAmmo(m_InfiniteAmmo);
			InfiniteClip(m_InfiniteClip);
			ExplosiveAmmo(m_ExplosiveAmmo);
			FireAmmo(m_FireAmmo);
			NoRecoil(m_NoRecoil);
			NoSpread(m_NoSpread);
			ShotGunMode(m_ShotGunMode);
			OneShotKill(m_OneShotKill);
			NoCooldown(m_NoCooldown);
			NoSpinUp(m_NoSpinUp);
			PortalGun(m_PortalGun);
			TeleportGun(m_TeleportGun);
			SkydiveGun(m_SkydiveGun);
			AirstrikeGun(m_AirstrikeGun);
			PushGun(m_PushGun);
			PullGun(m_PullGun);
			PedGun(m_PedGun);
			RapidFire(m_RapidFire);
			DeleteGun(m_DeleteGun);
			HijackGun(m_HijackGun);
			RopeGun(m_RopeGun);
			ValkyrieGun(m_ValkyrieGun);
			DeadEyes(m_DeadEyes);
			LaserSight(m_LaserSight);
			DefibrillatorGun(m_DefibrillatorGun);
			HashGun(m_HashGun);
			ShrinkGun(m_ShrinkGun);
			ClownGun(m_ClownGun);
			RepairGun(m_RepairGun);
			RecolorGun(m_RecolorGun);
			InstrumentOfDeath(m_InstrumentOfDeath);
			VortexGunEnable(m_VortexGunEnable);
			EnableTotemGun(m_EnableTotemGun);
			EnableBulletTracers(m_EnableBulletTracers);
			EnableGravityGun(m_EnableGravityGun);
			EnableBulletChanger(m_EnableBulletChanger);
			EnableImpactGun(m_EnableImpactGun);
			EnablePaintGun(m_EnablePaintGun);
			EnableParticleGun(m_EnableParticleGun);
			EnableEntityGun(m_EnableEntityGun);
			EnableWeaponWidth(m_EnableWeaponWidth);
			EnableWeaponHeight(m_EnableWeaponHeight);
			EnableWeaponScale(m_EnableWeaponScale);
			EnablePositionX(m_EnablePositionX);
			EnablePositionY(m_EnablePositionY);
			AllowWeaponsInVehicle(m_AllowWeaponsInVehicle);
			FriendlyFire(m_FriendlyFire);
			InstantLockOn(m_InstantLockOn);
			BypassC4Limit(m_BypassC4Limit);
			BypassFlareLimit(m_BypassFlareLimit);
			EnableTriggerBot(m_EnableTriggerBot);
			EnableAimBot(m_EnableAimBot);
			RainbowRegularTint(m_RainbowRegularTint);
			RainbowMk2Tint(m_RainbowMk2Tint);
			EnableCrosshair(m_EnableCrosshair);
		}

		bool m_InfiniteAmmo = false;
		bool m_InfiniteClip = false;
		bool m_ExplosiveAmmo = false;
		bool m_FireAmmo = false;
		bool m_NoRecoil = false;
		bool m_NoSpread = false;
		bool m_ShotGunMode = false;
		bool m_OneShotKill = false;
		bool m_NoCooldown = false;
		bool m_NoSpinUp = false;
		bool m_PortalGun = false;
		bool m_TeleportGun = false;
		bool m_SkydiveGun = false;
		bool m_AirstrikeGun = false;
		bool m_PushGun = false;
		bool m_PullGun = false;
		bool m_PedGun = false;
		bool m_RapidFire = false;
		bool m_DeleteGun = false;
		bool m_HijackGun = false;
		bool m_RopeGun = false;
		bool m_ValkyrieGun = false;
		bool m_DeadEyes = false;
		bool m_LaserSight = false;
		bool m_DefibrillatorGun = false;
		bool m_HashGun = false;
		bool m_ShrinkGun = false;
		bool m_ClownGun = false;
		bool m_RepairGun = false;
		bool m_RecolorGun = false;
		bool m_InstrumentOfDeath = false;
		bool m_VortexGunPeds = true;
		int m_VortexGunPedCapacity = 10;
		bool m_VortexGunVehicles = true;
		int m_VortexGunVehicleCapacity = 10;
		bool m_VortexGunObjects = true;
		int m_VortexGunObjectCapacity = 10;
		float m_VortexGunDistance = 20.f;
		float m_VortexGunForce = 500.f;
		bool m_VortexGunCooldown = false;
		bool m_VortexGunSphere = true;
		bool m_VortexGunEnable = false;
		float m_TotemGunDistance = 10.f;
		float m_TotemGunSpacing = 5.f;
		bool m_EnableTotemGun = false;
		int m_BulletTracerDuration = 5000;
		float m_BulletTracerColor[3];
		bool m_RainbowBulletTracers = false;
		bool m_EnableBulletTracers = false;
		float m_GravityGunDistance = 20.f;
		float m_GravityGunForce = 500.f;
		bool m_EnableGravityGun = false;
		const char* const m_BulletChangerType[14]
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
		int m_BulletChangerTypePos = 0;
		float m_BulletChangerSpeed = 100.f;
		bool m_BulletChangerAudible = true;
		bool m_BulletChangerVisible = true;
		bool m_EnableBulletChanger = false;
		const char* const m_ImpactGunType[84]
		{
			"Grenade",
			"Grenade Launcher",
			"Sticky Bomb",
			"Molotov",
			"Rocket",
			"Tank Shell",
			"Hi Octane",
			"Car",
			"Plane",
			"Petrol Pump",
			"Bike",
			"Water",
			"Flame",
			"Water Big",
			"Flame Big",
			"Boat",
			"Ship Destroy",
			"Truck",
			"Bullet",
			"Smoke Grenade Launcher",
			"Smoke Grenade",
			"Tear Gas",
			"Flare",
			"Gas Canister",
			"Extinguisher",
			"Unknown",
			"Train",
			"Barrel",
			"Propane",
			"Blimp",
			"Flame Explode",
			"Tanker",
			"Plane Rocket",
			"Vehicle Bullet",
			"Gas Tank",
			"Bird Crap",
			"Rail Gun",
			"Blimp 2",
			"Firework",
			"Snowball",
			"Proximity Mine",
			"Valkyrie Cannon",
			"Air Defence",
			"Pipebomb",
			"Vehicle Mine",
			"Explosive Ammo",
			"APC Shell",
			"Bomb Cluster",
			"Bomb Gas",
			"Bomb Incendiary",
			"Bomb Standard",
			"Torpedo",
			"Torpedo Underwater",
			"Bombushka Cannon",
			"Bomb Cluster Secondary",
			"Hunter Barrage",
			"Hunter Cannon",
			"Rogue Cannon",
			"Mine Underwater",
			"Orbital Cannon",
			"Bomb Standard Wide",
			"Explosive Ammo Shotgun",
			"Oppressor Mk2 Cannon",
			"Mortar Kinetic",
			"Vehicle Mine Kinetic",
			"Vehicle Mine EMP",
			"Vehicle Mine Spike",
			"Vehicle Mine Slick",
			"Vehicle Mine Tar",
			"Script Drone",
			"Raygun",
			"Buried Mine",
			"Script Missile",
			"RC Tank Rocket",
			"Bomb Water",
			"Bomb Water Secondary",
			"Unknown 2",
			"Unknown 3",
			"Flash Grenade",
			"Stun Grenade",
			"Unknown 4",
			"Large Script Missile",
			"Submarine",
			"EMP Launcher"
		};
		int m_ImpactGunTypePos = 0;
		float m_ImpactGunDamageScale = 9999.f;
		float m_ImpactGunCameraShake = 0.f;
		bool m_ImpactGunAudible = true;
		bool m_ImpactGunVisible = true;
		bool m_EnableImpactGun = false;
		float m_PaintGunColor[3];
		bool m_RainbowPaintGun = false;
		float m_PaintGunSize = 0.5f;
		float m_PaintGunIntensity = 1000.f;
		int m_PaintGunDuration = 5000;
		bool m_EnablePaintGun = false;
		const char* const m_ParticleGunEffect[20]
		{
			"Purple Cloud 1",
			"Purple Cloud 2",
			"Purple Cloud 3",
			"Firework Starburst",
			"Firework Fountain",
			"Firework Shotburst",
			"Xmas Firework Burst",
			"Teleport",
			"Disintegration",
			"Drip",
			"Flower Jet",
			"Flash",
			"Blue Flash",
			"Burnout",
			"Splash",
			"Blood",
			"Sparks",
			"Shock",
			"Money Cloud",
			"Tire Smoke"
		};
		int m_ParticleGunEffectPos = 0;
		float m_ParticleGunScale = 1.f;
		bool m_EnableParticleGun = false;
		const char* const m_EntityGunType[3]
		{
			"Object",
			"Vehicle",
			"Ped"
		};
		int m_EntityGunTypePos = 0;
		const char* const m_EntityGunObject[14]
		{
			"Meteor",
			"Stairs",
			"Egg",
			"Barbell",
			"Campfire",
			"Volleyball",
			"Dildo",
			"Guitar",
			"Gold",
			"Missile",
			"Toilet",
			"Money Bag",
			"Wheelchair",
			"Pumpkin"
		};
		int m_EntityGunObjectPos = 0;
		const char* const m_EntityGunVehicle[23]
		{
			"Jobuilt Hauler",
			"Weeny Issi",
			"Dinka Blista Kanjo",
			"Declasse Rhapsody",
			"Vapid Dominator",
			"Albany Hermes",
			"Vapid Clique",
			"Vapid Slamvan",
			"Space Dune",
			"Ocelot R88",
			"Maxwell Vagrant",
			"Annis Hellion",
			"BF Injection",
			"BF Bifta",
			"Nagasaki Hot Rod Blazer",
			"Vapid Peyote",
			"Vapid Hustler",
			"Vapid Imperator",
			"Vapid Ellie",
			"Speedophile Seashark",
			"Titan",
			"Truffade Adder",
			"Progen T20"
		};
		int m_EntityGunVehiclePos = 0;
		const char* const m_EntityGunPed[12]
		{
			"Whale",
			"Cow",
			"Seagull",
			"Topless Girl",
			"Bride",
			"Hooker",
			"Astronaut",
			"Human Statue",
			"Mime",
			"Jesus",
			"Pogo",
			"Zombie"
		};
		int m_EntityGunPedPos = 0;
		int m_EntityGunDelay = 100;
		float m_EntityGunForce = 900.f;
		bool m_EnableEntityGun = false;
		float m_WeaponWidth = 1.f;
		bool m_EnableWeaponWidth = false;
		float m_WeaponHeight = 1.f;
		bool m_EnableWeaponHeight = false;
		float m_WeaponScale = 1.f;
		bool m_EnableWeaponScale = false;
		float m_PositionX = 1.f;
		bool m_EnablePositionX = false;
		float m_PositionY = 1.f;
		bool m_EnablePositionY = false;
		bool m_InteriorWeapons = false;
		bool m_PassiveWeapons = false;
		bool m_AllowWeaponsInVehicle = false;
		bool m_FriendlyFire = false;
		bool m_InstantLockOn = false;
		bool m_BypassC4Limit = false;
		bool m_BypassFlareLimit = false;
		const char* const m_TriggerBotBone[7]
		{
			"Head",
			"Neck",
			"Core",
			"Left Hand",
			"Right Hand",
			"Left Foot",
			"Right Foot"
		};
		int m_TriggerBotBonePos = 0;
		bool m_TriggerBotPolice = true;
		bool m_TriggerBotCivilians = true;
		bool m_TriggerBotEnemies = true;
		bool m_TriggerBotPlayers = true;
		bool m_EnableTriggerBot = false;
		const char* const m_AimBotBone[7]
		{
			"Head",
			"Neck",
			"Core",
			"Left Hand",
			"Right Hand",
			"Left Foot",
			"Right Foot"
		};
		int m_AimBotBonePos = 0;
		float m_AimBotLegitSpeed = 2.f;
		float m_AimBotFOV = 90.f;
		float m_AimBotDistance = 200.f;
		bool m_AimBotPolice = true;
		bool m_AimBotCivilians = true;
		bool m_AimBotEnemies = true;
		bool m_AimBotPlayers = true;
		bool m_LegitBot = true;
		bool m_EnableAimBot = false;
		const char* const m_GunLockerCat[10]
		{
			"Melee Weapons",
			"Handguns",
			"Submachine Guns",
			"Shotguns",
			"Assault Rifles",
			"Light Machine Guns",
			"Sniper Rifles",
			"Heavy Weapons",
			"Throwables",
			"Miscellaneous Weapons"
		};
		int m_GunLockerCatPos = 0;
		const char* const m_MeleeWeapons[17]
		{
			"Antique Cavalry Dagger",
			"Baseball Bat",
			"Broken Bottle",
			"Crowbar",
			"Flashlight",
			"Golf Club",
			"Hammer",
			"Hatchet",
			"Brass Knuckles",
			"Knife",
			"Machete",
			"Switchblade",
			"Nightstick",
			"Pipe Wrench",
			"Battle Axe",
			"Pool Cue",
			"Stone Hatchet"
		};
		int m_MeleeWeaponsPos = 0;
		const char* const m_Handguns[19]
		{
			"Pistol",
			"Pistol Mk II",
			"Combat Pistol",
			"AP Pistol",
			"Stun Gun",
			"Pistol .50",
			"SNS Pistol",
			"SNS Pistol Mk II",
			"Heavy Pistol",
			"Vintage Pistol",
			"Flare Gun",
			"Marksman Pistol",
			"Heavy Revolver",
			"Heavy Revolver Mk II",
			"Double Action Revolver",
			"Up-n-Atomizer",
			"Ceramic Pistol",
			"Navy Revolver",
			"Perico Pistol"
		};
		int m_HandgunsPos = 0;
		const char* const m_SubmachineGuns[8]
		{
			"Micro SMG",
			"SMG",
			"SMG Mk II",
			"Assault SMG",
			"Combat PDW",
			"Machine Pistol",
			"Mini SMG",
			"Unholy Hellbringer"
		};
		int m_SubmachineGunsPos = 0;
		const char* const m_Shotguns[10]
		{
			"Pump Shotgun",
			"Pump Shotgun Mk II",
			"Sawed-Off Shotgun",
			"Assault Shotgun",
			"Bullpup Shotgun",
			"Musket",
			"Heavy Shotgun",
			"Double Barrel Shotgun",
			"Sweeper Shotgun",
			"Combat Shotgun"
		};
		int m_ShotgunsPos = 0;
		const char* const m_AssaultRifles[13]
		{
			"Assault Rifle",
			"Assault Rifle Mk II",
			"Carbine Rifle",
			"Carbine Rifle Mk II",
			"Advanced Rifle",
			"Special Carbine",
			"Special Carbine Mk II",
			"Bullpup Rifle",
			"Bullpup Rifle Mk II",
			"Compact Rifle",
			"Military Rifle",
			"Heavy Rifle",
			"Tactical Rifle"
		};
		int m_AssaultRiflesPos = 0;
		const char* const m_LightMachineGuns[4]
		{
			"MG",
			"Combat MG",
			"Combat MG Mk II",
			"Gusenberg Sweeper"
		};
		int m_LightMachineGunsPos = 0;
		const char* const m_SniperRifles[6]
		{
			"Sniper Rifle",
			"Heavy Sniper",
			"Heavy Sniper Mk II",
			"Marksman Rifle",
			"Marksman Rifle Mk II",
			"Precision Rifle"
		};
		int m_SniperRiflesPos = 0;
		const char* const m_HeavyWeapons[10]
		{
			"RPG",
			"Grenade Launcher",
			"Smoke Grenade Launcher",
			"Minigun",
			"Firework Launcher",
			"Railgun",
			"Homing Launcher",
			"Compact Grenade Launcher",
			"Widowmaker",
			"Compact EMP Launcher"
		};
		int m_HeavyWeaponsPos = 0;
		const char* const m_Throwables[10]
		{
			"Grenades",
			"BZ Gas",
			"Molotov Cocktails",
			"Sticky Bombs",
			"Proximity Mines",
			"Snowballs",
			"Pipe Bombs",
			"Baseballs",
			"Tear Gas",
			"Flares"
		};
		int m_ThrowablesPos = 0;
		const char* const m_MiscellaneousWeapons[5]
		{
			"Jerry Can",
			"Parachute",
			"Fire Extinguisher",
			"Hazardous Jerry Can",
			"Fertilizer Can"
		};
		int m_MiscellaneousWeaponsPos = 0;
		const char* const m_RegularTints[8]
		{
			"Default",
			"Green",
			"Gold",
			"Pink",
			"Army",
			"LSPD",
			"Orange",
			"Platinum"
		};
		int m_RegularTintsPos = 0;
		bool m_RainbowRegularTint = false;
		const char* const m_Mk2Tints[32]
		{
			"Classic Black",
			"Classic Gray",
			"Classic Two-Tone",
			"Classic White",
			"Classic Beige",
			"Classic Green",
			"Classic Blue",
			"Classic Earth",
			"Classic Brown & Black",
			"Contrast Red",
			"Contrast Blue",
			"Contrast Yellow",
			"Contrast Orange",
			"Bold Pink",
			"Bold Purple & Yellow",
			"Bold Orange",
			"Bold Green & Purple",
			"Bold Red Features",
			"Bold Green Features",
			"Bold Cyan Features",
			"Bold Yellow Features",
			"Bold Red & White",
			"Bold Blue & White",
			"Metallic Gold",
			"Metallic Platinum",
			"Metallic Gray & Lilac",
			"Metallic Purple & Lime",
			"Metallic Red",
			"Metallic Green",
			"Metallic Blue",
			"Metallic White & Aqua",
			"Metallic Red & Yellow"
		};
		int m_Mk2TintsPos = 0;
		bool m_RainbowMk2Tint = false;
		const char* const m_Crosshairs[11]
		{
			"Cross",
			"Target",
			"Arrow",
			"Holy Cross",
			"Reticle",
			"Reticle Zoomed",
			"Dot",
			"Plus",
			"Middle Finger",
			"Box",
			"Star"
		};
		int m_CrosshairPos = 0;
		float m_CrosshairColor[3];
		bool m_RainbowCrosshair = false;
		bool m_EnableCrosshair = false;
	private:
		void InfiniteAmmo(bool enable);
		void InfiniteClip(bool enable);
		void ExplosiveAmmo(bool enable);
		void FireAmmo(bool enable);
		void NoRecoil(bool enable);
		void NoSpread(bool enable);
		void ShotGunMode(bool enable);
		void OneShotKill(bool enable);
		void NoCooldown(bool enable);
		void NoSpinUp(bool enable);
		void PortalGun(bool enable);
		void TeleportGun(bool enable);
		void SkydiveGun(bool enable);
		void AirstrikeGun(bool enable);
		void PushGun(bool enable);
		void PullGun(bool enable);
		void PedGun(bool enable);
		void RapidFire(bool enable);
		void DeleteGun(bool enable);
		void HijackGun(bool enable);
		void RopeGun(bool enable);
		void ValkyrieGun(bool enable);
		void DeadEyes(bool enable);
		void LaserSight(bool enable);
		void DefibrillatorGun(bool enable);
		void HashGun(bool enable);
		void ShrinkGun(bool enable);
		void ClownGun(bool enable);
		void RepairGun(bool enable);
		void RecolorGun(bool enable);
		void InstrumentOfDeath(bool enable);
		void VortexGunEnable(bool enable);
		void EnableTotemGun(bool enable);
		void EnableBulletTracers(bool enable);
		void EnableGravityGun(bool enable);
		void EnableBulletChanger(bool enable);
		void EnableImpactGun(bool enable);
		void EnablePaintGun(bool enable);
		void EnableParticleGun(bool enable);
		void EnableEntityGun(bool enable);
		void EnableWeaponWidth(bool enable);
		void EnableWeaponHeight(bool enable);
		void EnableWeaponScale(bool enable);
		void EnablePositionX(bool enable);
		void EnablePositionY(bool enable);
		void AllowWeaponsInVehicle(bool enable);
		void FriendlyFire(bool enable);
		void InstantLockOn(bool enable);
		void BypassC4Limit(bool enable);
		void BypassFlareLimit(bool enable);
		void EnableTriggerBot(bool enable);
		void EnableAimBot(bool enable);
		void RainbowRegularTint(bool enable);
		void RainbowMk2Tint(bool enable);
		void EnableCrosshair(bool enable);
	public:
		std::uint32_t m_PreviousWeaponHash{};

		std::vector<std::pair<std::uint32_t, float>> m_OriginalRecoilValues{};
		bool IsRecoilValueCached(std::uint32_t hash)
		{
			return std::find_if(m_OriginalRecoilValues.begin(), m_OriginalRecoilValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalRecoilValues.end();
		}

		float GetOriginalRecoilValue(std::uint32_t hash)
		{
			return std::find_if(m_OriginalRecoilValues.begin(), m_OriginalRecoilValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetRecoilValue(std::uint32_t hash)
		{
			return m_NoRecoil
				? 0.f
				: GetOriginalRecoilValue(hash);
		}

		std::vector<std::pair<std::uint32_t, float>> m_OriginalSpreadValues{};
		bool IsSpreadValueCached(std::uint32_t hash)
		{
			return std::find_if(m_OriginalSpreadValues.begin(), m_OriginalSpreadValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalSpreadValues.end();
		}

		float GetOriginalSpeadValue(std::uint32_t hash)
		{
			return std::find_if(m_OriginalSpreadValues.begin(), m_OriginalSpreadValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetSpreadValue(std::uint32_t hash)
		{
			return m_NoSpread
				? 0.f
				: GetOriginalSpeadValue(hash);
		}

		std::vector<std::pair<std::uint32_t, float>> m_OriginalDamageValues{};
		bool IsDamageValueCached(std::uint32_t hash)
		{
			return std::find_if(m_OriginalDamageValues.begin(), m_OriginalDamageValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalDamageValues.end();
		}

		float GetOriginalDamageValue(std::uint32_t hash)
		{
			return std::find_if(m_OriginalDamageValues.begin(), m_OriginalDamageValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetDamageValue(std::uint32_t hash)
		{
			return m_OneShotKill
				? 1000.f
				: GetOriginalDamageValue(hash);
		}

		std::vector<std::pair<std::uint32_t, float>> m_OriginalCooldownValues{};
		bool IsCooldownValueCached(std::uint32_t hash)
		{
			return std::find_if(m_OriginalCooldownValues.begin(), m_OriginalCooldownValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalCooldownValues.end();
		}

		float GetOriginalCooldownValue(std::uint32_t hash)
		{
			return std::find_if(m_OriginalCooldownValues.begin(), m_OriginalCooldownValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetCooldownValue(std::uint32_t hash)
		{
			return m_NoCooldown
				? 0.f
				: GetOriginalCooldownValue(hash);
		}

		std::vector<std::pair<std::uint32_t, float>> m_OriginalSpinUpValues{};
		bool IsSpinUpValueCached(std::uint32_t hash)
		{
			return std::find_if(m_OriginalSpinUpValues.begin(), m_OriginalSpinUpValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalSpinUpValues.end();
		}

		float GetOriginalSpinUpValue(std::uint32_t hash)
		{
			return std::find_if(m_OriginalSpinUpValues.begin(), m_OriginalSpinUpValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
		}

		float GetSpinUpValue(std::uint32_t hash)
		{
			return m_NoSpinUp
				? 0.f
				: GetOriginalSpinUpValue(hash);
		}
	};

	inline std::unique_ptr<WeaponFeatures> g_WeaponFeatures;
}