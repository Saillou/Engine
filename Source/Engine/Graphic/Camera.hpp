#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    Camera();

    // Members
    glm::mat4 projection = {};
    glm::mat4 modelview  = {};
    glm::vec2 screenSize = {};

    glm::vec3 position  = {};
    glm::vec3 direction = {};
    glm::vec3 up        = {};

    float fieldOfView   = 30.0f;
    float near_plane    = 0.1f;
    float far_plane     = 1000.0f;

    // Setters
    void lookAt();
    void usePerspective(float aspect);
    void useOrtho(float aspect);

    // Method
    glm::vec3 ray(const glm::vec2& screenPos) const;
};
