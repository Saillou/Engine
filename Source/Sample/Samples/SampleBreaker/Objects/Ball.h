#pragma once

#include <Engine/Framework/Core/System.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/RenderComponent.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Force.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Collider.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Bounce.h"

struct Ball
{
    Ball(const glm::vec3& pos)
    {
        m_entity = ECS::createEntity();

        Breaker::Transform transform;
        Breaker::RenderComponent render;
        ColliderComponent collider;
        Force force;
        Bounce bounceComponent;

        transform.position = pos;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale = glm::vec3(1.0f);

        render.mobility = Breaker::RenderComponent::Mobility::Dynamic;
        render.modelId = Breaker::ModelId::SphereBasic;      
        
        collider.collisionBox = Model::Create(Model::Sphere);
        collider.collisionBox->localTransform = { glm::scale(glm::mat4(1.f), glm::vec3(0.2f)) };
        collider.collisionBox->transforms = { {transform.getMat4() } };

        ECS::addComponent(m_entity, transform);
        ECS::addComponent(m_entity, render);
        ECS::addComponent(m_entity, force);
        ECS::addComponent(m_entity, collider);
        ECS::addComponent(m_entity, bounceComponent);
    }

    void setForce(const glm::vec3& force)
    {
        Force& forceComponent = ECS::getComponent<Force>(m_entity);
        forceComponent.force = force;
    }

    Entity entity() const { return m_entity; }

private:
    Entity m_entity;
};