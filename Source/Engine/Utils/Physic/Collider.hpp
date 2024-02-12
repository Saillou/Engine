#pragma once

#include <array>
#include <optional>

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
}