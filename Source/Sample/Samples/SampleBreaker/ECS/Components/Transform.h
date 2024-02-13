#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Breaker
{
    struct Transform
    {
        // TODO: think of caching the getMat4()
        glm::vec3   position;
        glm::vec3   scale;
        glm::vec3   rotation;
        glm::mat4   model;

        glm::mat4 getMat4() const
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
            model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
            model = glm::scale(model, scale);

            return model;
        }

        void calculateMat4()
        {
            glm::mat4 newModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
            newModel = glm::rotate(newModel, rotation.x, glm::vec3(1, 0, 0));
            newModel = glm::rotate(newModel, rotation.y, glm::vec3(0, 1, 0));
            newModel = glm::rotate(newModel, rotation.z, glm::vec3(0, 0, 1));
            newModel = glm::translate(newModel, position);
            newModel = glm::scale(newModel, scale);

            model = newModel;
        }
    };
} // namespace Breaker