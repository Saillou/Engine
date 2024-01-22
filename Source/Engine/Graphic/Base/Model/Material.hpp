#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Material {
	glm::vec4 diffuse_color = { 0, 0, 0, 1 };
	bool cast_shadow        = true;

	static std::vector<glm::vec4> ExtractColors(const std::vector<Material>& materials);
};