#pragma once

#include <glm/glm.hpp>

struct Pose {
	Pose();
	Pose(const glm::mat4&);
	Pose(const glm::vec3& position);
	Pose(const glm::vec3& position, float scale);

	// Cast
	operator glm::mat4() const;
	operator glm::mat4&();
	const Pose& operator=(const glm::mat4&);
	Pose& operator=(glm::mat4&);

private:
	glm::mat4 _quat;
};