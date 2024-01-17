#pragma once

#include "imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

struct MainUI 
{
				MainUI(GLFWwindow* backend);
				~MainUI();
		
				void showBuildingSelection();
		
				void Prepare();
				void Render();

				void resetState();

				bool ready = false;

	// Members
	struct State
	{
					bool building;
	} state;

};