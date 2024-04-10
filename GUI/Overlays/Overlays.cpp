#include "Overlays.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Utility/Timer.hpp"

namespace Sentinel
{
	void Overlays::DrawRect(float x, float y, float width, float height, Color color, ImDrawList* drawlist)
	{
		auto ImGui = ImGui::GetIO().DisplaySize;

		drawlist->AddRectFilled(
			ImVec2(x * ImGui.x - ((width * ImGui.x) / 2.f), y * ImGui.y - ((height * ImGui.y) / 2.f)),
			ImVec2((x * ImGui.x) + ((width * ImGui.x) / 2.f), (y * ImGui.y) + ((height * ImGui.y) / 2.f)),
			IM_COL32(color.r, color.g, color.b, color.a)
		);
	}

	void Overlays::DrawLeftText(const char* text, float x, float y, ImFont* font, Color color, ImDrawList* drawlist)
	{
		auto ImGui = ImGui::GetIO().DisplaySize;

		ImGui::PushFont(font);
		drawlist->AddText(
			ImVec2(x * ImGui.x, y * ImGui.y),
			IM_COL32(color.r, color.g, color.b, color.a),
			text
		);
		ImGui::PopFont();
	}

	void Overlays::DrawRightText(const char* text, float x, float y, ImFont* font, Color color, ImDrawList* drawlist)
	{
		auto ImGui = ImGui::GetIO().DisplaySize;

		ImGui::PushFont(font);
		drawlist->AddText(
			ImVec2(x * ImGui.x - ImGui::CalcTextSize(text).x, y * ImGui.y),
			IM_COL32(color.r, color.g, color.b, color.a),
			text
		);
		ImGui::PopFont();
	}

	void Overlays::DrawCenteredText(const char* text, float x, float y, ImFont* font, Color color, ImDrawList* drawlist)
	{
		auto ImGui = ImGui::GetIO().DisplaySize;

		ImGui::PushFont(font);
		drawlist->AddText(
			ImVec2(x * ImGui.x - (ImGui::CalcTextSize(text).x / 2.f), y * ImGui.y),
			IM_COL32(color.r, color.g, color.b, color.a),
			text
		);
		ImGui::PopFont();
	}

	ImVec2 Overlays::GetTextScale(const char* text)
	{
		auto ImGui = ImGui::GetIO().DisplaySize;

		return { ImGui::CalcTextSize(text).x / ImGui.x, ImGui::CalcTextSize(text).y / ImGui.y };
	}
		
	// Notifications
	void Overlays::DrawNotification(const int& length, std::string header, std::string content, Color text, Color line, Color background, int alpha)
	{
		// Thanks yuh
		ImGui::BeginGroup();
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImColor(text.r, text.g, text.b, alpha)));
		ImGui::GetCurrentWindow()->DrawList->ChannelsSplit(2);
		ImGui::GetCurrentWindow()->DrawList->ChannelsSetCurrent(1);
		ImGui::PushFont(Renderer::NotificationTitle());
		if (header != "") {
			ImGui::SetCursorPosX(15.f);
			ImGui::TextWrapped(header.c_str());
		}
		ImGui::PopFont();
		ImGui::PushFont(Renderer::NotificationContent());
		ImGui::SetCursorPosX(15.f);
		ImGui::TextWrapped(content.c_str());
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::EndGroup();
		ImGui::GetCurrentWindow()->DrawList->ChannelsSetCurrent(0);
		ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(ImGui::GetItemRectMin().x - 5.f, ImGui::GetItemRectMin().y - 5.f), ImVec2(length + 5.f, ImGui::GetItemRectMax().y + 9.f), ImColor(background.r, background.g, background.b, alpha));
		ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(ImGui::GetItemRectMin().x - 5.f, ImGui::GetItemRectMin().y - 5.f), ImVec2(ImGui::GetItemRectMin().x - 2.5f, ImGui::GetItemRectMax().y + 9.f), ImColor(line.r, line.g, line.b, alpha));
		ImGui::Dummy(ImVec2(0.f, 0.1f));
		ImGui::GetCurrentWindow()->DrawList->ChannelsMerge();
	}

	void Overlays::NotificationManager()
	{
		// Before you say something Bomuld, indeed this may not be the best method, but it works.
		ImGui::SetNextWindowSize(ImVec2(300.f, 600.f), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(15.f, 615.f), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
		if (ImGui::Begin("Notification Manager", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove))
		{
			for (auto& Notification : g_Notifications)
			{
				if (GetTickCount() - Notification.m_CreationTime < Notification.m_Duration) {
					if (Notification.m_Alpha != 255) {
						Notification.m_Alpha += 5;
					}

					DrawNotification(300, Notification.m_Title, Notification.m_Content, g_TextColor, Notification.m_LineColor, g_BackgroundColor, Notification.m_Alpha);
				}
				if (GetTickCount() - Notification.m_CreationTime > Notification.m_Duration) {
					if (Notification.m_Alpha != 0) {
						Notification.m_Alpha -= 5;
					}

					DrawNotification(300, Notification.m_Title, Notification.m_Content, g_TextColor, Notification.m_LineColor, g_BackgroundColor, Notification.m_Alpha);
				}

				if (GetTickCount() - Notification.m_CreationTime > Notification.m_Duration + 1000) {
					g_Notifications.erase(g_Notifications.begin());
				}
			}

			ImGui::End();
		}
	}
}