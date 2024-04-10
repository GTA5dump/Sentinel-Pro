#include "../../Common.hpp"
#include "Submenus.hpp"
#include "../GUI.hpp"
#include "../Overlays/Overlays.hpp"
#include "../Elements.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../GTA/GTA.hpp"
#include "../../GTA/enums.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../GTA/Script/ScriptQueue.hpp"
#include "../../GTA/Script/ScriptPatches.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/Script/ScriptLocal.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../Features/Network.hpp"
#include "../../Utility/All.hpp"
#include "../../GTA/Crashes.hpp"

namespace Sentinel
{
	void Submenus::NetworkSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###network", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Miscellaneous Spoofing", ImVec2(290.f, 75.f), ImVec2(0.f, 0.f));
			Elements::BoolOption("Movement Animation", &g_NetworkFeatures->m_EnableMovementSpoofing, ImVec2(10.f, 30.f));
			if (Elements::RegularOption("Crashy Crashy", ImVec2(270.f, 25.f), ImVec2(10.f, 60.f)))
			{
				g_PlayerService->Iterate([](const PlayerStructEntry& player)
				{
					if (g_PlayerService->GetSelf()->GetPed() && player.second->GetPed() && player.second->GetPed()->m_health > 0)
					{
						Crashes::SyncCrash(player.second);
						Crashes::KitCrash(player.second);
						Crashes::PogoCrash(player.second);
						Crashes::ScriptCrash(player.second);

						g_ScriptQueue->Queue([=]
						{
							char ChatMessage[7] = { 'K','w','a','s', 'h', 'y', '\0'};
							const auto Ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player.second->ID());
							if (PED::IS_PED_IN_ANY_VEHICLE(Ped, FALSE))
								Crashes::TaskCrash(player.second);
							System::SendPublicChatMessage(ChatMessage, true);
						});				
					}
				});
			}

			ImGui::Dummy(ImVec2(0.f, 70.f));
			ImGui::Dummy(ImVec2(300.f, 70.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}