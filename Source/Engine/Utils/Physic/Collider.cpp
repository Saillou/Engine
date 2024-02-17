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

		std::optional<vec4> project_intersect_result = RayCaster::Intersect(target, ray, t1);

		// Potential intersection
		if (!project_intersect_result.has_value())
			continue;

		const Point& projected_point = project_intersect_result.value();
		float dop = length(origin - projected_point);
		float dpt = length(projected_point - target);
		float dot = length(origin - target);

		// Too far
		if (dop + dpt > dot)
			continue;

		return project_intersect_result;
	}

	return {};
}

// Public
std::optional<Point> Collider::CheckAccurate(
	const Model::Ref model1, const glm::mat4& worldPose1, 
	const Model::Ref model2, const glm::mat4& worldPose2
) {
	std::optional<Point> result;

	MeshIterator::forEachMesh(*model1, [&](const std::unique_ptr<Mesh>& mesh1, const MeshIterator::Accumulator& acc1) 
	{
		if (result.has_value())
			return;

		const glm::mat4& mesh_pose1 = worldPose1 * model1->localTransform * acc1.transform;

		MeshIterator::forEachMesh(*model2, [&](const std::unique_ptr<Mesh>& mesh2, const MeshIterator::Accumulator& acc2) 
		{
			if (result.has_value())
				return;

			const glm::mat4& mesh_pose2 = worldPose2 * model2->localTransform * acc2.transform;

			result = Collider::CheckMeshes(*mesh1, mesh_pose1, *mesh2, mesh_pose2);
		});
    });

	return result;
}

