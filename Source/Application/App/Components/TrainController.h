#pragma once

#include <list>

#include "Component.hpp"
#include "Application/App/Objects/GameModel.hpp" // for Transform

class TrainController : public Component
{
public:
    TrainController(Transform* transform);
    ~TrainController();

    void update(float dt) override;

private:
    bool goToPoint(const glm::vec3& pos, float dt);

    // Misha: is there a better way of doing it ?
    Transform* m_transform = nullptr;
    float m_speed;
    float m_minDistance;

    std::list<glm::vec3>::iterator  m_currentPoint;
    std::list<glm::vec3>            m_points;
};