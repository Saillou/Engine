#pragma once

#include "../../_imgui/imgui.h"
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
		glm::vec3 scale;
		float roll;
		float pitch;
		float yaw;
		std::string path;
		bool updateAsset;

		glm::vec3 centerPosition;
		glm::vec3 worldPosition;

		bool showWireFrame;
		int cameraType;
	} state;

};