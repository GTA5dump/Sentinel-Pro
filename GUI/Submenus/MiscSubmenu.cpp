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
#include "../../Features/Misc.hpp"
#include "../../Utility/All.hpp"

namespace Sentinel
{
	void Submenus::MiscSubmenu()
	{
		ImGui::PushFont(Renderer::RobotoMedium());
		if (ImGui::BeginChild("###misc", ImVec2(606.f, 610.f)))
		{
			Elements::SubOption("Sky VFX", ImVec2(290.f, 115.f), ImVec2(0.f, 0.f));
			ImGui::SetCursorPos(ImVec2(10.f, 30.f));
			ImGui::Text("Quadrant");
			ImGui::SetCursorPos(ImVec2(98.f, 30.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###sky_vfx_quadrant", &g_MiscFeatures->m_SkyVFXQuadrantPos, g_MiscFeatures->m_SkyVFXQuadrant, IM_ARRAYSIZE(g_MiscFeatures->m_SkyVFXQuadrant), -1);
			ImGui::PopItemWidth();
			ImGui::SetCursorPos(ImVec2(10.f, 60.f));
			ImGui::Text("Intensity");
			Elements::SliderFloatOption("###sky_intensity", &g_MiscFeatures->m_SkyVFXIntensity[g_MiscFeatures->m_SkyVFXQuadrantPos], 0.f, 10.f, 180.f, ImVec2(90.f, 60.f));
			Elements::ColorEditOption("Color Picker##1", (float*)&g_MiscFeatures->m_SkyVFXColor[g_MiscFeatures->m_SkyVFXQuadrantPos], ImVec2(10.f, 90.f));
			Elements::BoolOption("Rainbow##1", &g_MiscFeatures->m_RainbowSkyVFX[g_MiscFeatures->m_SkyVFXQuadrantPos], ImVec2(120.f, 90.f));
			Elements::BoolOption("Enable##1", &g_MiscFeatures->m_EnableSkyVFX[g_MiscFeatures->m_SkyVFXQuadrantPos], ImVec2(205.f, 90.f));

			Elements::SubOption("VFX Modifiers", ImVec2(290.f, 445.f), ImVec2(300.f, 0.f));
			ImGui::SetCursorPos(ImVec2(310.f, 30.f));
			ImGui::Text("Saturation");
			Elements::SliderFloatOption("###vfx_saturation", &Pointers::pVisualVFX->m_saturation, 0.f, 100.f, 180.f, ImVec2(380.f, 30.f));
			ImGui::SetCursorPos(ImVec2(310.f, 60.f));
			ImGui::Text("Sky Intensity");
			Elements::SliderFloatOption("###vfx_sky_intensity", &Pointers::pVisualVFX->m_sky_intensity, 0.f, 100.f, 180.f, ImVec2(380.f, 60.f));
			ImGui::SetCursorPos(ImVec2(310.f, 90.f));
			ImGui::Text("Shadow Light");
			Elements::SliderFloatOption("###vfx_shadow_light", &Pointers::pVisualVFX->m_shadow_light, 0.f, 100.f, 180.f, ImVec2(380.f, 90.f));
			ImGui::SetCursorPos(ImVec2(310.f, 120.f));
			ImGui::Text("Reflection");
			Elements::SliderFloatOption("###vfx_reflection", &Pointers::pVisualVFX->m_reflection, 0.f, 100.f, 180.f, ImVec2(380.f, 120.f));
			ImGui::SetCursorPos(ImVec2(310.f, 150.f));
			ImGui::Text("Gamma");
			Elements::SliderFloatOption("###vfx_gamma", &Pointers::pVisualVFX->m_gamma, 0.f, 100.f, 180.f, ImVec2(380.f, 150.f));
			ImGui::SetCursorPos(ImVec2(310.f, 180.f));
			ImGui::Text("Blur");
			Elements::SliderFloatOption("###vfx_blur_1", &Pointers::pVisualVFX->m_blur, 0.f, 100.f, 180.f, ImVec2(380.f, 180.f));
			ImGui::SetCursorPos(ImVec2(310.f, 210.f));
			ImGui::Text("Motion Blur");
			Elements::SliderFloatOption("###vfx_blur_2", &Pointers::pVisualVFX->m_blur2, 0.f, 100.f, 180.f, ImVec2(380.f, 210.f));
			ImGui::SetCursorPos(ImVec2(310.f, 240.f));
			ImGui::Text("Rain Intensity");
			if (Elements::SliderFloatOption("###vfx_rain_intensity", &g_MiscFeatures->m_RainIntensity, 0.f, 100.f, 180.f, ImVec2(380.f, 240.f)))
				g_ScriptQueue->Queue([] { MISC::SET_RAIN(g_MiscFeatures->m_RainIntensity); });
			ImGui::SetCursorPos(ImVec2(310.f, 270.f));
			ImGui::Text("Wave Intensity");
			if (Elements::SliderFloatOption("###vfx_wave_intensity", &g_MiscFeatures->m_WaveIntensity, 0.f, 100.f, 180.f, ImVec2(380.f, 270.f)))
				g_ScriptQueue->Queue([] { WATER::SET_DEEP_OCEAN_SCALER(g_MiscFeatures->m_WaveIntensity); });
			ImGui::SetCursorPos(ImVec2(310.f, 300.f));
			ImGui::Text("Wind Speed");
			if (Elements::SliderFloatOption("###vfx_wind_speed", &g_MiscFeatures->m_WindSpeed, 0.f, 100.f, 180.f, ImVec2(380.f, 300.f)))
				g_ScriptQueue->Queue([] { MISC::SET_WIND_SPEED(g_MiscFeatures->m_WindSpeed); });
			ImGui::SetCursorPos(ImVec2(310.f, 330.f));
			ImGui::Text("Wind Dir");
			if (Elements::SliderFloatOption("###vfx_wind_direction", &g_MiscFeatures->m_WindDirection, 0.f, 360.f, 180.f, ImVec2(380.f, 330.f)))
				g_ScriptQueue->Queue([] { MISC::SET_WIND_DIRECTION(g_MiscFeatures->m_WindDirection); });
			Elements::BoolOption("Fog", &g_MiscFeatures->m_EnableFogOnMap, ImVec2(310.f, 360.f));
			Elements::SliderFloatOption("###vfx_fog_on_map", &g_MiscFeatures->m_FogOnMap, -1.f, 1.f, 180.f, ImVec2(380.f, 363.f));
			Elements::BoolOption("Water", &g_MiscFeatures->m_EnableWaterOnMap, ImVec2(310.f, 390.f));
			Elements::SliderFloatOption("###vfx_water_on_map", &g_MiscFeatures->m_WaterOnMap, 0.f, 1.f, 180.f, ImVec2(380.f, 393.f));
			if (Elements::RegularOption("Reset All", ImVec2(260.f, 25.f), ImVec2(310.f, 420.f)))
			{
				g_ScriptQueue->Queue([]
				{
					Pointers::pVisualVFX->m_saturation = 1.f;
					Pointers::pVisualVFX->m_sky_intensity = 1.f;
					Pointers::pVisualVFX->m_shadow_light = 1.f;
					Pointers::pVisualVFX->m_reflection = 1.f;
					Pointers::pVisualVFX->m_gamma = 1.f;
					Pointers::pVisualVFX->m_blur = 1.f;
					Pointers::pVisualVFX->m_blur2 = 1.f;
					g_MiscFeatures->m_RainIntensity = g_MiscFeatures->m_RainIntensityCache;
					MISC::SET_RAIN(g_MiscFeatures->m_RainIntensityCache);
					g_MiscFeatures->m_WaveIntensity = g_MiscFeatures->m_WaveIntensityCache;
					WATER::SET_DEEP_OCEAN_SCALER(g_MiscFeatures->m_WaveIntensityCache);
					g_MiscFeatures->m_WindSpeed = g_MiscFeatures->m_WindSpeedCache;
					MISC::SET_WIND_SPEED(g_MiscFeatures->m_WindSpeedCache);
					g_MiscFeatures->m_WindDirection = g_MiscFeatures->m_WindDirectionCache;
					MISC::SET_WIND_DIRECTION(g_MiscFeatures->m_WindDirectionCache);
					g_MiscFeatures->m_FogOnMap = g_MiscFeatures->m_FogOnMapCache;
					*(float*)Pointers::pFOGOnMap = g_MiscFeatures->m_FogOnMapCache;
					g_MiscFeatures->m_WaterOnMap = g_MiscFeatures->m_WaterOnMapCache;
					*(float*)Pointers::pWaterOnMap = g_MiscFeatures->m_WaterOnMapCache;
				});
			}

			Elements::SubOption("Field Of View Changer", ImVec2(290.f, 75.f), ImVec2(0.f, 140.f));
			ImGui::SetCursorPos(ImVec2(10.f, 170.f));
			ImGui::Text("Value");
			Elements::SliderIntOption("###field_of_view_camera_value", &g_MiscFeatures->m_FieldOfView, 1.f, 120.f, 180.f, ImVec2(80.f, 170.f));
			Elements::BoolOption("Enable##2", &g_MiscFeatures->m_EnableFieldOfView, ImVec2(10.f, 200.f));

			Elements::SubOption("Aerial Camera", ImVec2(290.f, 75.f), ImVec2(300.f, 470.f));
			ImGui::SetCursorPos(ImVec2(310.f, 500.f));
			ImGui::Text("Distance");
			Elements::SliderIntOption("###aerial_camera_distance", &g_MiscFeatures->m_AerialCamerDistance, 0, 300, 180.f, ImVec2(380.f, 500.f));
			Elements::BoolOption("Enable##3", &g_MiscFeatures->m_AerialCamera, ImVec2(310.f, 530.f));

			Elements::SubOption("World Luminance", ImVec2(290.f, 115.f), ImVec2(0.f, 240.f));
			ImGui::SetCursorPos(ImVec2(10.f, 270.f));
			ImGui::Text("Range");
			Elements::SliderFloatOption("###world_luminance_range", &g_MiscFeatures->m_WorldLuminanceRange, 0.f, 500.f, 180.f, ImVec2(80.f, 270.f));
			ImGui::SetCursorPos(ImVec2(10.f, 300.f));
			ImGui::Text("Intensity");
			Elements::SliderFloatOption("###world_luminance_intensity", &g_MiscFeatures->m_WorldLuminanceIntensity, 0.f, 500.f, 180.f, ImVec2(80.f, 300.f));
			Elements::ColorEditOption("Color Picker##2", (float*)&g_MiscFeatures->m_WorldLuminanceColor, ImVec2(10.f, 330.f));
			Elements::BoolOption("Rainbow##2", &g_MiscFeatures->m_WorldLuminanceRainbow, ImVec2(120.f, 330.f));
			Elements::BoolOption("Enable##4", &g_MiscFeatures->m_EnableWorldLuminance, ImVec2(205.f, 330.f));

			Elements::SubOption("Weather Manager", ImVec2(290.f, 75.f), ImVec2(300.f, 570.f));
			ImGui::SetCursorPos(ImVec2(310.f, 600.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(398.f, 600.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###weather_type_asset", &g_MiscFeatures->m_WeatherTypesPos, g_MiscFeatures->m_WeatherTypes, IM_ARRAYSIZE(g_MiscFeatures->m_WeatherTypes), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Set Persistent", ImVec2(260.f, 25.f), ImVec2(310.f, 630.f)))
				g_ScriptQueue->Queue([] { MISC::CLEAR_OVERRIDE_WEATHER(); MISC::SET_WEATHER_TYPE_NOW_PERSIST(Lists::g_WeatherTypes[g_MiscFeatures->m_WeatherTypesPos].c_str()); });

			Elements::SubOption("Blackhole", ImVec2(290.f, 205.f), ImVec2(0.f, 380.f));
			ImGui::SetCursorPos(ImVec2(10.f, 410.f));
			ImGui::Text("X");
			Elements::SliderFloatOption("###black_hole_pos_x", &g_MiscFeatures->m_BlackHoleX, 0.f, 200.f, 180.f, ImVec2(80.f, 410.f));
			ImGui::SetCursorPos(ImVec2(10.f, 440.f));
			ImGui::Text("Y");
			Elements::SliderFloatOption("###black_hole_pos_y", &g_MiscFeatures->m_BlackHoleY, 0.f, 200.f, 180.f, ImVec2(80.f, 440.f));
			ImGui::SetCursorPos(ImVec2(10.f, 470.f));
			ImGui::Text("Z");
			Elements::SliderFloatOption("###black_hole_pos_z", &g_MiscFeatures->m_BlackHoleZ, 0.f, 200.f, 180.f, ImVec2(80.f, 470.f));
			if (Elements::RegularOption("Set To Me", ImVec2(260.f, 25.f), ImVec2(10.f, 500.f)))
			{
				g_ScriptQueue->Queue([] 
				{
					const auto Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE);
					g_MiscFeatures->m_BlackHoleX = Coords.x; g_MiscFeatures->m_BlackHoleY = Coords.y; g_MiscFeatures->m_BlackHoleZ = Coords.z;
				});
			}
			Elements::BoolOption("Peds", &g_MiscFeatures->m_BlackHolePeds, ImVec2(10.f, 530.f));
			Elements::BoolOption("Vehicles", &g_MiscFeatures->m_BlackHoleVehicles, ImVec2(145.f, 530.f));
			Elements::BoolOption("Objects", &g_MiscFeatures->m_BlackHoleObjects, ImVec2(10.f, 560.f));
			Elements::BoolOption("Enable##5", &g_MiscFeatures->m_EnableBlackHole, ImVec2(145.f, 560.f));

			Elements::SubOption("Cloud Manager", ImVec2(290.f, 75.f), ImVec2(300.f, 670.f));
			ImGui::SetCursorPos(ImVec2(310.f, 700.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(398.f, 700.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###cloud_type_asset", &g_MiscFeatures->m_CloudTypesPos, g_MiscFeatures->m_CloudTypes, IM_ARRAYSIZE(g_MiscFeatures->m_CloudTypes), -1);
			ImGui::PopItemWidth();
			if (Elements::RegularOption("Set Hat", ImVec2(260.f, 25.f), ImVec2(310.f, 730.f)))
				g_ScriptQueue->Queue([] { MISC::LOAD_CLOUD_HAT(Lists::g_CloudTypes[g_MiscFeatures->m_CloudTypesPos].c_str(), 0.5f); });

			Elements::SubOption("Time Manager", ImVec2(290.f, 175.f), ImVec2(0.f, 605.f));
			ImGui::SetCursorPos(ImVec2(10.f, 635.f));
			ImGui::Text("Hour");
			Elements::SliderIntOption("###time_hour", &g_MiscFeatures->m_TimeManagerHour, 0, 23, 180.f, ImVec2(80.f, 635.f));
			ImGui::SetCursorPos(ImVec2(10.f, 665.f));
			ImGui::Text("Minute");
			Elements::SliderIntOption("###time_minute", &g_MiscFeatures->m_TimeManagerMinute, 0, 59, 180.f, ImVec2(80.f, 665.f));
			ImGui::SetCursorPos(ImVec2(10.f, 695.f));
			ImGui::Text("Second");
			Elements::SliderIntOption("###time_second", &g_MiscFeatures->m_TimeManagerSecond, 0, 59, 180.f, ImVec2(80.f, 695.f));
			if (Elements::RegularOption("Set Time", ImVec2(260.f, 25.f), ImVec2(10.f, 725.f)))
				g_ScriptQueue->Queue([] { NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(g_MiscFeatures->m_TimeManagerHour, g_MiscFeatures->m_TimeManagerMinute, g_MiscFeatures->m_TimeManagerSecond); });
			if (Elements::BoolOption("Pause Clock", &g_MiscFeatures->m_FreezeTime, ImVec2(10.f, 755.f)))
				g_ScriptQueue->Queue([] { if (!g_MiscFeatures->m_FreezeTime) CLOCK::PAUSE_CLOCK(FALSE); });
			if (Elements::BoolOption("Sync Time", &g_MiscFeatures->m_SyncTime, ImVec2(145.f, 755.f)))
				g_ScriptQueue->Queue([] { if (!g_MiscFeatures->m_SyncTime) CLOCK::PAUSE_CLOCK(FALSE); });

			Elements::SubOption("HUD Color Manager", ImVec2(290.f, 75.f), ImVec2(300.f, 770.f));
			ImGui::SetCursorPos(ImVec2(310.f, 800.f));
			ImGui::Text("Color");
			ImGui::SetCursorPos(ImVec2(398.f, 800.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###hud_color_manager_selector", &g_MiscFeatures->m_HUDColorsPos, g_MiscFeatures->m_HUDColors, IM_ARRAYSIZE(g_MiscFeatures->m_HUDColors), -1);
			ImGui::PopItemWidth();
			if (Elements::ColorEditOption("Color Picker##3", (float*)&g_MiscFeatures->m_ModifyHUDColor[g_MiscFeatures->m_HUDColorsPos], ImVec2(310.f, 830.f)))
				g_ScriptQueue->Queue([] { HUD::REPLACE_HUD_COLOUR_WITH_RGBA(g_MiscFeatures->m_HUDColorsPos, g_MiscFeatures->m_ModifyHUDColor[g_MiscFeatures->m_HUDColorsPos][0] * 255.f, g_MiscFeatures->m_ModifyHUDColor[g_MiscFeatures->m_HUDColorsPos][1] * 255.f, g_MiscFeatures->m_ModifyHUDColor[g_MiscFeatures->m_HUDColorsPos][2] * 255.f, 255); });

			Elements::SubOption("Disables", ImVec2(290.f, 385.f), ImVec2(0.f, 805.f));
			if (Elements::BoolOption("Disable Anti Afk Kick", &g_MiscFeatures->m_DisableAntiAfkKick, ImVec2(10.f, 835.f)))
				ScriptPatches::RegisterScriptPatches();
			if (Elements::BoolOption("Disable Population Load Balancing", &g_MiscFeatures->m_DisablePopulationLoadBalancing, ImVec2(10.f, 865.f)))
				ScriptPatches::RegisterScriptPatches();
			if (Elements::BoolOption("Disable Underneath Map Death", &g_MiscFeatures->m_DisableUnderneathMapDeath, ImVec2(10.f, 895.f)))
				ScriptPatches::RegisterScriptPatches();
			Elements::BoolOption("Disable Phone", &g_MiscFeatures->m_DisablePhone, ImVec2(10.f, 925.f));
			Elements::BoolOption("Disable Phone Calls", &g_MiscFeatures->m_DisablePhoneCalls, ImVec2(145.f, 925.f));
			if (Elements::BoolOption("Disable HUD", &g_MiscFeatures->m_DisableHUD, ImVec2(10.f, 955.f)))
				g_ScriptQueue->Queue([] { if (!g_MiscFeatures->m_DisableHUD) HUD::DISPLAY_HUD(TRUE); });
			if (Elements::BoolOption("Disable Mini Map", &g_MiscFeatures->m_DisableMiniMap, ImVec2(145.f, 955.f)))
				g_ScriptQueue->Queue([] { if (!g_MiscFeatures->m_DisableMiniMap) HUD::DISPLAY_RADAR(TRUE); });
			Elements::BoolOption("Disable Help Messages", &g_MiscFeatures->m_DisableHelpMessages, ImVec2(10.f, 985.f));
			Elements::BoolOption("Disable Map Messages", &g_MiscFeatures->m_DisableMapMessages, ImVec2(10.f, 1015.f));
			Elements::BoolOption("Disable World Boundaries", &g_MiscFeatures->m_DisableWorldBoundaries, ImVec2(10.f, 1045.f));
			Elements::BoolOption("Disable Loading Prompts", &g_MiscFeatures->m_DisableLoadingPrompts, ImVec2(10.f, 1075.f));
			Elements::BoolOption("Disable Cinematic Camera", &g_MiscFeatures->m_DisableCinematicCamera, ImVec2(10.f, 1105.f));
			Elements::BoolOption("Disable Afk Camera", &g_MiscFeatures->m_DisableAFKCamera, ImVec2(10.f, 1135.f));
			Elements::BoolOption("Disable Cutscenes", &g_MiscFeatures->m_DisableCutscenes, ImVec2(10.f, 1165.f));
			Elements::BoolOption("Disable Radio", &g_MiscFeatures->m_DisableRadio, ImVec2(145.f, 1165.f));

			Elements::SubOption("Dispatch Service", ImVec2(290.f, 385.f), ImVec2(300.f, 870.f));
			if (Elements::BoolOption("Disable Police Automobile", &g_MiscFeatures->m_DisablePoliceAutomobile, ImVec2(310.f, 900.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_PoliceAutomobile, g_MiscFeatures->m_DisablePoliceAutomobile); });
			if (Elements::BoolOption("Disable Police Helicopter", &g_MiscFeatures->m_DisablePoliceHelicopter, ImVec2(310.f, 930.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_PoliceHelicopter, g_MiscFeatures->m_DisablePoliceHelicopter); });
			if (Elements::BoolOption("Disable Police Riders", &g_MiscFeatures->m_DisablePoliceRiders, ImVec2(310.f, 960.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_PoliceRiders, g_MiscFeatures->m_DisablePoliceRiders); });
			if (Elements::BoolOption("Disable Police Vehicle Request", &g_MiscFeatures->m_DisablePoliceVehicleRequest, ImVec2(310.f, 990.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_PoliceVehicleRequest, g_MiscFeatures->m_DisablePoliceVehicleRequest); });
			if (Elements::BoolOption("Disable Police Road Block", &g_MiscFeatures->m_DisablePoliceRoadblock, ImVec2(310.f, 1020.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_PoliceRoadBlock, g_MiscFeatures->m_DisablePoliceRoadblock); });
			if (Elements::BoolOption("Disable Police Boat", &g_MiscFeatures->m_DisablePoliceBoat, ImVec2(310.f, 1050.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_PoliceBoat, g_MiscFeatures->m_DisablePoliceBoat); });
			if (Elements::BoolOption("Disable SWAT Automobile", &g_MiscFeatures->m_DisablSwatAutomobile, ImVec2(310.f, 1080.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_SwatAutomobile, g_MiscFeatures->m_DisablSwatAutomobile); });
			if (Elements::BoolOption("Disable SWAT Helicopter", &g_MiscFeatures->m_DisableSwatHelicopter, ImVec2(310.f, 1110.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_SwatHelicopter, g_MiscFeatures->m_DisableSwatHelicopter); });
			if (Elements::BoolOption("Disable Ambulance Department", &g_MiscFeatures->m_DisableAmbulanceDepartment, ImVec2(310.f, 1140.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_AmbulanceDepartment, g_MiscFeatures->m_DisableAmbulanceDepartment); });
			if (Elements::BoolOption("Disable Fire Department", &g_MiscFeatures->m_DisableFireDepartment, ImVec2(310.f, 1170.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_FireDepartment, g_MiscFeatures->m_DisableFireDepartment); });
			if (Elements::BoolOption("Disable Army Vehicles", &g_MiscFeatures->m_DisableArmyVehicles, ImVec2(310.f, 1200.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_ArmyVehicle, g_MiscFeatures->m_DisableArmyVehicles); });
			if (Elements::BoolOption("Disable Gangs", &g_MiscFeatures->m_DisableGangs, ImVec2(310.f, 1230.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_Gangs, g_MiscFeatures->m_DisableGangs); });
			if (Elements::BoolOption("Disable Bikers", &g_MiscFeatures->m_DisableBikers, ImVec2(445.f, 1230.f)))
				g_ScriptQueue->Queue([] { Local::Dispatch::SetToggle(DT_BikerBackup, g_MiscFeatures->m_DisableBikers); });

			Elements::SubOption("Precipitation", ImVec2(290.f, 985.f), ImVec2(0.f, 1215.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1245.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(98.f, 1245.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###precipitation_type", &g_MiscFeatures->m_PrecipitationTypesPos, g_MiscFeatures->m_PrecipitationTypes, IM_ARRAYSIZE(g_MiscFeatures->m_PrecipitationTypes), -1);
			ImGui::PopItemWidth();
			if (Elements::ColorEditOption("Color Picker##4", (float*)&g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color, ImVec2(10.f, 1275.f)))
			{
				g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_color.x = (g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color[0] * g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance);
				g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_color.y = (g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color[1] * g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance);
				g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_color.z = (g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color[2] * g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance);
			}
			Elements::BoolOption("Rainbow##3", &g_MiscFeatures->m_RainbowPrecipitation[g_MiscFeatures->m_PrecipitationTypesPos], ImVec2(110.f, 1275.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1305.f));
			ImGui::Text("Luminance");
			if (Elements::SliderFloatOption("###precipitation_luminance", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance, -100.f, 100.f, 180.f, ImVec2(90.f, 1305.f)))
			{
				g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_color.x = (g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color[0] * g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance);
				g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_color.y = (g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color[1] * g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance);
				g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_color.z = (g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Color[2] * g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Luminance);
			}
			ImGui::SetCursorPos(ImVec2(10.f, 1335.f));
			ImGui::Text("Size Min X");
			Elements::SliderFloatOption("###size_min_x", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_size_min_max.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1335.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1365.f));
			ImGui::Text("Size Min Y");
			Elements::SliderFloatOption("###size_min_y", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_size_min_max.y, -10.f, 10.f, 180.f, ImVec2(90.f, 1365.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1395.f));
			ImGui::Text("Size Max X");
			Elements::SliderFloatOption("###size_max_x", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_size_min_max.z, -10.f, 10.f, 180.f, ImVec2(90.f, 1395.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1425.f));
			ImGui::Text("Size Max Y");
			Elements::SliderFloatOption("###size_max_y", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_size_min_max.w, -10.f, 10.f, 180.f, ImVec2(90.f, 1425.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1455.f));
			ImGui::Text("Box Centre X");
			Elements::SliderFloatOption("###box_centre_x", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_box_centre_offset.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1455.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1485.f));
			ImGui::Text("Box Centre Y");
			Elements::SliderFloatOption("###box_centre_y", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_box_centre_offset.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1485.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1515.f));
			ImGui::Text("Box Centre Z");
			Elements::SliderFloatOption("###box_centre_z", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_box_centre_offset.z, -100.f, 100.f, 180.f, ImVec2(90.f, 1515.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1545.f));
			ImGui::Text("Box Size X");
			Elements::SliderFloatOption("###box_size_x", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_box_size.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1545.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1575.f));
			ImGui::Text("Box Size Y");
			Elements::SliderFloatOption("###box_size_y", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_box_size.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1575.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1605.f));
			ImGui::Text("Box Size Z");
			Elements::SliderFloatOption("###box_size_z", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_box_size.z, -100.f, 100.f, 180.f, ImVec2(90.f, 1605.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1635.f));
			ImGui::Text("Vel Min X");
			Elements::SliderFloatOption("###velocity_min_x", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_velocity_min.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1635.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1665.f));
			ImGui::Text("Vel Min Y");
			Elements::SliderFloatOption("###velocity_min_y", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_velocity_min.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1665.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1695.f));
			ImGui::Text("Vel Min Z");
			Elements::SliderFloatOption("###velocity_min_z", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_velocity_min.z, -100.f, 100.f, 180.f, ImVec2(90.f, 1695.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1725.f));
			ImGui::Text("Vel Max X");
			Elements::SliderFloatOption("###velocity_max_x", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_velocity_max.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1725.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1755.f));
			ImGui::Text("Vel Max Y");
			Elements::SliderFloatOption("###velocity_max_y", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_velocity_max.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1755.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1785.f));
			ImGui::Text("Vel Max Z");
			Elements::SliderFloatOption("###velocity_max_z", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_velocity_max.z, -100.f, 100.f, 180.f, ImVec2(90.f, 1785.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1815.f));
			ImGui::Text("Life Min");
			Elements::SliderFloatOption("###life_min", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_life_min_max.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1815.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1845.f));
			ImGui::Text("Life Max");
			Elements::SliderFloatOption("###life_max", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_life_min_max.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1845.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1875.f));
			ImGui::Text("Gravity");
			Elements::SliderFloatOption("###gravity", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_gravity, -100.f, 100.f, 180.f, ImVec2(90.f, 1875.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1905.f));
			ImGui::Text("Fade In");
			Elements::SliderFloatOption("###fade_in", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_fade_in_out.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1905.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1935.f));
			ImGui::Text("Fade Out");
			Elements::SliderFloatOption("###fade_out", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_fade_in_out.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1935.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1965.f));
			ImGui::Text("Fade Near");
			Elements::SliderFloatOption("###fade_near", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_fade_near_far.x, -100.f, 100.f, 180.f, ImVec2(90.f, 1965.f));
			ImGui::SetCursorPos(ImVec2(10.f, 1995.f));
			ImGui::Text("Fade Far");
			Elements::SliderFloatOption("###fade_far", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_fade_near_far.y, -100.f, 100.f, 180.f, ImVec2(90.f, 1995.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2025.f));
			ImGui::Text("Edge Softness");
			Elements::SliderFloatOption("###edge_softness", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_edge_softness, -100.f, 100.f, 180.f, ImVec2(90.f, 2025.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2055.f));
			ImGui::Text("PTFX Col %");
			Elements::SliderFloatOption("###particle_color_percentage", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_particle_color_percentage, -100.f, 100.f, 180.f, ImVec2(90.f, 2055.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2085.f));
			ImGui::Text("BG DST %");
			Elements::SliderFloatOption("###background_distortion_visibility_percentage", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_background_distortion_visibility_percentage, -100.f, 100.f, 180.f, ImVec2(90.f, 2085.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2115.f));
			ImGui::Text("BG ALP %");
			Elements::SliderFloatOption("###background_distortion_alpha_booster", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_background_distortion_alpha_booster, -100.f, 100.f, 180.f, ImVec2(90.f, 2115.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2145.f));
			ImGui::Text("BG DST AMT");
			Elements::SliderFloatOption("###background_distortion_amount", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_background_distortion_amount, -100.f, 100.f, 180.f, ImVec2(90.f, 2145.f));
			ImGui::SetCursorPos(ImVec2(10.f, 2175.f));
			ImGui::Text("Lights Mult");
			Elements::SliderFloatOption("###local_lights_multiplier", &g_MiscFeatures->m_NewShaders[g_MiscFeatures->m_PrecipitationTypesPos].m_Drop->m_local_lights_multiplier, -100.f, 100.f, 180.f, ImVec2(90.f, 2175.f));

			Elements::SubOption("Ocean Quadrants", ImVec2(290.f, 75.f), ImVec2(300.f, 1280.f));
			static bool DisableOcean = false;
			if (Elements::BoolOption("Disable Ocean", &DisableOcean, ImVec2(310.f, 1310.f)))
				g_ScriptQueue->Queue([=] { Local::Ocean::NoOcean(DisableOcean); });
			static bool RaiseOcean = false;
			if (Elements::BoolOption("Raise Ocean", &RaiseOcean, ImVec2(445.f, 1310.f)))
				g_ScriptQueue->Queue([=] { Local::Ocean::RaiseOcean(RaiseOcean); });
			static bool TransparentOcean = false;
			if (Elements::BoolOption("Transparent Ocean", &TransparentOcean, ImVec2(310.f, 1340.f)))
				g_ScriptQueue->Queue([=] { Local::Ocean::TransparentOcean(TransparentOcean); });

			Elements::SubOption("Liquid VFX", ImVec2(290.f, 75.f), ImVec2(300.f, 1380.f));
			ImGui::SetCursorPos(ImVec2(310.f, 1410.f));
			ImGui::Text("Type");
			ImGui::SetCursorPos(ImVec2(398.f, 1410.f));
			ImGui::PushItemWidth(180.f);
			Elements::ComboOption("###liquid_vfx_type", &g_MiscFeatures->m_LiquidVFXTypesPos, g_MiscFeatures->m_LiquidVFXTypes, IM_ARRAYSIZE(g_MiscFeatures->m_LiquidVFXTypes), -1);
			ImGui::PopItemWidth();
			if (Elements::ColorEditOption("Color Picker##5", (float*)&g_MiscFeatures->m_LiquidVFXColor[g_MiscFeatures->m_LiquidVFXTypesPos], ImVec2(310.f, 1440.f)))
			{
				Pointers::pLiquidVFX[g_MiscFeatures->m_LiquidVFXTypesPos].r = g_MiscFeatures->m_LiquidVFXColor[g_MiscFeatures->m_LiquidVFXTypesPos][0] * 255.f;
				Pointers::pLiquidVFX[g_MiscFeatures->m_LiquidVFXTypesPos].g = g_MiscFeatures->m_LiquidVFXColor[g_MiscFeatures->m_LiquidVFXTypesPos][1] * 255.f;
				Pointers::pLiquidVFX[g_MiscFeatures->m_LiquidVFXTypesPos].b = g_MiscFeatures->m_LiquidVFXColor[g_MiscFeatures->m_LiquidVFXTypesPos][2] * 255.f;
			}

			ImGui::Dummy(ImVec2(0.f, 750.f));
		}
		ImGui::EndChild();
		ImGui::PopFont();
	}
}