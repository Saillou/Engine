#include "TrainController.h"

TrainController::TrainController(Transform* transform)
    : m_transform(transform)
    , m_speed(1.f)
    , m_minDistance(0.01f)
{
    m_id = ComponentId::TrainController;

    m_points.push_back({ 0.f, 2.f, 1.f });
    m_points.push_back({ 0.f, -2.f, 1.f });
    m_points.push_back({ -2.f, -2.f, 1.f });
    m_points.push_back({ -2.f, 2.f, 1.f });

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
    return false;
}
