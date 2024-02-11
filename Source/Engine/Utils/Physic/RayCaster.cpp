#include "RayCaster.hpp"

#include "../../Graphic/Base/Model/Primitive/Cube.hpp"

#include <stack>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

// Private
static inline std::optional<glm::vec4> _intersect_mesh(
	const glm::vec2& mousePos, 
	const glm::vec3& camPos, 
	const glm::vec3& camDir, 
	const Mesh& mesh, 
	const glm::mat4& quat)
{
	const auto& idx = mesh.indices();
	const auto& v = mesh.vertices();

	std::optional<glm::vec4> result;

	for (size_t i = 0; i < idx.size(); i += 3) {
		auto optIntersect = RayCaster::Intersect(camPos, camDir, PrimitiveHelper::Triangle{
			vec3(quat * vec4(v[idx[i + 0]].Position, +1.0f)),
			vec3(quat * vec4(v[idx[i + 1]].Position, +1.0f)),
			vec3(quat * vec4(v[idx[i + 2]].Position, +1.0f))
		});

		if (optIntersect.has_value()) {
			if (!result.has_value() || optIntersect.value().w < result.value().w)
				result = optIntersect;
		}
	}

	return result;
}

// Public
std::optional<glm::vec4> RayCaster::Intersect(const glm::vec2& mousePos, const Camera& camera, const Entity& entity, const glm::mat4& quat)
{
	// Mouse out screen
	if (!PointInRect(mousePos, vec2(0, 0), vec2(1, 1)))
		return {};

	// Traverse model's nodes
	if (!entity.model().root())
		return {};

	std::stack<std::unique_ptr<Model::Node> const*> st;
	st.push(&entity.model().root());

	while (!st.empty()) {
		// Get next in line
		const auto currNode = st.top();
		st.pop();

		// Check all meshes of this node
		for (const auto& mesh : (*currNode)->meshes) {
			auto optIntersect = RayCaster::Intersect(mousePos, camera, *mesh, quat * entity.model().localPose() * (*currNode)->transform);

			if (optIntersect.has_value())
				return optIntersect;
		}

		// Add children
		for (size_t i = 0; i < (*currNode)->children.size(); i++) {
			st.push(&(*currNode)->children[i]);
		}
	}

	return {};
}

std::optional<glm::vec4> RayCaster::Intersect(const glm::vec2& mousePos, const Camera& camera, const Mesh& mesh, const glm::mat4& quat)
{
	// Mouse out screen
	if (!PointInRect(mousePos, vec2(0, 0), vec2(1, 1)))
		return {};

	const glm::vec3 camera_ray = camera.ray(mousePos);

	// Check bounding rect
	if (!IntersectBox(mousePos, camera.position, camera_ray, quat * mesh.obb()))
		return {};

	// Check each triangle in meshes individually
	return _intersect_mesh(mousePos, camera.position, camera_ray, mesh, quat);
}



float RayCaster::ApproxDistance(const glm::vec3& origin, const Entity& entity, const glm::mat4& quat)
{
	// Will just check the root element..
	if (!entity.model().root() || entity.model().root()->meshes.empty())
		return -1.0f;

	// Check all meshes of root
	float avg_distance = 0.0f;
	const glm::mat4 Q = quat * entity.model().root()->transform * entity.model().localPose();

	for (const auto& mesh : entity.model().root()->meshes) {
		avg_distance += glm::distance(vec3(Q * mesh->obb()[3]), origin);
	}

	return avg_distance / entity.model().root()->meshes.size();
}

// - Helpers -
bool RayCaster::IntersectBox(const glm::vec2& mousePos, const glm::vec3& camPos, const glm::vec3& camDir, const glm::mat4& quat)
{
	return _intersect_mesh(mousePos, camPos, camDir, *Cube::GetOne(), quat).has_value();
}

// Note: It's Muller-Trumbore intersection algorithm
std::optional<glm::vec4> RayCaster::Intersect(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const PrimitiveHelper::Triangle& triangle)
{
	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	vec3 edge1 = triangle[1] - triangle[0];
	vec3 edge2 = triangle[2] - triangle[0];
	vec3 ray_cross_e2 = cross(ray_vector, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
		return {};    // This ray is parallel to this triangle.

	float inv_det = 1.f / det;
	vec3 s = ray_origin - triangle[0];
	float u = inv_det * dot(s, ray_cross_e2);

	if (u < 0 || u > 1)
		return {};

	vec3 s_cross_e1 = cross(s, edge1);
	float v = inv_det * dot(ray_vector, s_cross_e1);

	if (v < 0 || u + v > 1)
		return {};

	float t = inv_det * dot(edge2, s_cross_e1);
	return glm::vec4(ray_origin + ray_vector * t, t);
}

bool RayCaster::PointInRect(const glm::vec2& point, const glm::vec2& rectTopLeft, const glm::vec2& rectBotRight)
{
	if (point.x < rectTopLeft.x)
		return false;

	if (point.x > rectBotRight.x)
		return false;

	if (point.y < rectTopLeft.y)
		return false;

	if (point.y > rectBotRight.y)
		return false;

	return true;
}

bool RayCaster::PointInRect(const glm::vec2& point, const Quad& quad)
{
	const glm::vec2 rectTopLeft  = glm::vec2(quad.x(), quad.y());
	const glm::vec2 rectBotRight = rectTopLeft + glm::vec2(quad.w(), quad.h());

	return PointInRect(point, rectTopLeft, rectBotRight);
}