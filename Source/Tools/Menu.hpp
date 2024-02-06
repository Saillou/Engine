#pragma once

#include <Engine/Graphic/Window.hpp>
#include <memory>

#include "imgui/imgui.h"

enum class EditorId
{
	None = 0,
	Material, Model, World
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
		EditorId editorId = EditorId::None;
	} state;
};