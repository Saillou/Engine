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

    // Setters
    void lookAt(const glm::vec3& up);
    void usePerspective(float aspect);
    void useOrtho(float aspect);
};
