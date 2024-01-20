#pragma once

#include <glm/glm.hpp>

struct Pose {
	Pose(const glm::mat4& = glm::mat4(1.0f));

	glm::mat4& mat4();
	const glm::mat4& mat4() const;
	//glm::vec3 position;
	//glm::vec3 scale;
	//glm::vec3 rotation;

	//glm::mat4 getMat4() const;
	//void setMat4(const glm::mat4&);

private:
	glm::mat4 _quat;
};