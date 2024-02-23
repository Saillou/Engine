#pragma once

#include <string>
#include <glm/glm.hpp>
#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/DrawComponent.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>

namespace HelperTrain {
	Entity train();
	Entity tile_with_texture(const std::string& texturePath);
	Entity tile_with_rgba(const glm::vec4& colorRGBA);

	glm::mat4 pose(const glm::vec2& position);
	glm::mat4 pose_scale(const glm::vec2& position, float scale);
	glm::mat4 pose_rot(const glm::vec2& position, float angle);
}