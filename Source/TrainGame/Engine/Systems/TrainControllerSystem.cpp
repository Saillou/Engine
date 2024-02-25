#include "TrainControllerSystem.h"

#include <glm/gtx/vector_angle.hpp>
#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>

#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/TrainController.h"

namespace Thomas
{
    void TrainControllerSystem::init()
    {
        Signature signature;
        signature.set(ECS::getComponentType<Transform>());
        signature.set(ECS::getComponentType<TrainController>());

        ECS::setSystemSignature<TrainControllerSystem>(signature);
    }

    void TrainControllerSystem::update(float dt)
    {
        for (auto& entity : m_entities)
        {
            Transform& transform           = ECS::getComponent<Transform>(entity);
            BodyComponent& body            = ECS::getComponent<BodyComponent>(entity);
            TrainController& controller    = ECS::getComponent<TrainController>(entity);

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

            body.transform.world = transform.getMat4();

            if (atPos)
            {
                controller.m_currentPoint = std::next(controller.m_currentPoint, 1);
                if (controller.m_currentPoint == controller.m_points.end()) {
                    controller.m_currentPoint = controller.m_points.begin();
                }
            }
            
        }
    }
} // namespace Thomas