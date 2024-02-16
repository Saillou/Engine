#include "Collider.hpp"
#include "RayCaster.hpp"

#include "../../Graphic/Base/Model/Primitive/Cube.hpp"
#include "../../Graphic/Base/Model/MeshIterator.hpp"

#include <stack>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

using Point		= vec3;
using Triangle	= PrimitiveHelper::Triangle;

// Function to find the intersection points of two triangles
static optional<Point> __intersect_triangles(const Triangle& t1, const Triangle& t2) 
{
	// Check if one side of triangle #2 intersects triangle #1
	for (size_t i = 0; i < 3; i++) {
		const vec3& origin = t2[i];
		const vec3& target = t2[(i + 1) % 3];
		const vec3 ray = target - origin;

		std::optional<vec4> project_intersect_result = RayCaster::Intersect(origin, ray, t1);

		// Potential intersection
		if (!project_intersect_result.has_value())
			return {};

		const glm::vec3& projected_point = project_intersect_result.value();
		float dop = length(origin - projected_point);
		float dpt = length(projected_point - target);
		float dot = length(origin - target);

		// Too far
		if (dop + dpt > dot)
			return {};

		return project_intersect_result;
	}
	return {};
}

static optional<Point> __find_intersection_point(const Triangle& t1, const Triangle& t2)
{
	optional<Point> pt;

	pt = __intersect_triangles(t2, t1);
	if (pt.has_value())
		return pt;

	pt = __intersect_triangles(t1, t2);
	if (pt.has_value())
		return pt;

	return {};
}

// Public
std::optional<std::vector<glm::vec3>> Collider::Check(
	const Model::Ref model1, const glm::mat4& worldPose1, 
	const Model::Ref model2, const glm::mat4& worldPose2,
	bool accurate
) {
	const Mesh& cube = *Cube::GetOne();

	std::optional<std::vector<glm::vec3>> result;

	MeshIterator::forEachMesh(*model1, [&](const std::unique_ptr<Mesh>& mesh1, const MeshIterator::Accumulator& acc1) 
	{
		// Already got a value: break; (may try a go to?)
		if (!accurate && result.has_value())
			return;

		const glm::mat4& mesh_pose1 = worldPose1 * model1->localPose * acc1.transform;

		MeshIterator::forEachMesh(*model2, [&](const std::unique_ptr<Mesh>& mesh2, const MeshIterator::Accumulator& acc2) 
		{
			// Already got a value: break; (may try a go to?)
			if (!accurate && result.has_value())
				return;

			const glm::mat4& mesh_pose2 = worldPose2 * model2->localPose * acc2.transform;

			// First simple check: hitbox
			auto pt = Collider::Check(
				cube, mesh_pose1 * mesh1->obb(),
				cube, mesh_pose2 * mesh2->obb()
			);

			if (!pt.has_value())
				return;

			// Fast check: just get first colliding point
			if (!accurate) {
				result = pt;
				return;
			}

			// Accurate slow check: meshes
			auto pt_acc = Collider::Check(
				*mesh1, mesh_pose1,
				*mesh2, mesh_pose2
			);

			if (!pt_acc.has_value())
				return;

			if (!result.has_value())
				result = pt_acc;
			else
				result.value().insert(result.value().cend(), pt_acc.value().cbegin(), pt_acc.value().cend());
		});
    });

	return result;
}

std::optional<std::vector<glm::vec3>> Collider::Check(
	const Mesh& m1, const glm::mat4& q1, 
	const Mesh& m2, const glm::mat4& q2
) {
	std::vector<glm::vec3> hitPoints;

	const auto& idx1 = m1.indices;
	const auto& idx2 = m2.indices;
	const auto& v1 = m1.vertices;
	const auto& v2 = m2.vertices;

	for (size_t i1 = 0; i1 < idx1.size(); i1 += 3) {
		// Get triangle from mesh #1
		Triangle triangle1{
			vec3(q1 * vec4(v1[idx1[i1 + 0]].Position, +1.0f)),
			vec3(q1 * vec4(v1[idx1[i1 + 1]].Position, +1.0f)),
			vec3(q1 * vec4(v1[idx1[i1 + 2]].Position, +1.0f))
		};

		// Get triangle from mesh #2
		for (size_t i2 = 0; i2 < idx2.size(); i2 += 3) {
			Triangle triangle2{
				vec3(q2 * vec4(v2[idx2[i2 + 0]].Position, +1.0f)),
				vec3(q2 * vec4(v2[idx2[i2 + 1]].Position, +1.0f)),
				vec3(q2 * vec4(v2[idx2[i2 + 2]].Position, +1.0f))
			};

			auto pt = __find_intersection_point(triangle1, triangle2);
			if (pt.has_value())
				hitPoints.push_back(pt.value());
		}
	}

	if (hitPoints.empty())
		return {};

	return hitPoints;
}
