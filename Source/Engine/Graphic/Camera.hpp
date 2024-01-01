#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    Camera();

    // Members
    glm::mat4 projection = {};
    glm::mat4 modelview  = {};

    glm::vec3 position  = {};
    glm::vec3 direction = {};
    float fieldOfView   = 30.0f;
    float near_plane    = 1.0f; // 0.1f
    float far_plane     = 25.0f; // 100.0f

    // Setters
    void lookAt(const glm::vec3& up);
    void usePerspective(float aspect);
    void useOrtho(float aspect);
};
