#pragma once

#include <Engine/Framework/Core/ECS.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/RenderComponent.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/PlayerController.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/HumanController.h"

struct Player
{
    Player(const glm::vec3& pos)
    {
        m_entity = ECS::createEntity();

        Breaker::Transform transform;
        Breaker::RenderComponent render;
        PlayerController controller;
        ColliderComponent collider;

        transform.position = pos;
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale = glm::vec3(1.0f);
        
        render.mobility = Breaker::RenderComponent::Mobility::Dynamic;
        render.modelId = Breaker::ModelId::CubeBasic;
        
        collider.collisionBox = Model::Create(Model::Cube);
        collider.collisionBox->localTransform = { glm::scale(glm::mat4(1.f), glm::vec3(0.2f)) };
        collider.collisionBox->transforms = { {transform.getMat4() } };

        ECS::addComponent(m_entity, transform);
        ECS::addComponent(m_entity, render);
        ECS::addComponent(m_entity, controller);
        ECS::addComponent(m_entity, collider);
    }

    Entity entity() const { return m_entity; }

    void enableControls() 
    {
        HumanController humanController;
        ECS::addComponent(m_entity, humanController);
    }

    void disableControls()
    {
        ECS::removeComponent<HumanController>(m_entity);
    }

private:
    Entity m_entity;
};