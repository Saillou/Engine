#pragma once

#include <array>
#include <optional>

#include "../Graphic/Camera.hpp"
#include "../Graphic/Base/Model/Entity.hpp"

namespace RayCaster {
	using Triangle = std::array<glm::vec3, 3>;

	// Note: return vec4: {vec3(Point.xyz), float distance}
	std::optional<glm::vec4> Intersect(const glm::vec2& mousePos, const Camera&, const Entity&, const glm::mat4& quat);
	std::optional<glm::vec4> Intersect(const glm::vec2& mousePos, const Camera&, const Mesh&,	  const glm::mat4& quat);

	// For internally purpose, but feel free to use
	std::optional<glm::vec4> IntersectTriangle(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const Triangle& triangle);

	bool IntersectBox(const glm::vec2& mousePos, const glm::vec3& camPos, const glm::vec3& camDir, const glm::mat4& quat);
	bool PointInRect(const glm::vec2& point, const glm::vec2& rectTopLeft, const glm::vec2& rectBotRight);
}