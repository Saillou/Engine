#pragma once

#include <list>

#include "Component.hpp"
#include "TrainGame/App/Objects/Transform.h"

namespace Thomas
{

    class TrainController : public Component
    {
    public:
        TrainController(Transform* transform);
        ~TrainController();

        void update(float dt) override;

        void addPoint(const glm::vec3& pos);

    private:
        bool goToPoint(const glm::vec3& pos, float dt);

        // Misha: is there a better way of doing it ?
        Transform* m_transform = nullptr;
        float m_speed;
        float m_rotationSpeed;
        float m_minDistance;

        glm::vec3 m_forwardVector;

        std::list<glm::vec3>::iterator  m_currentPoint;
        std::list<glm::vec3>            m_points;
    };

} // namespace Thomas