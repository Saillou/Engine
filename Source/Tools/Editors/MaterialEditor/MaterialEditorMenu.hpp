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
		bool   lightEnabled;
		ImVec4 color;
	} state;
};