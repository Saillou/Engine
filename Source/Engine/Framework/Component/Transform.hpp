#pragma once

#include <glm/glm.hpp>

struct Transform 
{
	glm::mat4 local = glm::mat4(1.0f);
	glm::mat4 world = glm::mat4(1.0f);

	glm::mat4 get() const {
		return world * local;
	}
};