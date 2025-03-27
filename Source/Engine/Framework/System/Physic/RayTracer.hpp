#pragma once

#include <optional>

#include "../../../Graphic/Camera.hpp"

namespace RayTracer {
	glm::vec2 GetScreenPosition(const Camera&, const glm::vec3& worldPoint);
}