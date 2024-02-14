#include "Collider.hpp"
#include "RayCaster.hpp"

#include "../../Graphic/Base/Model/Primitive/Cube.hpp"

#include <stack>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

// Public
std::optional<std::vector<glm::vec3>> Collider::Check(
	const Model::Ref model1, const glm::mat4& worldPose1, 
	const Model::Ref model2, const glm::mat4& worldPose2
) {
	std::optional<std::vector<glm::vec3>> result;
	std::vector<glm::mat4> localPoses1 = model1->GetMeshesPoses();
	std::vector<glm::mat4> localPoses2 = model2->GetMeshesPoses();

	for (const glm::mat4& localPose1 : localPoses1) {
		const glm::mat4& q1 = worldPose1 * localPose1;

		for (const glm::mat4& localPose2 : localPoses2) {
			const glm::mat4& q2 = worldPose2 * localPose2;

			// First simple check
			auto pt = Collider::Check(*Cube::GetOne(), q1, *Cube::GetOne(), q2);
			if (!result.has_value())
				result = pt;
			else
				result.value().insert(result.value().cend(), pt.value().cbegin(), pt.value().cend());
		}
	}

	return result;
}

std::optional<std::vector<glm::vec3>> Collider::Check(
	const Mesh& m1, const glm::mat4& q1, 
	const Mesh& m2, const glm::mat4& q2
) {
	std::vector<glm::vec3> hitPoints;

	const auto& idx1 = m1.indices();
	const auto& idx2 = m2.indices();
	const auto& v1 = m1.vertices();
	const auto& v2 = m2.vertices();

	for (size_t i1 = 0; i1 < idx1.size(); i1 += 3) {
		// Get triangle from mesh #1
		PrimitiveHelper::Triangle triangle1{
			vec3(q1 * vec4(v1[idx1[i1 + 0]].Position, +1.0f)),
			vec3(q1 * vec4(v1[idx1[i1 + 1]].Position, +1.0f)),
			vec3(q1 * vec4(v1[idx1[i1 + 2]].Position, +1.0f))
		};

		// Get triangle from mesh #2
		for (size_t i2 = 0; i2 < idx2.size(); i2 += 3) {
			PrimitiveHelper::Triangle triangle2{
				vec3(q2 * vec4(v2[idx2[i2 + 0]].Position, +1.0f)),
				vec3(q2 * vec4(v2[idx2[i2 + 1]].Position, +1.0f)),
				vec3(q2 * vec4(v2[idx2[i2 + 2]].Position, +1.0f))
			};

			// Check if one side of triangle #2 intersects triangle #1
			for (size_t i = 0; i < 3; i++) {
				const vec3& origin = triangle2[i];
				const vec3& target = triangle2[(i + 1) % 3];
				const vec3 ray = target - origin;
				const vec3 uray = glm::normalize(ray);

				std::optional<vec4> project_intersect_result = RayCaster::Intersect(origin, uray, triangle1);

				// Potential intersection
				if (!project_intersect_result.has_value())
					continue;

				const glm::vec3& projected_point = project_intersect_result.value();
				float dop = length(origin - projected_point);
				float dtp = length(projected_point - target);
				float dot = length(origin - target);

				// Too far
				if (dop + dtp > dot)
					continue;

				// Add to list
				hitPoints.push_back(projected_point);
			}
		}
	}
	if (hitPoints.empty())
		return {};

	return hitPoints;
}

bool Collider::EqualEnough(const glm::vec3& a, const glm::vec3& b, const float epsilon) {
	return glm::all(glm::lessThan(glm::abs(a-b), glm::vec3(epsilon)));
}
