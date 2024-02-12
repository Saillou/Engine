#pragma once

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/RenderComponent.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/PlayerController.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/HumanController.h"

struct Player
{
    Player(const glm::vec3& pos)
    {
        m_entity = Thomas::ECS::createEntity();

        Thomas::Transform transform;
        Thomas::RenderComponent render;
        PlayerController controller;
        ColliderComponent collider;

        transform.position = pos;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale = glm::vec3(1.0f);
        
        render.mobility = Thomas::RenderComponent::Mobility::Dynamic;
        render.modelId = Thomas::ModelId::CubeBasic;
        
        collider.collisionBox = std::make_shared<Entity>(Entity::SimpleShape::Cube);
        collider.collisionBox->localPose() = { glm::scale(glm::mat4(1.f), glm::vec3(0.2f)) };
        collider.collisionBox->poses() = { {transform.getMat4() } };

        Thomas::ECS::addComponent(m_entity, transform);
        Thomas::ECS::addComponent(m_entity, render);
        Thomas::ECS::addComponent(m_entity, controller);
        Thomas::ECS::addComponent(m_entity, collider);
    }

    Thomas::Entity entity() const { return m_entity; }

    void enableControls() 
    {
        HumanController humanController;
        Thomas::ECS::addComponent(m_entity, humanController);
    }

    void disableControls()
    {
        Thomas::ECS::removeComponent<HumanController>(m_entity);
    }

private:
    Thomas::Entity m_entity;
};