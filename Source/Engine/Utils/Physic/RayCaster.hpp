#pragma once

#include <optional>

#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Base/Model/Model.hpp"
#include "../../Graphic/Base/Model/Primitive/Quad.hpp"

namespace RayCaster {
	// Note: return vec4: {vec3(Point.xyz), float distance}
	std::optional<glm::vec4> Intersect(const glm::vec2& mousePos, const Camera&, const Model::Ref, const glm::mat4& quat);
	std::optional<glm::vec4> Intersect(const glm::vec2& mousePos, const Camera&, const Mesh&,	   const glm::mat4& quat);

	float ApproxDistance(const glm::vec3& origin, const Model::Ref, const glm::mat4& quat);

	// For internally purpose, but feel free to use
	std::optional<glm::vec4> Intersect(const glm::vec3& ray_origin, const glm::vec3& ray_vector, const PrimitiveHelper::Triangle& triangle);

	bool IntersectBox(const glm::vec2& mousePos, const glm::vec3& camPos, const glm::vec3& camDir, const glm::mat4& quat);
	bool PointInRect(const glm::vec2& point, const glm::vec2& rectTopLeft, const glm::vec2& rectBotRight);
	bool PointInRect(const glm::vec2& point, const Quad& quad);
}