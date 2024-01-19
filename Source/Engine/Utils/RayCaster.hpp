#pragma once

#include <array>

#include "../Graphic/Camera.hpp"
#include "../Graphic/Base/Model/Entity.hpp"

namespace RayCaster {
	using Triangle = std::array<glm::vec3, 3>;

	bool Intersect(const glm::vec2& mousePos, const Camera&, const Entity&,	const glm::mat4& quat);
	bool Intersect(const glm::vec2& mousePos, const Camera&, const Mesh&,	const glm::mat4& quat);
	bool Intersect(const glm::vec2& mousePos, const glm::vec3& camPos, const glm::vec3& camDir, const glm::mat4& quat);

	// For internally purpose, but feel free to use
	bool IntersectTriangle(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const Triangle& triangle);
	bool PointInRect(const glm::vec2& point, const glm::vec2& rectTopLeft, const glm::vec2& rectBotRight);
}