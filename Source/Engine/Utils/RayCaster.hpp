#pragma once

#include <array>

#include "../Graphic/Camera.hpp"
#include "../Graphic/Base/Shapes/BaseShape.hpp"
#include "../Graphic/Base/Model/Entity.hpp"

namespace RayCaster {
	float Intersect(const glm::vec2& mousePos, const Camera&, const Entity&,	  const glm::mat4& quat, glm::vec3& outPos);
	float Intersect(const glm::vec2& mousePos, const Camera&, const BaseShape&,const glm::mat4& quat, glm::vec3& outPos);
	float Intersect(const glm::vec2& mousePos, const Camera&, const Mesh&,	  const glm::mat4& quat, glm::vec3& outPos);

	// For internally purpose, but feel free to use
	float IntersectTriangle(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const std::array<glm::vec3, 3>& triangle, glm::vec3& outPos);
	bool PointInRect(const glm::vec2& point, const glm::vec2& rectTopLeft, const glm::vec2& rectBotRight);
}