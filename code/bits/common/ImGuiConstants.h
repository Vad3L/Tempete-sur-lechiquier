#ifndef TSL_IMGUI_CONSTANTS_H
#define TSL_IMGUI_CONSTANTS_H

#include <imgui.h>

namespace tsl {

	constexpr ImGuiWindowFlags DefaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings ;
	const ImVec2 DefaultButtonSize = { 200.0f, 70.0f };
	const ImVec2 DefaultProgressSize = { 0.0f, 40.0f };
}

#endif // TSL_IMGUI_CONSTANTS_H