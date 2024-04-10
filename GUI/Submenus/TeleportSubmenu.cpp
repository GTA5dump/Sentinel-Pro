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

namespace Sentinel
{
	void Submenus::TeleportSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###teleport", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Teleportation Settings", ImVec2(290.f, 85.f), ImVec2(0.f, 0.f));
			ImGui::SetCursorPos(ImVec2(10.f, 30.f));
			ImGui::Text("Transition");
			ImGui::SetCursorPos(ImVec2(98.f, 30.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###teleportation_transition", &Teleport::g_TransitionPos, Teleport::g_Transition, IM_ARRAYSIZE(Teleport::g_Transition), -1);
			ImGui::PopItemWidth();
			Elements::BoolOption("Keep Vehicle", &Teleport::g_KeepVehicle, ImVec2(10.f, 60.f));

			Elements::SubOption("Markers", ImVec2(290.f, 85.f), ImVec2(300.f, 0.f));
			if (Elements::RegularOption("Waypoint", ImVec2(260.f, 25.f), ImVec2(310.f, 30.f)))
				g_ScriptQueue->Queue([] { Teleport::LocalToWaypoint(); });
			if (Elements::RegularOption("Objective", ImVec2(260.f, 25.f), ImVec2(310.f, 60.f)))
				g_ScriptQueue->Queue([] { Teleport::LocalToObjective(); });

			Elements::SubOption("Vehicles", ImVec2(290.f, 145.f), ImVec2(0.f, 110.f));
			if (Elements::RegularOption("Nearest", ImVec2(260.f, 25.f), ImVec2(10.f, 140.f)))
				g_ScriptQueue->Queue([] { Teleport::NearestVehicle(false); });
			if (Elements::RegularOption("Nearest As Driver", ImVec2(260.f, 25.f), ImVec2(10.f, 170.f)))
				g_ScriptQueue->Queue([] { Teleport::NearestVehicle(true); });
			if (Elements::RegularOption("Last Vehicle", ImVec2(260.f, 25.f), ImVec2(10.f, 200.f)))
				g_ScriptQueue->Queue([] { Teleport::LastVehicle(); });
			if (Elements::RegularOption("Personal Vehicle", ImVec2(260.f, 25.f), ImVec2(10.f, 230.f)))
				g_ScriptQueue->Queue([] { Teleport::LastVehicle(); });

			Elements::SubOption("Directional", ImVec2(290.f, 235.f), ImVec2(300.f, 110.f));
			ImGui::SetCursorPos(ImVec2(310.f, 140.f));
			ImGui::Text("Distance");
			Elements::SliderFloatOption("###directional_distance", &Teleport::g_DirectionalDistance, 1.f, 100.f, 155.f, ImVec2(402.5f, 143.f));
			if (Elements::RegularOption("Forwards", ImVec2(260.f, 25.f), ImVec2(310.f, 170.f)))
				g_ScriptQueue->Queue([] { Teleport::DirectionalTeleport(0); });
			if (Elements::RegularOption("Backwards", ImVec2(260.f, 25.f), ImVec2(310.f, 200.f)))
				g_ScriptQueue->Queue([] { Teleport::DirectionalTeleport(1); });
			if (Elements::RegularOption("Above", ImVec2(260.f, 25.f), ImVec2(310.f, 230.f)))
				g_ScriptQueue->Queue([] { Teleport::DirectionalTeleport(2); });
			if (Elements::RegularOption("Below", ImVec2(260.f, 25.f), ImVec2(310.f, 260.f)))
				g_ScriptQueue->Queue([] { Teleport::DirectionalTeleport(3); });
			if (Elements::RegularOption("Left", ImVec2(260.f, 25.f), ImVec2(310.f, 290.f)))
				g_ScriptQueue->Queue([] { Teleport::DirectionalTeleport(4); });
			if (Elements::RegularOption("Right", ImVec2(260.f, 25.f), ImVec2(310.f, 320.f)))
				g_ScriptQueue->Queue([] { Teleport::DirectionalTeleport(5); });


			ImGui::Dummy(ImVec2(0.f, 70.f));
			ImGui::Dummy(ImVec2(300.f, 70.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}