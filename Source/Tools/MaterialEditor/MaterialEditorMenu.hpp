#pragma once

#include "../imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

struct MaterialEditorMenu
{
				MaterialEditorMenu() {};
				~MaterialEditorMenu() {};
		
				void ShowMovableOptions();

				void resetState();

					// Members
					struct State
					{
								bool   lightEnabled;
								ImVec4 color;
					} state;

};