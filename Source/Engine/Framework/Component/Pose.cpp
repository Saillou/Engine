#include "Pose.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Pose::Pose() : _quat(glm::mat4(1.0f))
{ }

Pose::Pose(const glm::mat4& q):
    _quat(q)
{ }

Pose::Pose(const glm::vec3& position):
    _quat(glm::translate(glm::mat4(1.0f), position))
{ }

Pose::Pose(const glm::vec3& position, float scale) :
    _quat(glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(scale)))
{ }

Pose::Pose(const glm::vec3& position, const glm::vec3& scale) :
    _quat(glm::scale(glm::translate(glm::mat4(1.0f), position), scale))
{ }

Pose::operator glm::mat4() const {
    return _quat;
}

Pose::operator glm::mat4&() {
    return _quat;
}

const Pose& Pose::operator=(const glm::mat4& q) {
    _quat = q;
    return *this;
}
Pose& Pose::operator=(glm::mat4& q) {
    _quat = q;
    return *this;
}