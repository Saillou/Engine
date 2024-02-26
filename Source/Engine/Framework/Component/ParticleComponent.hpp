#pragma once

#include "../../Graphic/Base/Model/Model.hpp"

#include "Transform.hpp"
#include "Material.hpp"
#include <vector>

struct ParticleComponent 
{
	// Data
	enum class Type {
		Solid,
		Geometry,
		Particle
	};

	struct Element {
		glm::vec4 color;
		glm::mat4 world;
	};

	// Members
	Type type = Type::Solid;
	std::vector<Element> elements;
};