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
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/Script/ScriptLocal.hpp"
#include "../../Pointers/Pointers.hpp"
#include "../../Utility/All.hpp"
#include "../../Utility/VehicleSpawner.hpp"

namespace Sentinel
{
	void Submenus::SpawnerSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###spawner", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Spawner", ImVec2(290.f, 175.f), ImVec2(0.f, 0.f));
			Elements::BoolOption("Spawn Upgraded", &SpawnVehicles::g_SpawnUpgraded, ImVec2(10.f, 30.f));
			Elements::BoolOption("Spawn In Vehicle", &SpawnVehicles::g_SpawnInVehicle, ImVec2(145.f, 30.f));
			Elements::BoolOption("Delete Previous", &SpawnVehicles::g_DeletePrevious, ImVec2(10.f, 60.f));
			Elements::BoolOption("Despawn Bypass", &SpawnVehicles::g_DespawnBypass, ImVec2(145.f, 60.f));
			Elements::BoolOption("Prevent Crash", &SpawnVehicles::g_PreventCrash, ImVec2(10.f, 90.f));
			Elements::BoolOption("Fade Animation", &SpawnVehicles::g_FadeAnimation, ImVec2(145.f, 90.f));
			Elements::BoolOption("Spawn Air Vehicles In Air", &SpawnVehicles::g_SpawnInAir, ImVec2(10.f, 120.f));
			if (Elements::RegularOption("Spawn Vehicle", ImVec2(260.f, 25.f), ImVec2(10.f, 150.f)))
				g_ScriptQueue->Queue([] { SpawnVehicles::CreateVehicle(SpawnVehicles::g_SelectedVehicle, true); SpawnVehicles::g_SelectedVehicle = 0; });

