#include "../../Common.hpp"
#include "Submenus.hpp"
#include "../Elements.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../GTA/Script/ScriptQueue.hpp"
#include "../../Features/ProtectionScript.hpp"
#include "../../GTA/Script/ScriptPatches.hpp"

namespace Sentinel
{
	void Submenus::ProtectionSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###protections", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Network Events", ImVec2(290.f, 205.f), ImVec2(0.f, 0.f));
			Elements::BoolOption("Vote Kick", &g_ProtectionScript->m_VoteKick, ImVec2(10.f, 30.f));
			Elements::BoolOption("Freeze", &g_ProtectionScript->m_Freeze, ImVec2(145.f, 30.f));
			Elements::BoolOption("Ragdoll", &g_ProtectionScript->m_Ragdoll, ImVec2(10.f, 60.f));
			Elements::BoolOption("State Change", &g_ProtectionScript->m_StateChange, ImVec2(145.f, 60.f));
			Elements::BoolOption("Request Control", &g_ProtectionScript->m_RequestControl, ImVec2(10.f, 90.f));
			Elements::BoolOption("Give Control", &g_ProtectionScript->m_GiveControl, ImVec2(145.f, 90.f));
			Elements::BoolOption("Remove Weapon", &g_ProtectionScript->m_RemoveWeapon, ImVec2(10.f, 120.f));
			Elements::BoolOption("Remove All Weapons", &g_ProtectionScript->m_RemoveAllWeapons, ImVec2(145.f, 120.f));
			Elements::BoolOption("Give Weapon", &g_ProtectionScript->m_GiveWeapons, ImVec2(10.f, 150.f));
			Elements::BoolOption("Explosions", &g_ProtectionScript->m_Explosions, ImVec2(145.f, 150.f));
			Elements::BoolOption("PTFX", &g_ProtectionScript->m_PTFX, ImVec2(10.f, 180.f));
			Elements::BoolOption("Notify##1", &g_ProtectionScript->m_NetworkEventsNotify, ImVec2(145.f, 180.f));

			Elements::SubOption("Script Events", ImVec2(290.f, 415.f), ImVec2(300.f, 0.f));
			Elements::BoolOption("Bounty", &g_ProtectionScript->m_Bounty, ImVec2(310.f, 30.f));
			Elements::BoolOption("CEO Kick", &g_ProtectionScript->m_CEOKick, ImVec2(445.f, 30.f));
			Elements::BoolOption("CEO Money", &g_ProtectionScript->m_CEOMoney, ImVec2(310.f, 60.f));
			Elements::BoolOption("CEO Raid", &g_ProtectionScript->m_CEORaid, ImVec2(445.f, 60.f));
			Elements::BoolOption("Remote CWL", &g_ProtectionScript->m_ClearWantedLevel, ImVec2(310.f, 90.f));
			Elements::BoolOption("Money Notify", &g_ProtectionScript->m_MoneyNotifications, ImVec2(445.f, 90.f));
			Elements::BoolOption("Force Mission", &g_ProtectionScript->m_ForceMission, ImVec2(310.f, 120.f));
			Elements::BoolOption("Give Collectible", &g_ProtectionScript->m_GiveCollectible, ImVec2(445.f, 120.f));
			Elements::BoolOption("GTA Banner", &g_ProtectionScript->m_GTABanner, ImVec2(310.f, 150.f));
			Elements::BoolOption("Remote Teleport", &g_ProtectionScript->m_RemoteTeleport, ImVec2(445.f, 150.f));
			Elements::BoolOption("Vehicle Message", &g_ProtectionScript->m_VehicleMessage, ImVec2(310.f, 180.f));
			Elements::BoolOption("Remote OTR", &g_ProtectionScript->m_RemoteOffRadar, ImVec2(445.f, 180.f));
			Elements::BoolOption("Rotate Cam", &g_ProtectionScript->m_RotateCam, ImVec2(310.f, 210.f));
			Elements::BoolOption("Force To Cayo", &g_ProtectionScript->m_ForceToCayo, ImVec2(445.f, 210.f));
			Elements::BoolOption("Force To Cutscene", &g_ProtectionScript->m_ForceToCutscene, ImVec2(310.f, 240.f));
			Elements::BoolOption("Sound Spam", &g_ProtectionScript->m_SoundSpam, ImVec2(445.f, 240.f));
			Elements::BoolOption("Spectate Message", &g_ProtectionScript->m_SpectateMessage, ImVec2(310.f, 270.f));
			Elements::BoolOption("Transaction Error", &g_ProtectionScript->m_TransactionError, ImVec2(445.f, 270.f));
			Elements::BoolOption("Vehicle Kick", &g_ProtectionScript->m_VehicleKick, ImVec2(310.f, 300.f));
			Elements::BoolOption("Bail Kick", &g_ProtectionScript->m_BailKick, ImVec2(445.f, 300.f));
			Elements::BoolOption("Force Activity", &g_ProtectionScript->m_ForceActivity, ImVec2(310.f, 330.f));
			Elements::BoolOption("Null Kick", &g_ProtectionScript->m_NullKick, ImVec2(445.f, 330.f));
			Elements::BoolOption("SMS", &g_ProtectionScript->m_SMS, ImVec2(310.f, 360.f));
			Elements::BoolOption("Interior Kick", &g_ProtectionScript->m_InteriorKick, ImVec2(445.f, 360.f));
			Elements::BoolOption("TSE Crash", &g_ProtectionScript->m_TSECrash, ImVec2(310.f, 390.f));
			Elements::BoolOption("Notify##2", &g_ProtectionScript->m_ScriptEventsNotify, ImVec2(445.f, 390.f));

