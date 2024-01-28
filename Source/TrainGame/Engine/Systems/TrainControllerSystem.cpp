#include "TrainControllerSystem.h"

#include "glm/gtx/vector_angle.hpp"

#include "TrainGame/Engine/Core/ECS.h"
#include "TrainGame/Engine/Components/Transform2.h"
#include "TrainGame/Engine/Components/TrainController2.h"

namespace Thomas
{
    void TrainControllerSystem::update(float dt)
    {
        for (auto& entity : m_entities)
        {
            Transform2& transform           = ECS::getComponent<Transform2>(entity);
            TrainController2& controller    = ECS::getComponent<TrainController2>(entity);

            if (controller.m_points.empty())
                continue;

            bool atPos = false;

            if (!controller.m_init)
            {
                controller.m_currentPoint = controller.m_points.begin();
                controller.m_init = true;
            }

            glm::vec3 dir = *controller.m_currentPoint - transform.position;
                
            if (glm::length(dir) < controller.m_minDistance)
            {
                transform.position = *controller.m_currentPoint;
                atPos = true;
            }
            else
            {
                dir = glm::normalize(dir);
                transform.position += dir * dt * controller.m_speed;

                // change angle
                glm::vec3 forwardVector = { 0.f,0.f,0.f };
                forwardVector.x = -std::sin(transform.rotation.z);
                forwardVector.y = std::cos(transform.rotation.z);
                forwardVector.z = 0.f;

                const float angle = -glm::orientedAngle(glm::vec2(forwardVector.x, forwardVector.y), glm::vec2(dir.x, dir.y));

                const float speed = controller.m_rotationSpeed * dt;
                const float epsilon = 0.01f;

                if (angle > epsilon) {
                    transform.rotation.z -= speed;
                }
                else if (angle < -epsilon) {
                    transform.rotation.z += speed;
                }
                else
                {
                }
            }

            if (atPos)
            {
                controller.m_currentPoint = std::next(controller.m_currentPoint, 1);
                if (controller.m_currentPoint == controller.m_points.end()) {
                    controller.m_currentPoint = controller.m_points.begin();
                }
            }
            
        }
    }

    bool TrainControllerSystem::goToPoint(const glm::vec3& pos, float dt)
    {
        return false;
    }
} // namespace Thomas