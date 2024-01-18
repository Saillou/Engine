#include "RayCaster.hpp"

#include <stack>

using namespace glm;

bool RayCaster::Intersect(const glm::vec2& mousePos, const Camera& camera, const Entity& objModel, const glm::mat4& quat)
{
	// Mouse out screen
	if (!PointInRect(mousePos, vec2(0, 0), vec2(1, 1)))
		return false;

	// Traverse model's nodes
	if (!objModel.model.root())
		return false;

	std::stack<std::unique_ptr<Model::Node> const*> st;
	st.push(&objModel.model.root());

	while (!st.empty()) {
		// Get next in line
		const auto currNode = st.top();
		st.pop();

		// Check all meshes of this node
		for (const auto& mesh : (*currNode)->meshes) {
			if (RayCaster::Intersect(mousePos, camera, *mesh, quat * (*currNode)->transform))
				return true;
		}

		// Add children
		for (size_t i = 0; i < (*currNode)->children.size(); i++) {
			st.push(&(*currNode)->children[i]);
		}
	}

	return false;
}

bool RayCaster::Intersect(const glm::vec2& mousePos, const Camera& camera, const Mesh& mesh, const glm::mat4& quat)
{
	// Mouse out screen
	if (!PointInRect(mousePos, vec2(0, 0), vec2(1, 1)))
		return false;

	// It's almost the same as base shape
	const auto& idx = mesh.indices();
	const auto& v = mesh.vertices();

	for (size_t i = 0; i < idx.size(); i += 3) {
		if (IntersectTriangle(camera.position, camera.ray(mousePos), std::array<vec3, 3> {
			vec3(quat* vec4(v[idx[i+0]].Position, +1.0f)),
			vec3(quat* vec4(v[idx[i+1]].Position, +1.0f)),
			vec3(quat* vec4(v[idx[i+2]].Position, +1.0f))
		})) return true;
	}

	return false;
}

// Note: It's Möller–Trumbore intersection algorithm
bool RayCaster::IntersectTriangle(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const std::array<glm::vec3, 3>& triangle)
{
	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	vec3 edge1 = triangle[1] - triangle[0];
	vec3 edge2 = triangle[2] - triangle[0];
	vec3 ray_cross_e2 = cross(ray_vector, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
		return false;    // This ray is parallel to this triangle.

	float inv_det = 1.f / det;
	vec3 s = ray_origin - triangle[0];
	float u = inv_det * dot(s, ray_cross_e2);

	if (u < 0 || u > 1)
		return false;

	vec3 s_cross_e1 = cross(s, edge1);
	float v = inv_det * dot(ray_vector, s_cross_e1);

	if (v < 0 || u + v > 1)
		return false;

	return true;

	//// At this stage we can compute t to find out where the intersection point is on the line.
	//float t = inv_det * dot(edge2, s_cross_e1);

	//if (t > epsilon) // ray intersection: intersection_point = ray_origin + ray_vector * t;
	//	return true;

	//return false;
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