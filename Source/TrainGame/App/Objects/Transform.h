#pragma once

#include "glm/glm.hpp"

namespace Thomas
{
    struct Transform
    {
        glm::vec3   position;
        glm::vec3   scale;
        glm::vec3   rotation;
    };
} // namespace Thomas