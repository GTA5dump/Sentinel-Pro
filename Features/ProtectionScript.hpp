#pragma once 
#include "../Common.hpp"
#include <GTAV-Classes/network/CNetGamePlayer.hpp>

namespace Sentinel
{
	class ProtectionScript
	{
	public:
		// Network Events
		bool m_VoteKick = true;
		bool m_Freeze = true;
		bool m_Ragdoll = true;
		bool m_StateChange = true;
		bool m_RequestControl = true;
		bool m_GiveControl = true;
		bool m_RemoveWeapon = true;
		bool m_RemoveAllWeapons = true;
		bool m_GiveWeapons = true;
		bool m_Explosions = true;
		bool m_NetworkEventsNotify = true;
		bool m_PTFX = true;

		// Script Events
		bool m_Bounty = true;
		bool m_CEOKick = true;
		bool m_CEOMoney = true;
		bool m_CEORaid = true;
		bool m_ClearWantedLevel = true;
		bool m_MoneyNotifications = true;
		bool m_ForceMission = true;
		bool m_GiveCollectible = true;
		bool m_GTABanner = true;
		bool m_RemoteTeleport = true;
		bool m_VehicleMessage = true;
		bool m_RemoteOffRadar = true;
		bool m_RotateCam = true;
		bool m_ForceToCayo = true;
		bool m_ForceToCutscene = true;
		bool m_SoundSpam = true;
		bool m_SpectateMessage = true;
		bool m_TransactionError = true;
		bool m_VehicleKick = true;
		bool m_BailKick = true;
		bool m_ForceActivity = true;
		bool m_NullKick = true;
		bool m_SMS = true;
		bool m_InteriorKick = true;
		bool m_TSECrash = true;
		bool m_ScriptEventsNotify = true;

		// Increment Events
		bool m_BadCrewStatus = true;
		bool m_BadCrewMotto = true;
		bool m_BadCrewName = true;
		bool m_BadCrewEmblem = true;
		bool m_Exploits = true;
		bool m_GameExploits = true;
		bool m_ChatAnnoying = true;
		bool m_ChatHate = true;
		bool m_VoiceAnnoying = true;
		bool m_VoiceHate = true;
		bool m_IncrementEventsNotify = true;

		// Manual Patches
		bool m_SweepCrash = true;
		bool m_FragmentCrash = true;
		bool m_ConstraintCrash = true;
		bool m_InvalidDecalCrash = true;
		bool m_InfiniteTrainCrash = true;
		bool m_BDSMCrash = true;
		bool m_KanyeCrash = true;
		bool m_ManualPatchesNotify = true;

		// Miscellaneous
		bool m_Pickups = true;
		bool m_BlockRIDJoin = true;
		bool m_BlockMaliciousSyncs = true;
		bool m_DisableTransactionError = true;
		bool m_ScriptHostKick = true;
		bool m_EndSessionKick = true;
		bool m_AntiCheatEvents = true;
		bool m_MiscellaneousNotify = true;

		// Data Nodes
		bool m_InvalidVehicles = true;
		bool m_InvalidDoorModel = true;
		bool m_InvalidPickupModel = true;
		bool m_InvalidAttachment = true;
		bool m_InvalidPedModel = true;
		bool m_InvalidPlayerModel = true;
		bool m_InvalidPedPropModel = true;
		bool m_InvalidObjects = true;
		bool m_InvalidSector = true;
		bool m_BlockCages = true;
		bool m_VehicleManipulation = true;
		bool m_DataNodesNotify = true;

		CNetGamePlayer* m_SyncingPlayer{};
	};

	inline std::unique_ptr<ProtectionScript> g_ProtectionScript;
}