#include "Collider.hpp"
#include "RayCaster.hpp"

#include "../../Graphic/Base/Model/Primitive/Cube.hpp"

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

	return {};
}

// Public
std::optional<std::vector<glm::vec3>> Collider::Check(
	const Model::Ref model1, const glm::mat4& worldPose1, 
	const Model::Ref model2, const glm::mat4& worldPose2,
	bool accurate
) {
	std::optional<std::vector<glm::vec3>> result;

	//for (auto it1 = Model::MeshIteratorBegin(model1); it1 != Model::MeshIteratorEnd(model1); it1++) {
	//	const glm::mat4& q1 = worldPose1 * it1->localPose;

	//	for (auto it2 = Model::MeshIteratorBegin(model2); it2 != Model::MeshIteratorEnd(model2); it2++) {
	//		const glm::mat4& q2 = worldPose1 * it2->localPose;
	//	}
	//}

	std::vector<glm::mat4> localPoses1 = model1->GetMeshesPoses();
	std::vector<glm::mat4> localPoses2 = model2->GetMeshesPoses();

	for (const glm::mat4& localPose1 : localPoses1) {
		const glm::mat4& q1 = worldPose1 * localPose1;

		for (const glm::mat4& localPose2 : localPoses2) {
			const glm::mat4& q2 = worldPose2 * localPose2;

			// First simple check
			auto pt = Collider::Check(*Cube::GetOne(), q1, *Cube::GetOne(), q2);
			if (!pt.has_value())
				continue;

			if (!accurate)
				return pt;

			// Accurate
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

bool Collider::EqualEnough(const glm::vec3& a, const glm::vec3& b, const float epsilon) {
	return glm::all(glm::lessThan(glm::abs(a-b), glm::vec3(epsilon)));
}
