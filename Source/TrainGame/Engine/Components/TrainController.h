#pragma once

#include <list>
#include "glm/glm.hpp"

namespace Thomas
{
    struct TrainController
    {
        float m_speed;
        float m_rotationSpeed;
        float m_minDistance;

        std::list<glm::vec3>::iterator  m_currentPoint;
        std::list<glm::vec3>            m_points;
        bool                            m_init = false;
    };
} // namespace Thomas