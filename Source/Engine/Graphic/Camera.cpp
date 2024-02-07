#include "Camera.hpp"

Camera::Camera() : 
    projection(glm::mat4()), 
    modelview(glm::mat4()),
    position(glm::vec3(0.0f, -1.0f, 0.0f)),
    direction(glm::vec3(0.0f, 0.0, 0.0f)),
    up(glm::vec3(0,0,1))
{
    // ..
}

// Helpers
void Camera::lookAt() {
    modelview = glm::lookAt(position, direction, up);
}

void Camera::usePerspective(float aspect) {
    projection = glm::perspective(glm::radians<float>(fieldOfView), aspect, near_plane, far_plane);
}

void Camera::useOrtho(float aspect) {
    projection = glm::ortho(-aspect, +aspect, -1.0f, 1.0f, near_plane, far_plane);
}

// Methods
glm::vec3 Camera::ray(const glm::vec2& screenPos) const {
    // 4d Homogeneous Clip Coordinates
    const glm::vec4 vpMousePos(2.0f * screenPos.x - 1.0f, 1.0f - 2.0 * screenPos.y, +1.0f, +1.0f);

    // Un-projection of camera to get ray direction
    return normalize(glm::vec3(glm::inverse(projection * modelview) * vpMousePos));
}