			Elements::SubOption("Compacts", ImVec2(290.f, 235.f), ImVec2(300.f, 0.f));
			if (Elements::ListBoxHeaderOption("###compacts", ImVec2(290.f, 225.f), ImVec2(300.f, 25.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_CompactsVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_CompactsVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_CompactsVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_CompactsVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Sedans", ImVec2(290.f, 235.f), ImVec2(0.f, 200.f));
			if (Elements::ListBoxHeaderOption("###sedans", ImVec2(290.f, 225.f), ImVec2(0.f, 225.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_SedansVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_SedansVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_SedansVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_SedansVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("SUVs", ImVec2(290.f, 235.f), ImVec2(300.f, 260.f));
			if (Elements::ListBoxHeaderOption("###suvs", ImVec2(290.f, 225.f), ImVec2(300.f, 285.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_SUVsVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_SUVsVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_SUVsVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_SUVsVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Coupes", ImVec2(290.f, 235.f), ImVec2(0.f, 460.f));
			if (Elements::ListBoxHeaderOption("###coupes", ImVec2(290.f, 225.f), ImVec2(0.f, 485.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_CoupesVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_CoupesVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_CoupesVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_CoupesVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Muscle", ImVec2(290.f, 235.f), ImVec2(300.f, 520.f));
			if (Elements::ListBoxHeaderOption("###muscle", ImVec2(290.f, 225.f), ImVec2(300.f, 545.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_MuscleVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_MuscleVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_MuscleVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_MuscleVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Sports Classics", ImVec2(290.f, 235.f), ImVec2(0.f, 720.f));
			if (Elements::ListBoxHeaderOption("###sports_classics", ImVec2(290.f, 225.f), ImVec2(0.f, 745.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_SportsClassicsVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_SportsClassicsVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_SportsClassicsVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_SportsClassicsVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Sports", ImVec2(290.f, 235.f), ImVec2(300.f, 780.f));
			if (Elements::ListBoxHeaderOption("###sports", ImVec2(290.f, 225.f), ImVec2(300.f, 805.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_SportsVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_SportsVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_SportsVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_SportsVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Super", ImVec2(290.f, 235.f), ImVec2(0.f, 980.f));
			if (Elements::ListBoxHeaderOption("###super", ImVec2(290.f, 225.f), ImVec2(0.f, 1005.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_SuperVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_SuperVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_SuperVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_SuperVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Motorcycles", ImVec2(290.f, 235.f), ImVec2(300.f, 1040.f));
			if (Elements::ListBoxHeaderOption("###motorcycles", ImVec2(290.f, 225.f), ImVec2(300.f, 1065.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_MotorcyclesVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_MotorcyclesVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_MotorcyclesVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_MotorcyclesVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Off-Road", ImVec2(290.f, 235.f), ImVec2(0.f, 1240.f));
			if (Elements::ListBoxHeaderOption("###offroad", ImVec2(290.f, 225.f), ImVec2(0.f, 1265.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_OffRoadVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_OffRoadVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_OffRoadVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_OffRoadVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Industrial", ImVec2(290.f, 235.f), ImVec2(300.f, 1300.f));
			if (Elements::ListBoxHeaderOption("###industrial", ImVec2(290.f, 225.f), ImVec2(300.f, 1325.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_IndustrialVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_IndustrialVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_IndustrialVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_IndustrialVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Utility", ImVec2(290.f, 235.f), ImVec2(0.f, 1500.f));
			if (Elements::ListBoxHeaderOption("###utility", ImVec2(290.f, 225.f), ImVec2(0.f, 1525.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_UtilityVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_UtilityVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_UtilityVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_UtilityVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Vans", ImVec2(290.f, 235.f), ImVec2(300.f, 1560.f));
			if (Elements::ListBoxHeaderOption("###vans", ImVec2(290.f, 225.f), ImVec2(300.f, 1585.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_VansVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_VansVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_VansVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_VansVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Cycles", ImVec2(290.f, 235.f), ImVec2(0.f, 1760.f));
			if (Elements::ListBoxHeaderOption("###cycles", ImVec2(290.f, 225.f), ImVec2(0.f, 1785.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_CyclesVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_CyclesVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_CyclesVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_CyclesVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Boats", ImVec2(290.f, 235.f), ImVec2(300.f, 1820.f));
			if (Elements::ListBoxHeaderOption("###boats", ImVec2(290.f, 225.f), ImVec2(300.f, 1845.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_BoatsVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_BoatsVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_BoatsVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_BoatsVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Helicopters", ImVec2(290.f, 235.f), ImVec2(0.f, 2020.f));
			if (Elements::ListBoxHeaderOption("###helicopters", ImVec2(290.f, 225.f), ImVec2(0.f, 2045.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_HelicoptersVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_HelicoptersVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_HelicoptersVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_HelicoptersVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Planes", ImVec2(290.f, 235.f), ImVec2(300.f, 2080.f));
			if (Elements::ListBoxHeaderOption("###planes", ImVec2(290.f, 225.f), ImVec2(300.f, 2105.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_PlanesVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_PlanesVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_PlanesVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_PlanesVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Service", ImVec2(290.f, 235.f), ImVec2(0.f, 2280.f));
			if (Elements::ListBoxHeaderOption("###service", ImVec2(290.f, 225.f), ImVec2(0.f, 2305.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_ServiceVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_ServiceVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_ServiceVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_ServiceVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Emergency", ImVec2(290.f, 235.f), ImVec2(300.f, 2340.f));
			if (Elements::ListBoxHeaderOption("###emergency", ImVec2(290.f, 225.f), ImVec2(300.f, 2365.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_EmergencyVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_EmergencyVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_EmergencyVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_EmergencyVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Military", ImVec2(290.f, 235.f), ImVec2(0.f, 2540.f));
			if (Elements::ListBoxHeaderOption("###military", ImVec2(290.f, 225.f), ImVec2(0.f, 2565.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_MilitaryVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_MilitaryVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_MilitaryVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_MilitaryVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Commercial", ImVec2(290.f, 235.f), ImVec2(300.f, 2600.f));
			if (Elements::ListBoxHeaderOption("###commercial", ImVec2(290.f, 225.f), ImVec2(300.f, 2625.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_CommercialVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_CommercialVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_CommercialVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_CommercialVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Trains", ImVec2(290.f, 235.f), ImVec2(0.f, 2800.f));
			if (Elements::ListBoxHeaderOption("###trains", ImVec2(290.f, 225.f), ImVec2(0.f, 2825.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_TrainsVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_TrainsVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_TrainsVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_TrainsVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}

			Elements::SubOption("Open Wheel", ImVec2(290.f, 175.f), ImVec2(300.f, 2860.f));
			if (Elements::ListBoxHeaderOption("###open_wheel", ImVec2(290.f, 225.f), ImVec2(300.f, 2885.f)))
			{
				for (int Iterator = 0; Iterator < SpawnVehicles::g_OpenWheelVector.size(); Iterator++)
				{
					if (ImGui::Selectable(SpawnVehicles::g_OpenWheelVector[Iterator].m_Name.c_str(), SpawnVehicles::g_SelectedVehicle == SpawnVehicles::g_OpenWheelVector[Iterator].m_Model))
						SpawnVehicles::g_SelectedVehicle = SpawnVehicles::g_OpenWheelVector[Iterator].m_Model;
				}
				ImGui::ListBoxFooter();
			}
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}