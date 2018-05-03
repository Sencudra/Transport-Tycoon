
/////////////////////////////////////////////////////////////////////////////////////////
//
//	WARNING
//	This is not a part of Dear ImGui library. This header file has only custom widgets' 
//  and helper functions' implementation in ImGui namespace
//
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>


namespace ImGui
{
	// Functions here ( labdas below ):

	static void ShowHelpMarker(const char* desc);
	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);
	bool ListBox(const char* label, int* currIndex, std::vector<std::pair<std::string, std::string>> & values);
	bool MyListBox(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**, const char**), void* data, int items_count, int height_in_items = -1);

	// Question mark helper
	static void ShowHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	// Vector lambda
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	// rewritten lambda for std::vector<std::pair<std:string, std::string>> for ImGui::ListBox
	static auto vector_getterSS = [](void* vec, int idx, const char** first, const char** second)
	{
		auto& vector = *static_cast<std::vector<std::pair<std::string, std::string>>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*first = vector.at(idx).first.c_str();
		*second = vector.at(idx).second.c_str();
		return true;
	};

	bool ListBox(const char* label, int* currIndex, std::vector<std::pair<std::string, std::string>> & values)
	{
		if (values.empty()) { return false; }
		return MyListBox(label, currIndex, vector_getterSS,static_cast<void*>(&values), values.size());
	}

	bool MyListBox(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**, const char**), void* data, int items_count, int height_in_items)
	{
		if (!ListBoxHeader(label, items_count, height_in_items))
			return false;

		// Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
		bool value_changed = false;
		ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
		while (clipper.Step())
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				const bool item_selected = (i == *current_item);
				const char* first;
				const char* second;

				if (!items_getter(data, i, &first, &second))
				{
					first = "*Unknown item*";
					second = "*Unknown item*";
				}

				PushID(i);
				if (Selectable(first, item_selected))
				{
					*current_item = i;
					value_changed = true;
				}
				ImGui::SameLine(GetWindowContentRegionMax().x - ImGui::CalcTextSize(second).x);
				ImGui::Text(second);

				if (item_selected)
					SetItemDefaultFocus();
				PopID();
			}
		ListBoxFooter();
		return value_changed;
	}



}
