#pragma once

#include <array>
#include <optional>
#include <vector>

#include "../../Graphic/Base/Model/Model.hpp"

namespace Collider {
	// -- Return first point colliding --
	std::optional<glm::vec3> CheckAccurate(
		const Model::Ref model1, const glm::mat4& worldPose1,
		const Model::Ref model2, const glm::mat4& worldPose2
	);
	std::optional<glm::vec3> CheckHitboxes(
		const Model::Ref model1, const glm::mat4& worldPose1,
		const Model::Ref model2, const glm::mat4& worldPose2
	);
	std::optional<glm::vec3> CheckMeshes(
		const Mesh& mesh1, const glm::mat4& q1,
		const Mesh& mesh2, const glm::mat4& q2
	);

	// -- Return all points colliding --
	std::optional<std::vector<glm::vec3>> CheckAccurateMultiple(
		const Model::Ref model1, const glm::mat4& worldPose1,
		const Model::Ref model2, const glm::mat4& worldPose2
	);
	std::optional<std::vector<glm::vec3>> CheckHitboxesMultiple(
		const Model::Ref model1, const glm::mat4& worldPose1,
		const Model::Ref model2, const glm::mat4& worldPose2
	);
	std::optional<std::vector<glm::vec3>> CheckMeshesMultiple(
		const Mesh& mesh1, const glm::mat4& q1,
		const Mesh& mesh2, const glm::mat4& q2
	);

}