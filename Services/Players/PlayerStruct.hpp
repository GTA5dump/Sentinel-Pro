#pragma once
#include "PlayerService.hpp"
#include "../../GTA/Include.hpp"
#include <GTAV-Classes/vehicle/CVehicle.hpp>
#include <GTAV-Classes/network/snSession.hpp>

namespace Sentinel
{
	class PlayerStruct final
	{
		friend class PlayerService;

		CNetGamePlayer* m_NetGamePlayer = nullptr;
		std::string m_Identifier;
		bool m_IsFriend;

	public:
		explicit PlayerStruct(CNetGamePlayer* net_game_player);
		~PlayerStruct() = default;

		PlayerStruct(const PlayerStruct&) = default;
		PlayerStruct(PlayerStruct&&) noexcept = default;
		PlayerStruct& operator=(const PlayerStruct&) = default;
		PlayerStruct& operator=(PlayerStruct&&) noexcept = default;

		[[nodiscard]] CVehicle* GetCurrentVehicle() const;
		[[nodiscard]] const char* GetName() const;
		[[nodiscard]] rage::rlGamerInfo* GetNetData() const;
		[[nodiscard]] CNetGamePlayer* GetNetPlayer() const;
		[[nodiscard]] CPed* GetPed() const;
		[[nodiscard]] CPlayerInfo* GetPlayerInfo() const;
		[[nodiscard]] class rage::snPlayer* GetSessionPlayer();
		[[nodiscard]] class rage::snPeer* GetSessionPeer();
		[[nodiscard]] netAddress GetIPAddress();
		[[nodiscard]] std::uint16_t GetPort();

		[[nodiscard]] std::uint8_t ID() const;

		[[nodiscard]] bool IsFriend() const;
		[[nodiscard]] bool IsHost() const;
		[[nodiscard]] bool IsValid() const;

		// Info
		bool m_ScriptHost = false;

		// Trolling
		bool m_KillPlayer = false;
		bool m_FreezePlayer = false;
		bool m_RagdollPlayer = false;
		bool m_ExplodePlayer = false;
		bool m_BurnPlayer = false;
		bool m_FlameRay = false;
		bool m_WaterHydrant = false;
		bool m_ShakeCamera = false;
		bool m_GlitchPhysics = false;
		bool m_RainRockets = false;
		bool m_CloneSpam = false;
		bool m_Glitchfield = false;
		bool m_GiveWantedLevel = false;
		int m_WantedLevelToGive = 0;

		// Shoot
		const char* const m_ShootBullet[6]
		{
			"Pistol",
			"Tazer",
			"Shotgun",
			"Sniper",
			"Rifle",
			"Lazer"
		};
		int m_ShootBulletPos = 0;
		bool m_LoopShoot = false;

