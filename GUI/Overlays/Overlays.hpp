#pragma once
#include "../../Common.hpp"
#include "../../GTA/Invoker/NativeTypes.hpp"
#include "../../Utility/Math.hpp"

namespace Sentinel::Overlays
{
	extern void DrawRect(float x, float y, float width, float height, Color color, ImDrawList* drawlist = ImGui::GetBackgroundDrawList());
	extern void DrawLeftText(const char* text, float x, float y, ImFont* font, Color color, ImDrawList* drawlist = ImGui::GetForegroundDrawList());
	extern void DrawRightText(const char* text, float x, float y, ImFont* font, Color color, ImDrawList* drawlist = ImGui::GetForegroundDrawList());
	extern void DrawCenteredText(const char* text, float x, float y, ImFont* font, Color color, ImDrawList* drawlist = ImGui::GetForegroundDrawList());
	extern ImVec2 GetTextScale(const char* text);

	// Notification Manager
	struct Notification
	{
		std::string m_Title;
		std::string m_Content;
		int m_Duration;
		Color m_LineColor;
		int m_CreationTime;
		int m_Alpha;
		bool m_FadeOut;
	}; inline std::vector<Notification> g_Notifications;
	inline Color g_TextColor = { 170, 170, 170, 255 };
	inline Color g_BackgroundColor = { 30, 30, 30, 255 };
	inline std::string g_LastProtectionID = "";

	extern void DrawNotification(const int& length, std::string header, std::string content, Color text, Color line, Color background, int alpha);
	extern void NotificationManager();

	// Implementation:
	inline void PushInfo(std::string title, std::string content, int duration = 5000)
	{
		Notification Notify;
		Notify.m_Title = title;
		Notify.m_Content = content;
		Notify.m_Duration = duration;
		Notify.m_LineColor = { 99, 66, 245, 255 };
		Notify.m_CreationTime = GetTickCount64();
		Notify.m_Alpha = 0;
		Notify.m_FadeOut = false;
		g_Notifications.push_back(Notify);
	}

	inline void PushError(std::string title, std::string content, int duration = 5000)
	{
		Notification Notify;
		Notify.m_Title = title;
		Notify.m_Content = content;
		Notify.m_Duration = duration;
		Notify.m_LineColor = { 245, 66, 99, 255 };
		Notify.m_CreationTime = GetTickCount64();
		Notify.m_Alpha = 0;
		Notify.m_FadeOut = false;
		g_Notifications.push_back(Notify);
	}

	inline void PushWarning(std::string title, std::string content, int duration = 5000)
	{
		Notification Notify;
		Notify.m_Title = title;
		Notify.m_Content = content;
		Notify.m_Duration = duration;
		Notify.m_LineColor = { 245, 235, 51, 255 };
		Notify.m_CreationTime = GetTickCount64();
		Notify.m_Alpha = 0;
		Notify.m_FadeOut = false;
		g_Notifications.push_back(Notify);
	}

	inline void ProtectionMessage(std::string player, std::string protection)
	{
		if (g_LastProtectionID != protection) {
			LOG(WARNING) << "[Protections] " << protection << " has been triggered by " << player << ".";
			PushWarning(std::format("Protection Script", protection), std::format("{} has been triggered by {}.", protection, player));
			g_LastProtectionID = protection;
		}
	}

	inline void ReportMessage(std::string player, std::string report)
	{
		LOG(WARNING) << "[Reports] " << report << " has been blocked from " << player << ".";
		PushWarning(std::format("{} Report", report), std::format("Blocked report from {} for: {}.", player, report));
	}
}