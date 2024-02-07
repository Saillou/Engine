#pragma once

#include <Engine/Graphic/Base/Model/Entity.hpp>

// Just a bunch of utility functions
// Defined here to avoid bloating the editor with implementation details

namespace WorldEditorHelper {
	Entity train();
	Entity tile_with_texture(const std::string& texturePath);
	Entity tile_with_rgba(const glm::vec4& colorRGBA);

	glm::mat4 pose(const glm::vec2& position);
	glm::mat4 pose_scale(const glm::vec2& position, float scale);
	glm::mat4 pose_rot(const glm::vec2& position, float angle);
}