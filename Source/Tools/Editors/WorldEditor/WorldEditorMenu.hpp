#pragma once

#include "../../_imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

struct WorldEditorMenu
{
	WorldEditorMenu() {};
	~WorldEditorMenu() {};
		
	void show();
	void reset();

	// Members
	struct State
	{

	} state;

};