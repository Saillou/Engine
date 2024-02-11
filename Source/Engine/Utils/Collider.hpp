#pragma once

#include <array>
#include <optional>

#include "../Graphic/Base/Model/Entity.hpp"

namespace Collider {
	std::optional<glm::vec3> Check(
		const Entity& e1, const glm::mat4& q1, 
		const Entity& e2, const glm::mat4& q2
	);

	std::optional<glm::vec3> Check(
		const Mesh& e1, const glm::mat4& q1,
		const Mesh& e2, const glm::mat4& q2
	);
}