		const char* const m_AppartmentNames[116] =
		{
			"INVALID",
			"Eclipse Towers, Apt 31",
			"Eclipse Towers, Apt 9",
			"Eclipse Towers, Apt 40",
			"Eclipse Towers, Apt 5",
			"3 Alta St, Apt 10",
			"3 Alta St, Apt 57",
			"Del Perro Heights, Apt 20",
			"1162 Power St, Apt 3",
			"0605 Spanish Ave, Apt 1",
			"0604 Las Lagunas Blvd, Apt 4",
			"0184 Milton Rd, Apt 13",
			"The Royale, Apt 19",
			"0504 S Mo Milton Dr",
			"0115 Bay City Ave, Apt 45",
			"0325 South Rockford Dr",
			"Dream Tower, Apt 15",
			"2143 Las Lagunas Blvd, Apt 9",
			"1561 San Vitas St, Apt 2",
			"0112 S Rockford Dr, Apt 13",
			"2057 Vespucci Blvd, Apt 1",
			"0069 Cougar Ave, Apt 19",
			"1237 Prosperity St, Apt 21",
			"1115 Blvd Del Perro, Apt 18",
			"0120 Murrieta Heights",
			"Unit 14 Popular St",
			"Unit 2 Popular St",
			"331 Supply St",
			"Unit 1 Olympic Fwy",
			"0754 Roy Lowenstein Blvd",
			"12 Little Bighorn Ave",
			"Unit 124 Popular St",
			"0552 Roy Lowenstein Blvd",
			"0432 Davis Ave",
			"Del Perro Heights, Apt 7",
			"Weazel Plaza, Apt 101",
			"Weazel Plaza, Apt 70",
			"Weazel Plaza, Apt 26",
			"4 Integrity Way, Apt 30",
			"4 Integrity Way, Apt 35",
			"Richards Majestic, Apt 4",
			"Richards Majestic, Apt 51",
			"Tinsel Towers, Apt 45",
			"Tinsel Towers, Apt 29",
			"142 Paleto Blvd",
			"1 Strawberry Ave",
			"1932 Grapeseed Ave",
			"1920 Senora Way",
			"2000 Great Ocean Highway",
			"197 Route 68",
			"870 Route 68 Approach",
			"1200 Route 68",
			"8754 Route 68",
			"1905 Davis Ave",
			"1623 South Shambles St",
			"4531 Dry Dock St",
			"1337 Exceptionalists Way",
			"Unit 76 Greenwich Parkway",
			"Garage Innocence Blvd",
			"634 Blvd Del Perro",
			"0897 Mirror Park Blvd",
			"Eclipse Towers, Apt 3",
			"Del Perro Heights, Apt 4",
			"Richards Majestic, Apt 2",
			"Tinsel Towers, Apt 42",
			"4 Integrity Way, Apt 28",
			"4 Hangman Ave",
			"12 Sustancia Rd",
			"4584 Procopio Dr",
			"4401 Procopio Dr",
			"0232 Paleto Blvd",
			"140 Zancudo Ave",
			"1893 Grapeseed Ave",
			"3655 Wild Oats Drive",
			"2044 North Conker Avenue",
			"2868 Hillcrest Avenue",
			"2862 Hillcrest Avenue",
			"3677 Whispymound Drive",
			"2117 Milton Road",
			"2866 Hillcrest Avenue",
			"2874 Hillcrest Avenue",
			"2113 Mad Wayne Thunder Drive",
			"2045 North Conker Avenue",
			"Eclipse Towers, Penthouse Suite 1",
			"Eclipse Towers, Penthouse Suite 2",
			"Eclipse Towers, Penthouse Suite 3",
			"Private Yacht",
			"Lombank West",
			"Maze Bank West",
			"Arcadius Business Center",
			"Maze Bank Tower",
			"Rancho Clubhouse",
			"Del Perro Beach Clubhouse",
			"Pillbox Hill Clubhouse",
			"Great Chaparral Clubhouse",
			"Paleto Bay Clubhouse",
			"Sandy Shores Clubhouse",
			"La Mesa Clubhouse",
			"Downtown Vinewood Clubhouse",
			"Hawick Clubhouse",
			"Grapeseed Clubhouse",
			"Paleto Bay Clubhouse",
			"Vespucci Beach Clubhouse",
			"Office Garage 1",
			"Office Garage 2",
			"Office Garage 3",
			"Office Garage 1",
			"Office Garage 2",
			"Office Garage 3",
			"Office Garage 1",
			"Office Garage 2",
			"Office Garage 3",
			"Office Garage 1",
			"Office Garage 2",
			"Office Garage 3",
			"Vehicle Warehouse"
		};
		int m_AppartmentIndex = 1;

		const char* const m_WarehouseNames[23] =
		{
			"INVALID",
			"Pacific Bait Storage",
			"White Widow Garage",
			"Celltowa Unit",
			"Convenience Store Lockup",
			"Foreclosed Garage",
			"Xero Gas Factory",
			"Derriere Lingerie Backlot",
			"Bilgeco Warehouse",
			"Pier 400 Utility Building",
			"GEE Warehouse",
			"LS Marine Building 3",
			"Railyard Warehouse",
			"Fridgit Annexe",
			"Disused Factory Outlet",
			"Discount Retail Unit",
			"Logistics Depot",
			"Darnell Bros Warehouse",
			"Wholesale Furniture",
			"Cypress Warehouses",
			"West Vinewood Backlot",
			"Old Power Station",
			"Walker & Sons Warehouse"
		};
		int m_WarehouseIndex = 1;

		const char* const m_InteriorNames[10] =
		{
			"Mobile Operations Center",
			"Casino",
			"Penthouse",
			"Arcade",
			"Music Locker",
			"Record A Studios",
			"Custom Auto Shop",
			"Agency",
			"Freakshop",
			"Multi Floor Garage"
		};
		int m_InteriorIndex = 0;

		// Bounty Placement
		int m_BountyAmount = 9000;
		bool m_AnonymousBounty = true;

		// Action Denying
		bool m_DenyVehicles = false;
		bool m_DenyPlanes = false;
		bool m_DenyHelicopters = false;
		bool m_DenyBoats = false;
		bool m_DenyAiming = false;
		bool m_DenyShooting = false;
		bool m_DenyCover = false;
		bool m_DenyReloading = false;
		bool m_DenyWeaponSwitch = false;
		bool m_DenySkydiving = false;
		bool m_DenyJumping = false;
		bool m_DenySwimming = false;
		bool m_DenySprinting = false;
		bool m_DenyWalking = false;
		bool m_DenyRagdoll = false;
		bool m_DenyClimbing = false;

		// Explosions
		const char* const m_ExplosionTag[84]
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
		int m_ExplosionTagPos = 0;
		float m_DamageScale = 999.f;
		float m_CameraShake = 0.f;
		bool m_Audible = true;
		bool m_Visible = true;
		bool m_TakeBlame = false;
		bool m_OnPlayer = false;
		bool m_WeaponImpact = false;

