#pragma once

#include "imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

enum class EditorId
{
				None = 0,
				Material,
				Model
};

struct Menu 
{
				Menu(GLFWwindow* backend);
				~Menu();
		
	void ShowMenuBar();
	void ShowMenuFile();
		
	void Prepare();
	void Render();

	// Members
	struct State
	{
					bool goToNewEditor;
					EditorId editorId;
	} state;

};