			Elements::SubOption("Increment Events", ImVec2(290.f, 205.f), ImVec2(0.f, 230.f));
			Elements::BoolOption("Bad Crew Status", &g_ProtectionScript->m_BadCrewStatus, ImVec2(10.f, 260.f));
			Elements::BoolOption("Bad Crew Motto", &g_ProtectionScript->m_BadCrewMotto, ImVec2(145.f, 260.f));
			Elements::BoolOption("Bad Crew Name", &g_ProtectionScript->m_BadCrewName, ImVec2(10.f, 290.f));
			Elements::BoolOption("Bad Crew Emblem", &g_ProtectionScript->m_BadCrewEmblem, ImVec2(145.f, 290.f));
			Elements::BoolOption("Exploits", &g_ProtectionScript->m_Exploits, ImVec2(10.f, 320.f));
			Elements::BoolOption("Gamer Exploits", &g_ProtectionScript->m_GameExploits, ImVec2(145.f, 320.f));
			Elements::BoolOption("Chat Annoying", &g_ProtectionScript->m_ChatAnnoying, ImVec2(10.f, 350.f));
			Elements::BoolOption("Chat Hate", &g_ProtectionScript->m_ChatHate, ImVec2(145.f, 350.f));
			Elements::BoolOption("Voice Annoying", &g_ProtectionScript->m_VoiceAnnoying, ImVec2(10.f, 380.f));
			Elements::BoolOption("Voice Hate", &g_ProtectionScript->m_VoiceHate, ImVec2(145.f, 380.f));
			Elements::BoolOption("Notify##3", &g_ProtectionScript->m_IncrementEventsNotify, ImVec2(10.f, 410.f));

			Elements::SubOption("Manual Patches", ImVec2(290.f, 145.f), ImVec2(300.f, 440.f));
			Elements::BoolOption("Sweep Crash", &g_ProtectionScript->m_SweepCrash, ImVec2(310.f, 470.f));
			Elements::BoolOption("Fragment Crash", &g_ProtectionScript->m_FragmentCrash, ImVec2(445.f, 470.f));
			Elements::BoolOption("Constraint Crash", &g_ProtectionScript->m_ConstraintCrash, ImVec2(310.f, 500.f));
			Elements::BoolOption("Invalid Decal Crash", &g_ProtectionScript->m_InvalidDecalCrash, ImVec2(445.f, 500.f));
			Elements::BoolOption("Infinite Train Crash", &g_ProtectionScript->m_InfiniteTrainCrash, ImVec2(310.f, 530.f));
			Elements::BoolOption("BDSM Crash", &g_ProtectionScript->m_BDSMCrash, ImVec2(445.f, 530.f));
			Elements::BoolOption("Kanye Crash", &g_ProtectionScript->m_KanyeCrash, ImVec2(310.f, 560.f));
			Elements::BoolOption("Notify##4", &g_ProtectionScript->m_ManualPatchesNotify, ImVec2(445.f, 560.f));

