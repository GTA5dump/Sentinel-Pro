#pragma once 
#include "../Common.hpp"
#include "../GTA/Invoker/Natives.hpp"

namespace Sentinel
{
	class SelfFeatures
	{
	public:
		void Tick()
		{
			GodMode(m_GodMode);
			SemiGodMode(m_SemiGodMode);
			UnbreakableArmor(m_UnbreakableArmor);
			AutoRestore(m_AutoRestore);
			RestoreInCover(m_RestoreInCover);
			NoClip(m_NoClip);
			SlideRun(m_SlideRun);
			NinjaJump(m_NinjaJump);
			SuperMan(m_SuperMan);
			WalkOnAir(m_WalkOnAir);
			WalkOnWater(m_WalkOnWater);
			WalkUnderWater(m_WalkUnderWater);
			NoRagdoll(m_NoRagdoll);
			WalkThroughWater(m_WalkThroughWater);
			Freecam(m_Freecam);
			EnableRunSpeed(m_EnableRunSpeed);
			EnableSwimSpeed(m_EnableSwimSpeed);
			EnablePedWidth(m_EnablePedWidth);
			EnablePedHeight(m_EnablePedHeight);
			EnablePedLean(m_EnablePedLean);
			ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), m_BulletProof, m_FireProof, m_ExplosionProof, m_CollisionProof, m_MeleeProof, TRUE, FALSE, m_DrownProof);
			SwimAnywhere(m_SwimAnywhere);
			NoMelee(m_NoMelee);
			CanPedFlyThroughWindshield(m_CanPedFlyThroughWindshield);
			PedDiesByRagdoll(m_PedDiesByRagdoll);
			NoJumping(m_NoJumping);
			NoCollision(m_NoCollision);
			ReducedCollision(m_ReducedCollision);
			Freeze(m_Freeze);
			Shrink(m_Shrink);
			Handtrails(m_Handtrails);
			NeverWanted(m_NeverWanted);
			DisableSharing(m_DisableSharing);
			EnableFakeWantedLevel(m_EnableFakeWantedLevel);
			EnableRealWantedLevel(m_EnableRealWantedLevel);
			QuickRespawn(m_QuickRespawn);
			RespawnAtDeath(m_RespawnAtDeath);
			InfiniteStamina(m_InfiniteStamina);
			Ignored(m_Ignored);
			AntiNPC(m_AntiNPC);
			Karma(m_Karma);
			FireBreath(m_FireBreath);
			Vomit(m_Vomit);
			SlowMotion(m_SlowMotion);
			MobileRadio(m_MobileRadio);
			Invisibility(m_Invisibility);
			EnableOpacity(m_EnableOpacity);
			ParticleMan();
			LoopFireworkParticles(m_LoopFireworkParticles);
			LoopClownParticles(m_LoopClownParticles);
			LoopAlienParticles(m_LoopAlienParticles);
			LoopConstructionParticles(m_LoopConstructionParticles);
			LoopGoreParticles(m_LoopGoreParticles);
			LoopMiscParticles(m_LoopMiscParticles);
			ModelChanger();
			OffRadar(m_OffRadar);
			CopsTurnBlindEye(m_CopsTurnBlindEye);
			RevealPlayers(m_RevealPlayers);
			YogaGod(m_YogaGod);
			ElectricSprint(m_ElectricSprint);
			LazerEyes(m_LazerEyes);
			ForcePush(m_ForcePush);
			ForcePull(m_ForcePull);
			Forcefield(m_EnableForcefield);
			Blackout(m_Blackout);
			ThermalVision(m_ThermalVision);
			NightVision(m_NightVision);
		}

		bool m_GodMode = false;
		bool m_SemiGodMode = false;
		bool m_UnbreakableArmor = false;
		const char* const m_RestoreComponent[3]
		{
			"Health",
			"Armor",
			"Health + Armor"
		};
		int m_ComponentPos = 0;
		bool m_AutoRestore = false;
		bool m_RestoreInCover = false;
		bool m_NoClip = false;
		float m_NoClipSpeed = 1.f;
		bool m_SlideRun = false;
		float m_SlideRunSpeed = 1.f;
		bool m_NinjaJump = false;
		int m_NinjaJumps = 2;
		bool m_SuperMan = false;
		bool m_SuperJump = false;
		bool m_BeastJump = false;
		bool m_GracefulLanding = false;
		bool m_WalkOnAir = false;
		bool m_WalkOnWater = false;
		bool m_WalkUnderWater = false;
		bool m_NoRagdoll = false;
		bool m_WalkThroughWater = false;
		bool m_Freecam = false;
		float m_HealthRegenModifier = 1.f;
		bool m_EnableRunSpeed = false;
		float m_RunSpeed = 1.f;
		bool m_EnableSwimSpeed = false;
		float m_SwimSpeed = 1.f;
		bool m_EnablePedWidth = false;
		float m_PedWidth = 1.f;
		bool m_EnablePedHeight = false;
		float m_PedHeight = 1.f;
		bool m_EnablePedLean = false;
		float m_PedLean = 1.f;
		bool m_FireProof = false;
		bool m_ExplosionProof = false;
		bool m_BulletProof = false;
		bool m_CollisionProof = false;
		bool m_MeleeProof = false;
		bool m_DrownProof = false;
		bool m_SwimAnywhere = false;
		bool m_NoMelee = false;
		bool m_CanPedFlyThroughWindshield = false;
		bool m_PedDiesByRagdoll = false;
		bool m_NoJumping = false;
		bool m_NoCollision = false;
		bool m_ReducedCollision = false;
		bool m_Freeze = false;
		bool m_Shrink = false;
		bool m_Handtrails = false;
		bool m_RainbowHandtrails = false;
		float m_HandTrailsColor[3];
		float m_HandtrailsScale = 0.2f;
		bool m_NeverWanted = false;
		bool m_DisableSharing = false;
		int m_FakeWantedLevel = 0;
		bool m_EnableFakeWantedLevel = false;
		int m_RealWantedLevel = 0;
		bool m_EnableRealWantedLevel = false;
		bool m_QuickRespawn = false;
		bool m_RespawnAtDeath = false;
		float m_Stamina = 0.f;
		bool m_InfiniteStamina = false;
		bool m_Ignored = false;
		bool m_AntiNPC = false;
		bool m_Karma = false;
		int m_FireBreathHandle = 0;
		bool m_FireBreath = false;
		int m_VomitHandle = 0;
		bool m_Vomit = false;
		bool m_SlowMotion = false;
		bool m_MobileRadio = false;
		bool m_Invisibility = false;
		const char* const m_InvisibilityMode[3]
		{
			"Local",
			"Network",
			"Network + Local"
		};
		int m_InvisibilityModePos = 0;
		int m_Opacity = 255;
		bool m_EnableOpacity = false;
		const char* const m_ParticleManTypes[6]
		{
			"Ghost Rider",
			"Alien Disintegrate",
			"Alien Teleport",
			"Clown Appear",
			"Clown Death",
			"Smoke"
		};
		int m_ParticleManTypesPos = 0;
		float m_ParticleManScale = 0.2f;
		bool m_ParticleManHead = false;
		bool m_ParticleManLeftHand = false;
		bool m_ParticleManRightHand = false;
		bool m_ParticleManLeftFoot = false;
		bool m_ParticleManRightFoot = false;
		float m_ParticleOnPedScale = 0.2f;
		bool m_LoopFireworkParticles = false;
		const char* const m_FireworkParticles[5]
		{
			"Trailburst",
			"Starburst",
			"Shotburst",
			"Fountain",
			"Ringburst"
		};
		int m_FireworkParticlesPos = 0;
		bool m_LoopClownParticles = false;
		const char* const m_ClownParticles[3]
		{
			"Appears",
			"Death",
			"Explode"
		};
		int m_ClownParticlesPos = 0;
		bool m_LoopAlienParticles = false;
		const char* const m_AlienParticles[3]
		{
			"Teleport",
			"Disintigrate",
			"Impact"
		};
		int m_AlienParticlesPos = 0;
		bool m_LoopConstructionParticles = false;
		const char* const m_ConstructionParticles[5]
		{
			"Electric Box",
			"Electric Sphere",
			"Dirt Flash",
			"Burial Dirt",
			"Machine Smoke"
		};
		int m_ConstructionParticlesPos = 0;
		bool m_LoopGoreParticles = false;
		const char* const m_GoreParticles[4]
		{
			"Blood Splash",
			"Blood Stab",
			"Ped Sliced",
			"Sniper Impact"
		};
		int m_GoreParticlesPos = 0;
		bool m_LoopMiscParticles = false;
		const char* const m_MiscParticles[4]
		{
			"Camera Flash",
			"Banknote Trail",
			"Water Splash",
			"Truck Crash"
		};
		int m_MiscParticlesPos = 0;
		bool m_ControllableAnimation = false;
		bool m_ContortAnimation = false;
		Hash m_LastModel;
		bool m_ReapplyModel = false;
		char m_CustomModel[30] = "";
		bool m_OffRadar = false;
		bool m_CopsTurnBlindEye = false;
		bool m_RevealPlayers = false;
		Color m_SuperPowerMarkers = { 99, 66, 245, 100 };
		bool m_YogaGod = false;
		bool m_ElectricSprint = false;
		bool m_LazerEyes = false;
		bool m_ForcePush = false;
		bool m_ForcePull = false;
		float m_ForcefieldRange = 25.f;
		bool m_ForcefieldSphere = true;
		bool m_ForcefieldPeds = true;
		bool m_ForcefieldVehicles = true;
		bool m_EnableForcefield = false;
		bool m_Blackout = false;
		bool m_ThermalVision = false;
		bool m_NightVision = false;
		const char* const m_BodyGuardModel[8]
		{
			"Alien",
			"Clown",
			"Pogo",
			"Jezus",
			"Body Builder",
			"Zombie",
			"Hooker",
			"Stripper"
		};
		int m_BodyGuardModelPos = 0;
		const char* const m_BodyGuardWeapon[8]
		{
			"Pistol",
			"Tazer",
			"RPG",
			"Minigun",
			"Assault Rifle",
			"Widowmaker",
			"Hellbringer",
			"Gusenberg"
		};
		int m_BodyGuardWeaponPos = 0;
		bool m_BodyGuardInvincible = false;
		bool m_BodyGuardInvisible = false;
		bool m_BodyGuardNoRagdoll = false;
		std::vector<Entity> m_CreatedBodyguards;
	private:
		void GodMode(bool enable);
		void SemiGodMode(bool enable);
		void UnbreakableArmor(bool enable);
		void AutoRestore(bool enable);
		void RestoreInCover(bool enable);
		void NoClip(bool enable);
		void SlideRun(bool enable);
		void NinjaJump(bool enable);
		void SuperMan(bool enable);
		void WalkOnAir(bool enable);
		void WalkOnWater(bool enable);
		void WalkUnderWater(bool enable);
		void NoRagdoll(bool enable);
		void WalkThroughWater(bool enable);
		void Freecam(bool enable);
		void EnableRunSpeed(bool enable);
		void EnableSwimSpeed(bool enable);
		void EnablePedWidth(bool enable);
		void EnablePedHeight(bool enable);
		void EnablePedLean(bool enable);
		void SwimAnywhere(bool enable);
		void NoMelee(bool enable);
		void CanPedFlyThroughWindshield(bool enable);
		void PedDiesByRagdoll(bool enable);
		void NoJumping(bool enable);
		void NoCollision(bool enable);
		void ReducedCollision(bool enable);
		void Freeze(bool enable);
		void Shrink(bool enable);
		void Handtrails(bool enable);
		void NeverWanted(bool enable);
		void DisableSharing(bool enable);
		void EnableFakeWantedLevel(bool enable);
		void EnableRealWantedLevel(bool enable);
		void QuickRespawn(bool enable);
		void RespawnAtDeath(bool enable);
		void InfiniteStamina(bool enable);
		void Ignored(bool enable);
		void AntiNPC(bool enable);
		void Karma(bool enable);
		void FireBreath(bool enable);
		void Vomit(bool enable);
		void SlowMotion(bool enable);
		void MobileRadio(bool enable);
		void Invisibility(bool enable);
		void EnableOpacity(bool enable);
		void ParticleMan();
		void LoopFireworkParticles(bool enable);
		void LoopClownParticles(bool enable);
		void LoopAlienParticles(bool enable);
		void LoopConstructionParticles(bool enable);
		void LoopGoreParticles(bool enable);
		void LoopMiscParticles(bool enable);
		void ModelChanger();
		void OffRadar(bool enable);
		void CopsTurnBlindEye(bool enable);
		void RevealPlayers(bool enable);
		void YogaGod(bool enable);
		void ElectricSprint(bool enable);
		void LazerEyes(bool enable);
		void ForcePush(bool enable);
		void ForcePull(bool enable);
		void Forcefield(bool enable);
		void Blackout(bool enable);
		void ThermalVision(bool enable);
		void NightVision(bool enable);
	};

	inline std::unique_ptr<SelfFeatures> g_SelfFeatures;
}