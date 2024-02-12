#pragma once

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/RenderComponent.h"

struct Brick
{
    Brick()
    {
        m_entity = Thomas::ECS::createEntity();

        Thomas::Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, -2.0f);
        transform.rotation = glm::vec3(0, 0, 0);
        transform.scale = glm::vec3(1.0f);

        Thomas::RenderComponent render;
        render.mobility = Thomas::RenderComponent::Mobility::Dynamic;
        render.modelId = Thomas::ModelId::CubeBasic;

        Thomas::ECS::addComponent(m_entity, transform);
        Thomas::ECS::addComponent(m_entity, render);
    }

    Thomas::Entity entity() const { return m_entity; }

private:
    Thomas::Entity m_entity;
};