			Elements::SubOption("Miscellaneous", ImVec2(290.f, 205.f), ImVec2(0.f, 460.f));
			Elements::BoolOption("Pickups", &g_ProtectionScript->m_Pickups, ImVec2(10.f, 490.f));
			Elements::BoolOption("Block RID Join", &g_ProtectionScript->m_BlockRIDJoin, ImVec2(145.f, 490.f));
			Elements::BoolOption("Block Bad Syncs", &g_ProtectionScript->m_BlockMaliciousSyncs, ImVec2(10.f, 520.f));
			Elements::BoolOption("Disable Transaction Error", &g_ProtectionScript->m_DisableTransactionError, ImVec2(10.f, 550.f));
			if (Elements::BoolOption("Script Host Kick", &g_ProtectionScript->m_ScriptHostKick, ImVec2(10.f, 580.f)))
				g_ScriptQueue->Queue([] { ScriptPatches::RegisterScriptPatches(); });
			if (Elements::BoolOption("Session End Kick", &g_ProtectionScript->m_EndSessionKick, ImVec2(10.f, 610.f)))
				g_ScriptQueue->Queue([] { ScriptPatches::RegisterScriptPatches(); });
			Elements::BoolOption("Anti Cheat Events", &g_ProtectionScript->m_AntiCheatEvents, ImVec2(145.f, 640.f));
			Elements::BoolOption("Notify##5", &g_ProtectionScript->m_MiscellaneousNotify, ImVec2(145.f, 640.f));

			Elements::SubOption("Data Nodes", ImVec2(290.f, 385.f), ImVec2(300.f, 610.f));
			Elements::BoolOption("Invalid Vehicles", &g_ProtectionScript->m_InvalidVehicles, ImVec2(310.f, 640.f));
			Elements::BoolOption("Invalid Door Model", &g_ProtectionScript->m_InvalidDoorModel, ImVec2(310.f, 670.f));
			Elements::BoolOption("Invalid Pickup Model", &g_ProtectionScript->m_InvalidPickupModel, ImVec2(310.f, 700.f));
			Elements::BoolOption("Invalid Attachment", &g_ProtectionScript->m_InvalidAttachment, ImVec2(310.f, 730.f));
			Elements::BoolOption("Invalid Ped Model", &g_ProtectionScript->m_InvalidPedModel, ImVec2(310.f, 760.f));
			Elements::BoolOption("Invalid Player Model", &g_ProtectionScript->m_InvalidPlayerModel, ImVec2(310.f, 790.f));
			Elements::BoolOption("Invalid Ped Prop Model", &g_ProtectionScript->m_InvalidPedPropModel, ImVec2(310.f, 820.f));
			Elements::BoolOption("Invalid Objects", &g_ProtectionScript->m_InvalidObjects, ImVec2(310.f, 850.f));
			Elements::BoolOption("Invalid Sector", &g_ProtectionScript->m_InvalidSector, ImVec2(310.f, 880.f));
			Elements::BoolOption("Block Cages", &g_ProtectionScript->m_BlockCages, ImVec2(310.f, 910.f));
			Elements::BoolOption("Vehicle Manipulation", &g_ProtectionScript->m_VehicleManipulation, ImVec2(310.f, 940.f));
			Elements::BoolOption("Notify##6", &g_ProtectionScript->m_DataNodesNotify, ImVec2(310.f, 970.f));

			ImGui::Dummy(ImVec2(0.f, 120.f));
			ImGui::Dummy(ImVec2(300.f, 120.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}