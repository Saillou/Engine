#include "Camera.hpp"

Camera::Camera() : 
    projection(glm::mat4()), 
    modelview(glm::mat4()),
    position(glm::vec3(0.0f, -1.0f, 0.0f)),
    direction(glm::vec3(0.0f, 0.0, 0.0f))
{
    // ..
}

// Helpers
void Camera::lookAt(const glm::vec3& up) {
    modelview = glm::lookAt(position, direction, up);
}

void Camera::usePerspective(float aspect) {
    projection = glm::perspective(glm::radians<float>(fieldOfView), aspect, near_plane, far_plane);
}

void Camera::useOrtho(float aspect) {
    projection = glm::ortho(-aspect, +aspect, -1.0f, 1.0f, near_plane, far_plane);
}

