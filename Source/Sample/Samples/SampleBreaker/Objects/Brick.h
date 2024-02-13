#pragma once

#include <Engine/Framework/Core/ECS.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/RenderComponent.h"

struct Brick
{
    Brick()
    {
        m_entity = ECS::createEntity();

        Breaker::Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, -2.0f);
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale = glm::vec3(1.0f);

        Breaker::RenderComponent render;
        render.mobility = Breaker::RenderComponent::Mobility::Dynamic;
        render.modelId = Breaker::ModelId::CubeBasic;

        ECS::addComponent(m_entity, transform);
        ECS::addComponent(m_entity, render);
    }

    Entity entity() const { return m_entity; }

private:
    Entity m_entity;
};