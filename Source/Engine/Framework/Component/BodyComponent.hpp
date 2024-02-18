#pragma once

#include "../../Graphic/Base/Model/Model.hpp"


struct BodyComponent 
{
	Model::Ref model = nullptr;

	glm::mat4 localTransform = glm::mat4(1.0f);
	glm::vec4 localMaterial  = glm::vec4(1.0f);
};