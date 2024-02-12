#include "RenderSystem.h"

#include "../Core/ECS.h"
#include "../Components/Transform.h"
#include "../Components/RenderComponent.h"

RenderSystem::RenderSystem()
{
    init();
}

void RenderSystem::init()
    {
        Thomas::Signature signature;
        signature.set(Thomas::ECS::getComponentType<Thomas::Transform>());
        signature.set(Thomas::ECS::getComponentType<Thomas::RenderComponent>());

        Thomas::ECS::setSystemSignature<RenderSystem>(signature);

        loadModels();
    }

    void RenderSystem::loadModels()
    {
        m_gameModels[Thomas::ModelId::CubeBasic].entity = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_gameModels[Thomas::ModelId::CubeBasic].drawType = Render::DrawType::Basic;
        m_gameModels[Thomas::ModelId::CubeBasic].localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));

        m_gameModels[Thomas::ModelId::SphereBasic].entity = std::make_unique<Entity>(Entity::SimpleShape::Sphere);
        m_gameModels[Thomas::ModelId::SphereBasic].drawType = Render::DrawType::Basic;
        m_gameModels[Thomas::ModelId::SphereBasic].localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    }

    void RenderSystem::update(Renderer& renderer)
    {
        m_timer.tic();
        
        for (auto& entity : m_entities)
        {
            Thomas::Transform& transform                = Thomas::ECS::getComponent<Thomas::Transform>(entity);
            Thomas::RenderComponent& renderComponent    = Thomas::ECS::getComponent<Thomas::RenderComponent>(entity);

            if (renderComponent.mobility == Thomas::RenderComponent::Mobility::Static)
                continue;

            m_batchedPoses[renderComponent.modelId][m_batchIdMap[entity]] = { 
                transform.getMat4() * m_gameModels[renderComponent.modelId].localTransform
            };
            m_batchedMaterials[renderComponent.modelId][m_batchIdMap[entity]] = renderComponent.material;
        }

        for (auto& model : m_gameModels)
        {
            model.second.entity->setPosesWithMaterials(m_batchedPoses[model.first], m_batchedMaterials[model.first]);
            renderer.draw(model.second.drawType, *model.second.entity);
        }
    }

    void RenderSystem::onEntityAdded(Thomas::Entity entity)
    {
        if (m_batchIdMap.find(entity) != m_batchIdMap.end())
            return;

        auto& renderComponent   = Thomas::ECS::getComponent<Thomas::RenderComponent>(entity);
        auto& transform         = Thomas::ECS::getComponent<Thomas::Transform>(entity);

        m_batchedPoses[renderComponent.modelId].push_back(
            transform.getMat4() * m_gameModels[renderComponent.modelId].localTransform
        );
        m_batchedMaterials[renderComponent.modelId].push_back(renderComponent.material);

        m_batchIdMap[entity] = m_batchedPoses[renderComponent.modelId].size() - 1;
        m_batchModelMap[entity] = renderComponent.modelId;
        m_positionMap[m_batchedPoses[renderComponent.modelId].size() - 1] = entity;
    }

    void RenderSystem::onEntityRemoved(Thomas::Entity entity)
    {
        if (m_batchIdMap.find(entity) == m_batchIdMap.end())
            return;

        const Thomas::ModelId modelId = m_batchModelMap[entity];
        const size_t position = m_batchIdMap[entity];
        const bool isLast = position == m_batchedPoses[modelId].size() - 1;

        if (isLast)
        {
            m_batchedPoses[modelId].pop_back();
            m_batchedMaterials[modelId].pop_back();
            m_batchIdMap.erase(entity);
        }
        else
        {
            const size_t indexOfLast = m_batchedPoses[modelId].size() - 1;
            const Thomas::Entity lastEntity = m_positionMap[indexOfLast];

            m_batchedPoses[modelId][position]       = m_batchedPoses[modelId][indexOfLast];
            m_batchedMaterials[modelId][position]   = m_batchedMaterials[modelId][indexOfLast];
            m_batchIdMap[lastEntity] = position;
            m_positionMap[position] = lastEntity;

            m_batchedPoses[modelId].pop_back();
            m_batchedMaterials[modelId].pop_back();
            m_batchIdMap.erase(entity);
        }
    }

    void RenderSystem::onEntityDeleted(Thomas::Entity entity)
    {
        onEntityRemoved(entity);
    }
