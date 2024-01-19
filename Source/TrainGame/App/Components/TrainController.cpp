#include "TrainController.h"

#include "glm/gtx/vector_angle.hpp"

namespace Thomas
{

    TrainController::TrainController(Transform* transform)
        : m_transform(transform)
        , m_speed(0.7f)
        , m_rotationSpeed(2.5f)
        , m_minDistance(0.01f)
    {
        m_id = ComponentId::TrainController;

        m_currentPoint = m_points.end();
    }

    TrainController::~TrainController()
    {
    }

    void TrainController::update(float dt)
    {
        if (m_points.empty())
            return;

        if (goToPoint(*m_currentPoint, dt))
        {
            m_currentPoint = std::next(m_currentPoint, 1);
            if (m_currentPoint == m_points.end())
            {
                m_currentPoint = m_points.begin();
            }
        }
    }

    void TrainController::addPoint(const glm::vec3& pos)
    {
        m_points.push_back(pos);

        if (m_currentPoint == m_points.end())
            m_currentPoint = m_points.begin();
    }

    bool TrainController::goToPoint(const glm::vec3& pos, float dt)
    {
        glm::vec3 dir = pos - m_transform->position;

        if (glm::length(dir) < m_minDistance)
        {
            m_transform->position = pos;
            return true;
        }

        dir = glm::normalize(dir);
        m_transform->position += dir * dt * m_speed;

        // change angle
        m_forwardVector.x = -std::sin(m_transform->rotation.z);
        m_forwardVector.z = 0.f;
        m_forwardVector.y = std::cos(m_transform->rotation.z);

        //const float angle2 = glm::angle(m_forwardVector, dir);
        const float angle = -glm::orientedAngle(glm::vec2(m_forwardVector.x, m_forwardVector.y), glm::vec2(dir.x, dir.y));

        const float speed = m_rotationSpeed * dt;
        const float epsilon = 0.01f;

        if (angle > epsilon)
        {
            m_transform->rotation.z -= speed;
        }
        else if (angle < -epsilon)
        {
            m_transform->rotation.z += speed;
        }
        else
        {
        }
        return false;
    }

} // namespace Thomas