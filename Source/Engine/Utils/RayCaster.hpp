#pragma once

#include <array>

#include "../Graphic/Camera.hpp"
#include "../Graphic/Base/Shapes/BaseShape.hpp"

namespace RayCaster {
	bool Intersect(const glm::vec2& mousePos, const Camera&, const BaseShape&, const glm::mat4& quat);
	bool IntersectTriangle(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const std::array<glm::vec3, 3>& triangle);

	bool PointInRect(const glm::vec2& point, const glm::vec2& rectTopLeft, const glm::vec2& rectBotRight);
}