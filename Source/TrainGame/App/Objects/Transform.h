#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Thomas
{
    struct Transform
    {
        // TODO: think of caching the getMat4()
        glm::vec3   position;
        glm::vec3   scale;
        glm::vec3   rotation;

        glm::mat4 getMat4() const
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
            model = glm::translate(model, position);
            model = glm::scale(model, scale);

            return model;
        } 
    };
} // namespace Thomas