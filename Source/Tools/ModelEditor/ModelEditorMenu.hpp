#pragma once

#include "../imgui/imgui.h"
#include <Engine/Graphic/Window.hpp>

struct ModelEditorMenu
{
				ModelEditorMenu() {};
				~ModelEditorMenu() {};
		
				void ShowMovableOptions();

				void resetState();

				// Members
				struct State
				{
								glm::vec3 position;
								glm::vec3 rotationVector;
								glm::vec3 scale;
								float rotation;
								std::string path;
								bool updateAsset;

								glm::vec3 centerPosition;
								glm::vec3 worldPosition;

								bool showWireFrame;
								int cameraType;
				} state;

};