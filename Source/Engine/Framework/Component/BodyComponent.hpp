#pragma once

#include "../../Graphic/Base/Model/Model.hpp"


struct BodyComponent 
{
	Model::Ref model = nullptr;

	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec4 material  = glm::vec4(1.0f);
};