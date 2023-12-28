#pragma once

#include <glm/glm.hpp>

// Objects
struct Light {
	Light(const glm::vec3& position, const glm::vec4& color);

	glm::vec3 position;
	glm::vec4 color;
};
