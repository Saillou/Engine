#pragma once

#include "../../Graphic/Base/Model/Model.hpp"

#include "Transform.hpp"
#include "Material.hpp"
#include <vector>

struct ParticleComponent 
{
	// Expect vectors to be of the same size
	std::vector<glm::vec4> speeds;
	std::vector<glm::vec4> colors;
	std::vector<glm::mat4> worlds;
};