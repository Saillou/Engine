#pragma once

#include "../../Graphic/Base/Model/Model.hpp"


struct BatchComponent 
{
	std::vector<glm::mat4> transforms = {};
	std::vector<glm::vec4> materials  = {};
};