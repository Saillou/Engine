#pragma once

#include "../../_imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

struct MaterialEditorMenu
{
	MaterialEditorMenu() {};
	~MaterialEditorMenu() {};
		
	void show();
	void reset();

	// Members
	struct State
	{
		bool lightEnabled = true;
		ImVec4 color = ImVec4(1,1,1,1);
	} state;
};