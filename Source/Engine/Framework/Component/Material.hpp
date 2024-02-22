#pragma once

#include <glm/glm.hpp>

struct Material
{
	glm::vec4 color  = glm::vec4(1.0f);
	bool cast_shadow = true;

	Material& operator=(const glm::vec4& new_color) {
		color = new_color;
		return *this;
	}
};