#include "RenderSystem.h"

#include "TrainGame/Engine/Core/ECS.h"
#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/RenderComponent.h"

#include "TrainGame/TrainGame/TrainGameModels.hpp"

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
        m_gameModels[Thomas::ModelId::Locomotive].entity = std::make_unique<Entity>("Resources/objects/train/locomotive.glb");
        m_gameModels[Thomas::ModelId::Wagon].entity = std::make_unique<Entity>("Resources/objects/train/wagon_no_wheels.glb");
        m_gameModels[Thomas::ModelId::Track].entity = std::make_unique<Entity>("Resources/objects/train/track_forward.glb");
        m_gameModels[Thomas::ModelId::TrackLeft].entity = std::make_unique<Entity>("Resources/objects/train/track_left.glb");
        m_gameModels[Thomas::ModelId::TrackRight].entity = std::make_unique<Entity>("Resources/objects/train/track_right.glb");
        m_gameModels[Thomas::ModelId::Building1].entity = std::make_unique<Entity>("Resources/objects/train/building_1.glb");
        m_gameModels[Thomas::ModelId::CubeBasic].entity = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_gameModels[Thomas::ModelId::CubeBasic].drawType = Render::DrawType::Basic;
        m_gameModels[Thomas::ModelId::CubeGeometry].entity = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_gameModels[Thomas::ModelId::CubeGeometry].drawType = Render::DrawType::Geometry;
        m_gameModels[Thomas::ModelId::SphereBasic].entity = std::make_unique<Entity>(Entity::SimpleShape::Sphere);
        m_gameModels[Thomas::ModelId::SphereBasic].drawType = Render::DrawType::Basic;
        m_gameModels[Thomas::ModelId::SphereGeometry].entity = std::make_unique<Entity>(Entity::SimpleShape::Sphere);
        m_gameModels[Thomas::ModelId::SphereGeometry].drawType = Render::DrawType::Geometry;
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

            const Thomas::GameModel& gameModel = Thomas::GameModelTable::getModelById(renderComponent.modelId);
            
            m_batchedPoses[renderComponent.modelId][m_batchIdMap[entity]] = { transform.getMat4() * gameModel.transform.model };
            m_batchedMaterials[renderComponent.modelId][m_batchIdMap[entity]] = renderComponent.material;
        }

        for (auto& model : m_gameModels)
        {
            model.second.entity->setPosesWithMaterials(m_batchedPoses[model.first], m_batchedMaterials[model.first]);
            renderer.draw(model.second.drawType, *model.second.entity);
        }

        {
            static float average = 0.f;
            static size_t amount = 0;

            ++amount;
            average += (m_timer.elapsed<Timer::microsecond>() / 1000.f);

            std::ostringstream ss;
            ss << "Render batch models in: " << (m_timer.elapsed<Timer::microsecond>() / 1000.f) << " ms";
            std::string s(ss.str());

            std::ostringstream ss2;
            ss2 << "Render batch average: " << (average / amount) << " ms";
            std::string s2(ss2.str());

            renderer.text(s, 30, 30, 1.f);
            renderer.text(s2, 30, 80, 1.f);
        }
    }

    void RenderSystem::onEntityAdded(Thomas::Entity entity)
    {
        if (m_batchIdMap.find(entity) != m_batchIdMap.end())
            return;

        auto& renderComponent   = Thomas::ECS::getComponent<Thomas::RenderComponent>(entity);
        auto& transform         = Thomas::ECS::getComponent<Thomas::Transform>(entity);

        const Thomas::GameModel& gameModel = Thomas::GameModelTable::getModelById(renderComponent.modelId);

        m_batchedPoses[renderComponent.modelId].push_back(transform.getMat4() * gameModel.transform.model);
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
