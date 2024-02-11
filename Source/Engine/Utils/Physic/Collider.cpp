#include "Collider.hpp"
#include "RayCaster.hpp"

#include "../../Graphic/Base/Model/Primitive/Cube.hpp"

#include <stack>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

// Public
std::optional<glm::vec3> Collider::Check(
	const Entity& e1, const mat4& q1, 
	const Entity& e2, const mat4& q2)
{
	// TODO: do better - here just checking root's model's obb
	const mat4& obb_1 = e1.model().root()->meshes.front()->obb();
	const mat4& obb_2 = e2.model().root()->meshes.front()->obb();

	const mat4 tq_1 = q1 * e1.model().localPose() * e1.model().root()->transform * obb_1;
	const mat4 tq_2 = q2 * e2.model().localPose() * e2.model().root()->transform * obb_2;

	return Collider::Check(*Cube::GetOne(), tq_1, *Cube::GetOne(), tq_2);
}

std::optional<glm::vec3> Collider::Check(
	const Mesh& m1, const mat4& q1,
	const Mesh& m2, const mat4& q2)
{
	// TODO: do better - here just the first intersection, but there are (potentially) multiple ones
	const auto& idx1 = m1.indices();
	const auto& idx2 = m2.indices();
	const auto& v1   = m1.vertices();
	const auto& v2   = m2.vertices();

	for (size_t i1 = 0; i1 < idx1.size(); i1 += 3) {
		// Get triangle from mesh #1
		PrimitiveHelper::Triangle triangle1 {
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
				const glm::vec3& origin = triangle2[i];
				const glm::vec3& target = triangle2[(i+1)%3];
				const glm::vec3 ray = target - origin;

				std::optional<vec4> project_intersect_result = RayCaster::Intersect(origin, ray, triangle1);

				// Potential intersection
				if (project_intersect_result.has_value()) {
					// Too far, no interesect (idk why the /2. perhaps logic, perhaps mistake)
					if (abs(project_intersect_result.value().w) > length(ray)/2.0f)
						continue;

					return project_intersect_result;
				}
			}
		}
	}

	return {};
}
