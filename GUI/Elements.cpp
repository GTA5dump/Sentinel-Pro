#include "Elements.hpp"
#include "../Renderer/Renderer.hpp"

namespace Sentinel
{
	void Elements::Gradient(ImVec2 a, ImVec2 b, ImColor c1, ImColor c2)
	{
		auto window = ImGui::GetCurrentWindow();

		if (window->SkipItems)
			return;
		window->DrawList->AddRectFilledMultiColor(a, b, c1, c2, c2, c1);
	}

	void Elements::ChildGradient(const int& length, const int& height)
	{
		auto old = ImGui::GetStyle().ItemSpacing;
		ImGui::GetStyle().ItemSpacing = ImVec2(0, 0);
		ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(ImGui::GetCurrentWindow()->DC.CursorPos.x, ImGui::GetCurrentWindow()->DC.CursorPos.y), ImVec2(ImGui::GetCurrentWindow()->DC.CursorPos.x + length, ImGui::GetCurrentWindow()->DC.CursorPos.y + height),
			ImGui::GetColorU32(ImVec4(0.145f, 0.145f, 0.145f, 1.0f)), 4.f);

		// Top line
		ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(ImGui::GetCurrentWindow()->DC.CursorPos.x, ImGui::GetCurrentWindow()->DC.CursorPos.y), ImVec2(ImGui::GetCurrentWindow()->DC.CursorPos.x + length * 0.5f, ImGui::GetCurrentWindow()->DC.CursorPos.y + 6.f),
			ImColor(99, 66, 245, 255), 4.f);
		ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(ImGui::GetCurrentWindow()->DC.CursorPos.x, ImGui::GetCurrentWindow()->DC.CursorPos.y), ImVec2(ImGui::GetCurrentWindow()->DC.CursorPos.x + length, ImGui::GetCurrentWindow()->DC.CursorPos.y + 6.f),
			ImColor(99, 66, 245, 255), 4.f);
		ImGui::GetStyle().ItemSpacing = old;
		ImGui::GetStyle().ItemSpacing = old;
	}

	void Elements::SubOption(const char* title, ImVec2 size, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		if (title != "")
			ImGui::Text(title);
		ImGui::SetCursorPos(ImVec2(pos.x, pos.y + 17.f));
		ChildGradient(size.x, size.y);
	}

	bool Elements::BoolOption(const char* title, bool* a, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(60, 60, 60, 255)));
		auto temp = ImGui::Checkbox(title, a);
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::SliderIntOption(const char* title, int* a, int min, int max, float width, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(60, 60, 60, 255)));
		ImGui::PushItemWidth(width);
		auto temp = ImGui::SliderInt(title, a, min, max);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::SliderFloatOption(const char* title, float* a, float min, float max, float width, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(60, 60, 60, 255)));
		ImGui::PushItemWidth(width);
		auto temp = ImGui::SliderFloat(title, a, min, max);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::RegularOption(const char* title, ImVec2 size, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		return ImGui::Button(title, size);
	}

	bool Elements::ListBoxHeaderOption(const char* title, ImVec2 size, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(255, 45, 45, 0)));
		auto temp = ImGui::ListBoxHeader(title, ImVec2(size.x, size.y));
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::CustomListBoxHeaderOption(const char* title, ImVec2 size, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		return ImGui::ListBoxHeader(title, ImVec2(size.x, size.y));
	}

	bool Elements::ComboOption(const char* title, int* current_item, const char* const items[], int items_count, int height_in_items)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(55, 55, 55, 255)));
		auto temp = ImGui::Combo(title, current_item, items, items_count, height_in_items);
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::InputFloatOption(const char* title, float* a, float min, float max, const char* format)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(30, 30, 30, 255)));
		auto temp = ImGui::InputFloat(title, a, min, max, format);
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::InputTextOption(const char* title, char* a, std::size_t size)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ImColor(30, 30, 30, 255)));
		auto temp = ImGui::InputText(title, a, size);
		ImGui::PopStyleColor();
		return temp;
	}

	bool Elements::ColorEditOption(const char* title, float* col, ImVec2 pos)
	{
		if (pos.x != -1.f && pos.y != -1.f)
			ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
		return ImGui::ColorEdit4(title, col, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
	}
}