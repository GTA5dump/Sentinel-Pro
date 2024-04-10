#pragma once
#include "../Common.hpp"

namespace Sentinel::Elements
{
	// Drawing Functions
	extern void Gradient(ImVec2 a, ImVec2 b, ImColor c1, ImColor c2);
	extern void ChildGradient(const int& length, const int& height = 120);
	extern void SubOption(const char* title, ImVec2 size, ImVec2 pos);
	extern bool BoolOption(const char* title, bool* a, ImVec2 pos);
	extern bool SliderIntOption(const char* title, int* a, int min, int max, float width, ImVec2 pos);
	extern bool SliderFloatOption(const char* title, float* a, float min, float max, float width, ImVec2 pos);
	extern bool RegularOption(const char* title, ImVec2 size, ImVec2 pos);
	extern bool ListBoxHeaderOption(const char* title, ImVec2 size, ImVec2 pos);
	extern bool CustomListBoxHeaderOption(const char* title, ImVec2 size, ImVec2 pos);
	extern bool ComboOption(const char* title, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
	extern bool InputFloatOption(const char* title, float* a, float min, float max, const char* format);
	extern bool InputTextOption(const char* title, char* a, std::size_t size);
	extern bool ColorEditOption(const char* title, float* col, ImVec2 pos);
}