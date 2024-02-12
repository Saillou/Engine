#pragma once

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/RenderComponent.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Force.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Collider.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Bounce.h"

struct Ball
{
    Ball(const glm::vec3& pos)
    {
        m_entity = Thomas::ECS::createEntity();

        Thomas::Transform transform;
        Thomas::RenderComponent render;
        ColliderComponent collider;
        Force force;
        Bounce bounceComponent;

        transform.position = pos;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale = glm::vec3(1.0f);

        render.mobility = Thomas::RenderComponent::Mobility::Dynamic;
        render.modelId = Thomas::ModelId::SphereBasic;      
        
        collider.collisionBox = std::make_shared<Entity>(Entity::SimpleShape::Sphere);
        collider.collisionBox->localPose() = { glm::scale(glm::mat4(1.f), glm::vec3(0.2f)) };
        collider.collisionBox->poses() = { {transform.getMat4() } };

        Thomas::ECS::addComponent(m_entity, transform);
        Thomas::ECS::addComponent(m_entity, render);
        Thomas::ECS::addComponent(m_entity, force);
        Thomas::ECS::addComponent(m_entity, collider);
        Thomas::ECS::addComponent(m_entity, bounceComponent);
    }

    void setForce(const glm::vec3& force)
    {
        Force& forceComponent = Thomas::ECS::getComponent<Force>(m_entity);
        forceComponent.force = force;
    }

    Thomas::Entity entity() const { return m_entity; }

private:
    Thomas::Entity m_entity;
};