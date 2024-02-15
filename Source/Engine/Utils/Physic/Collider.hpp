#pragma once

#include <array>
#include <optional>
#include <vector>

#include "../../Graphic/Base/Model/Model.hpp"

namespace Collider {
	std::optional<std::vector<glm::vec3>> Check(
		const Model::Ref model1, const glm::mat4& q1,
		const Model::Ref model2, const glm::mat4& q2,
		bool accurate = false
	);

	std::optional<std::vector<glm::vec3>> Check(
		const Mesh& mesh1, const glm::mat4& q1,
		const Mesh& mesh2, const glm::mat4& q2
	);

	static bool EqualEnough(const glm::vec3& a, const glm::vec3& b, const float epsilon = 1e-3f);
}