		// Remote Events
		const char* const m_RemoteEvent[14]
		{
			"CEO Kick",
			"CEO Join",
			"Enter Interior",
			"Vehicle Kick",
			"Send To Mission",
			"Send To Island",
			"Send Sext",
			"Send Transaction Error",
			"Send Ban Message",
			"Send Money Stolen Message",
			"Send Money Removed Message",
			"Send Money Banked Message",
			"Send Spectate Message",
			"Send Sound Spam"
		};
		int m_RemoteEventPos = 0;
		bool m_LoopedRemoteEvents = false;

		// Entity Spammer
		const char* const m_EntitySpammer[8]
		{
			"Mexicans",
			"Black People",
			"Clones",
			"Planes",
			"Helicopters",
			"Boats",
			"Super Cars",
			"Military Equipment"
		};
		int m_EntitySpammerPos = 0;
		bool m_EntitySpammerEnable = false;
		int m_EntitySpammerDelay = 500;
		std::vector<std::int32_t> m_CreatedEntities;

		// SMS Service
		char m_TextMessage[64] = { 'E','m','p','t', 'y', '\0' };
		const char* const m_MessageColor[11]
		{
			"White",
			"Red",
			"Blue",
			"Green",
			"Yellow",
			"Purple",
			"Pink",
			"Orange",
			"Gray",
			"Dark Gray",
			"Black"
		};
		int m_MessageColorPos = 0;

		const char* const m_MessageFont[4]
		{
			"Normal",
			"Italic",
			"Bold",
			"Italic + Bold"
		};
		int m_MessageFontPos = 0;

		// Private Chat Spammer
		char m_ChatMessage[256] = { 'E','m','p','t', 'y', '\0' };
		bool m_DrawLocally = true;
		int m_SpamDelay = 100;
		bool m_StartSpamming = false;

		// Bodyguards	
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
		std::vector<std::int32_t> m_CreatedBodyguards;

		// Attackers	
		const char* const m_AttackerModel[8]
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
		int m_AttackerModelPos = 0;
		const char* const m_AttackerWeapon[8]
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
		int m_AttackerWeaponPos = 0;
		bool m_AttackerInvincible = false;
		bool m_AttackerInvisible = false;
		bool m_AttackerNoRagdoll = false;
		std::vector<std::int32_t> m_CreatedAttackers;

		// Sound Player
		const char* const m_SoundNames[3]
		{
			"Timer",
			"Ring",
			"Klaxon"
		};
		int m_SoundNamesPos = 0;
		int m_SoundSpammerDelay = 100;
		bool m_EnableSoundSpammer = false;

		// Angry Planes
		bool m_EnableAngryPlanes = false;
		bool m_SpawnWithRockets = false;
		bool m_SpawnInvincible = false;
		bool m_SpawnInvisible = false;
		int m_AmountOfPlanes = 15;

		// Friendly
		bool m_SemiGod = false;
		bool m_UnbreakableArmor = false;
		bool m_NeverWanted = false;
		bool m_OffRadar = false;
		bool m_RevealPlayers = false;

		// Drop Manager
		const char* const m_DropType[3]
		{
			"Money",
			"RP",
			"Fake Money"
		};
		int m_DropTypePos = 0;

		const char* const m_DropModel[15]
		{
			"Money Bag",
			"Alien Egg",
			"Weed",
			"Beach Fire",
			"Xmas Tree",
			"Bear",
			"Gold Bar",
			"Guitar",
			"Dildo",
			"Basketball",
			"Bong",
			"C4",
			"Ammo Box",
			"Toilet",
			"Wheelchair"
		};
		int m_DropModelPos = 0;

		const char* const m_DropStyle[4]
		{
			"Traditional",
			"Rain",
			"Shower",
			"Bait"
		};
		int m_DropStylePos = 0;

		int m_DropDelay = 2500;
		bool m_StartDropping = false;

		// Forcefield
		float m_ForcefieldRange = 25.f;
		bool m_ForcefieldSphere = true;
		bool m_ForcefieldPeds = true;
		bool m_ForcefieldVehicles = true;
		bool m_EnableForcefield = false;

		// Weapon
		bool m_BulletRedirection = false;
		bool m_PedGun = false;
		bool m_AirstrikeGun = false;
		bool m_TeleportGun = false;
		bool m_MoneyGun = false;
		bool m_FakeMoneyGun = false;

		// Vehicle
		bool m_VehicleInvincibility = false;
		bool m_DisorientHandling = false;
		bool m_NoCollision = false;
		bool m_SlippyWheels = false;

	protected:
		bool Equals(const CNetGamePlayer* player) const;

		[[nodiscard]] std::string ToLowerCaseIdentifier() const;

	};
}