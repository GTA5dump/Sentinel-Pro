#include "Menu.hpp"
#include "../Elements.hpp"
#include "../Submenus/Submenus.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../GTA/Script/ScriptQueue.hpp"
#include "../../Features/Players.hpp"
#include "../../GTA/GTA.hpp"
#include "../../Utility/System.hpp"
#include "../../Utility/Math.hpp"

namespace Sentinel
{          
    void Menu::DXMain()
    {
        if (!GUI::IsOpen())
            return;

		ImGui::SetNextWindowSize(ImVec2(684.f, 660.f), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(g_PosX, g_PosY), ImGuiCond_FirstUseEver);
		ImGui::PushFont(Renderer::Pricedown());
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGui::ColorConvertU32ToFloat4(ImColor(255, 255, 255, 255))));
        if (ImGui::Begin("Sentinel Pro: Open Beta", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImGui::PopStyleColor();
            ImGui::PopFont();
            if (ImGui::BeginChild("###subs", ImVec2(100.f, 610.f)))
            {
                Elements::Gradient(ImVec2(ImGui::GetWindowPos().x + 57.f, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 56.f, ImGui::GetWindowPos().y + 608.f), ImGui::ColorConvertU32ToFloat4(ImColor(80, 80, 80, 255)), ImGui::ColorConvertU32ToFloat4(ImColor(80, 80, 80, 255)));
                ImGui::PushFont(Renderer::Icons());
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertU32ToFloat4(ImColor(50, 50, 50, 255)));
                if (ImGui::TabButton("a", g_CurrentTab == SubmenuSelf, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuSelf; }
                if (ImGui::TabButton("b", g_CurrentTab == SubmenuProtection, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuProtection; }
                if (ImGui::TabButton("c", g_CurrentTab == SubmenuPlayers, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuPlayers; }
                if (ImGui::TabButton("d", g_CurrentTab == SubmenuNetwork, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuNetwork; }
                if (ImGui::TabButton("e", g_CurrentTab == SubmenuRecovery, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuRecovery; }
                if (ImGui::TabButton("f", g_CurrentTab == SubmenuWeapons, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuWeapons; }
                if (ImGui::TabButton("g", g_CurrentTab == SubmenuVehicles, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuVehicles; }
                if (ImGui::TabButton("h", g_CurrentTab == SubmenuTeleport, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuTeleport; }
                if (ImGui::TabButton("i", g_CurrentTab == SubmenuSpawner, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuSpawner; }
                if (ImGui::TabButton("j", g_CurrentTab == SubmenuMisc, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuMisc; }
                if (ImGui::TabButton("k", g_CurrentTab == SubmenuSettings, ImVec2(48.f, 48.f))) { g_CurrentTab = SubmenuSettings; }
                ImGui::PopStyleColor();
                ImGui::PopFont();
            }
            ImGui::EndChild();

            ImGui::SetCursorPos(ImVec2(75.f, 40.f));
            switch (g_CurrentTab)
            {
                case SubmenuSelf:
                {
                    Submenus::SelfSubmenu();
                    break;
                }
                case SubmenuProtection:
                {
                    Submenus::ProtectionSubmenu();
                    break;
                }
                case SubmenuPlayers:
                {
                    Submenus::PlayersSubmenu();
                    break;
                }
                case SubmenuNetwork:
                {
                    Submenus::NetworkSubmenu();
                    break;
                }
                case SubmenuWeapons:
                {
                    Submenus::WeaponSubmenu();
                    break;
                }
                case SubmenuVehicles:
                {
                    Submenus::VehicleSubmenu();
                    break;
                }
                case SubmenuTeleport:
                {
                    Submenus::TeleportSubmenu();
                    break;
                }
                case SubmenuSpawner:
                {
                    Submenus::SpawnerSubmenu();
                    break;
                }
                case SubmenuMisc:
                {
                    Submenus::MiscSubmenu();
                    break;
                }
            }
        }
        ImGui::End();
    }

    void Menu::NativeMain()
    {
		if (GUI::IsOpen())
		{
			for (int i = 0; i <= 6; i++)
				PAD::DISABLE_CONTROL_ACTION(2, i, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 106, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 329, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 330, TRUE);

			PAD::DISABLE_CONTROL_ACTION(2, 14, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 15, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 16, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 17, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 24, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 69, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 70, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 84, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 85, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 99, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 92, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 100, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 114, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 115, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 121, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 142, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 241, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 261, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 257, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 262, TRUE);
			PAD::DISABLE_CONTROL_ACTION(2, 331, TRUE);
            if (GUI::IsTyping())
                PAD::DISABLE_ALL_CONTROL_ACTIONS(0);            
		}

        System::SlowRainbowLoop();
        System::FastRainbowLoop();
    }
}