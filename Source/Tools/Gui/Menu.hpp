#pragma once

#include "imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

struct Menu 
{
			 Menu(GLFWwindow* backend);
			~Menu();

	void	 ShowMovableOptions();
	void	 ShowMenuBar();
	void	 ShowMenuFile();

	void	 Prepare();
	void	 Render();

	// Members
	struct State 
	{
		bool lightEnabled	= true;
		ImVec4 color		= { 1.0f, 0.7f, 0.3f, 1.0f };
	} state;

};