std::optional<std::vector<Point>> Collider::CheckAccurateMultiple(
	const Model::Ref model1, const glm::mat4& worldPose1, 
	const Model::Ref model2, const glm::mat4& worldPose2
) {
	std::optional<std::vector<Point>> result;

	MeshIterator::forEachMesh(*model1, [&](const std::unique_ptr<Mesh>& mesh1, const MeshIterator::Accumulator& acc1) 
	{
		const glm::mat4& mesh_pose1 = worldPose1 * model1->localTransform * acc1.transform;

		MeshIterator::forEachMesh(*model2, [&](const std::unique_ptr<Mesh>& mesh2, const MeshIterator::Accumulator& acc2) 
		{
			const glm::mat4& mesh_pose2 = worldPose2 * model2->localTransform * acc2.transform;

			std::optional<std::vector<Point>> pt_acc = Collider::CheckMeshesMultiple(*mesh1, mesh_pose1, *mesh2, mesh_pose2);
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

std::optional<Point> Collider::CheckHitboxes(
	const Model::Ref model1, const glm::mat4& worldPose1,
	const Model::Ref model2, const glm::mat4& worldPose2)
{
	const Mesh& cube = *Cube::GetOne();

	std::optional<Point> result;

	MeshIterator::forEachMesh(*model1, [&](const std::unique_ptr<Mesh>& mesh1, const MeshIterator::Accumulator& acc1)
	{
		if (result.has_value())
			return;

		const glm::mat4& mesh_pose1 = worldPose1 * model1->localTransform * acc1.transform;

		MeshIterator::forEachMesh(*model2, [&](const std::unique_ptr<Mesh>& mesh2, const MeshIterator::Accumulator& acc2)
		{
			if (result.has_value())
				return;

			const glm::mat4& mesh_pose2 = worldPose2 * model2->localTransform * acc2.transform;

			auto pt = Collider::CheckMeshes(
				cube, mesh_pose1 * mesh1->obb(),
				cube, mesh_pose2 * mesh2->obb()
			);
			if (!pt.has_value())
				return;

			result = pt;
		});
	});

	return result;
}

std::optional<std::vector<Point>> Collider::CheckHitboxesMultiple(
	const Model::Ref model1, const glm::mat4& worldPose1,
	const Model::Ref model2, const glm::mat4& worldPose2)
{
	const Mesh& cube = *Cube::GetOne();

	std::optional<std::vector<Point>> result;

	MeshIterator::forEachMesh(*model1, [&](const std::unique_ptr<Mesh>& mesh1, const MeshIterator::Accumulator& acc1)
	{
		const glm::mat4& mesh_pose1 = worldPose1 * model1->localTransform * acc1.transform;

		MeshIterator::forEachMesh(*model2, [&](const std::unique_ptr<Mesh>& mesh2, const MeshIterator::Accumulator& acc2)
		{
			const glm::mat4& mesh_pose2 = worldPose2 * model2->localTransform * acc2.transform;

			auto pt = Collider::CheckMeshesMultiple(
				cube, mesh_pose1 * mesh1->obb(),
				cube, mesh_pose2 * mesh2->obb()
			);
			if (!pt.has_value())
				return;

			if (!result.has_value())
				result = pt;
			else
				result.value().insert(result.value().cend(), pt.value().cbegin(), pt.value().cend());
		});
	});

	return result;
}

std::optional<std::vector<Point>> Collider::CheckMeshesMultiple(
	const Mesh& m1, const glm::mat4& q1, 
	const Mesh& m2, const glm::mat4& q2
) {
	std::vector<Point> hitPoints;

	const auto& idx1 = m1.indices;
	const auto& idx2 = m2.indices;
	const auto& v1 = m1.vertices;
	const auto& v2 = m2.vertices;

	for (size_t i1 = 0; i1 < idx1.size(); i1 += 3) {
		// Get triangle from mesh #1
		Triangle triangle1{
			Point(q1 * vec4(v1[idx1[i1 + 0]].Position, +1.0f)),
			Point(q1 * vec4(v1[idx1[i1 + 1]].Position, +1.0f)),
			Point(q1 * vec4(v1[idx1[i1 + 2]].Position, +1.0f))
		};

		// Get triangle from mesh #2
		for (size_t i2 = 0; i2 < idx2.size(); i2 += 3) {
			Triangle triangle2{
				Point(q2 * vec4(v2[idx2[i2 + 0]].Position, +1.0f)),
				Point(q2 * vec4(v2[idx2[i2 + 1]].Position, +1.0f)),
				Point(q2 * vec4(v2[idx2[i2 + 2]].Position, +1.0f))
			};

			std::optional<Point> pt;
			pt = __intersect_triangles(triangle1, triangle2);
			if (pt.has_value())
				hitPoints.push_back(pt.value());

			pt = __intersect_triangles(triangle2, triangle1);
			if (pt.has_value())
				hitPoints.push_back(pt.value());
		}
	}

	if (hitPoints.empty())
		return {};

	return hitPoints;
}

std::optional<Point> Collider::CheckMeshes(
	const Mesh& m1, const glm::mat4& q1, 
	const Mesh& m2, const glm::mat4& q2
) {
	const auto& idx1 = m1.indices;
	const auto& idx2 = m2.indices;
	const auto& v1 = m1.vertices;
	const auto& v2 = m2.vertices;

	for (size_t i1 = 0; i1 < idx1.size(); i1 += 3) {
		// Get triangle from mesh #1
		Triangle triangle1{
			Point(q1 * vec4(v1[idx1[i1 + 0]].Position, +1.0f)),
			Point(q1 * vec4(v1[idx1[i1 + 1]].Position, +1.0f)),
			Point(q1 * vec4(v1[idx1[i1 + 2]].Position, +1.0f))
		};

		// Get triangle from mesh #2
		for (size_t i2 = 0; i2 < idx2.size(); i2 += 3) {
			Triangle triangle2{
				Point(q2 * vec4(v2[idx2[i2 + 0]].Position, +1.0f)),
				Point(q2 * vec4(v2[idx2[i2 + 1]].Position, +1.0f)),
				Point(q2 * vec4(v2[idx2[i2 + 2]].Position, +1.0f))
			};

			std::optional<Point> pt;
			pt = __intersect_triangles(triangle1, triangle2);
			if (pt.has_value())
				return pt;

			pt = __intersect_triangles(triangle2, triangle1);
			if (pt.has_value())
				return pt;
		}
	}

	return {};
}
