#pragma once

#include <array>
#include <optional>
#include <vector>

#include "../../Graphic/Base/Model/Model.hpp"

namespace Collider {
	std::optional<glm::vec3> Check(
		const Model::Ref model1, const glm::mat4& q1,
		const Model::Ref model2, const glm::mat4& q2
	);

	std::optional<glm::vec3> Check(
		const Mesh& mesh1, const glm::mat4& q1,
		const Mesh& mesh2, const glm::mat4& q2
	);

	std::optional<std::vector<glm::vec3>> CheckMultiple(
		const Entity& e1, const glm::mat4& q1,
		const Entity& e2, const glm::mat4& q2
	);

	std::optional<std::vector<glm::vec3>> CheckMultiple(
		const Mesh& e1, const glm::mat4& q1,
		const Mesh& e2, const glm::mat4& q2
	);
}