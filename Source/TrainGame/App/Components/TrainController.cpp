#include "TrainController.h"

#include "glm/gtx/vector_angle.hpp"

TrainController::TrainController(Transform* transform)
    : m_transform(transform)
    , m_speed(1.f)
    , m_rotationSpeed(1.f)
    , m_minDistance(0.01f)
{
    m_id = ComponentId::TrainController;

    m_points.push_back({ 1.5f, 2.f, 1.f });
    m_points.push_back({ 1.5f, -2.f, 1.f });
    m_points.push_back({ -1.5f, -2.f, 1.f });
    m_points.push_back({ -1.5f, 2.f, 1.f });

    m_currentPoint = m_points.begin();
}

TrainController::~TrainController()
{
}

void TrainController::update(float dt)
{
    if (goToPoint(*m_currentPoint, dt))
    {
        m_currentPoint = std::next(m_currentPoint, 1);
        if (m_currentPoint == m_points.end())
        {
            m_currentPoint = m_points.begin();
        }
    }
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
    m_forwardVector.x = -std::sin(m_transform->rotation);
    m_forwardVector.z = 0.f;
    m_forwardVector.y = std::cos(m_transform->rotation);

    //const float angle2 = glm::angle(m_forwardVector, dir);
    const float angle = -glm::orientedAngle(glm::vec2(m_forwardVector.x, m_forwardVector.y), glm::vec2(dir.x, dir.y));

    const float speed = m_rotationSpeed * dt;
    const float epsilon = 0.01f;
    std::cout << "Forward vecotr: {" << m_forwardVector.x << ", " << m_forwardVector.y << ", " << m_forwardVector.z << "}; Angle: " << angle << "\n";
    if (angle > epsilon)
    {
        m_transform->rotation -= speed;
        std::cout << "MINUS!\n";
    }
    else if(angle < -epsilon)
    {
        m_transform->rotation += speed;
        std::cout << "PLUS!\n";
    }
    else
    {
        //m_transform->rotation += angle;
        std::cout << "SMOL!\n";
    }
    return false;
}
