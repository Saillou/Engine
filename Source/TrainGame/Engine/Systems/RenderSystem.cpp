#include "RenderSystem.h"

#include "TrainGame/Engine/Core/ECS.h"
#include "TrainGame/Engine/Components/Transform2.h"
#include "TrainGame/Engine/Components/RenderComponent.h"

#include "TrainGame/TrainGame/TrainGameModels.hpp"

    void RenderSystem::init()
    {
        m_model = std::make_unique<Entity>("Resources/objects/train/locomotive.glb");
        loadModels();
    }

    void RenderSystem::loadModels()
    {
        m_gameModels[Thomas::ModelId::Locomotive] = std::make_unique<Entity>("Resources/objects/train/locomotive.glb");
        m_gameModels[Thomas::ModelId::Wagon] = std::make_unique<Entity>("Resources/objects/train/wagon_no_wheels.glb");
        m_gameModels[Thomas::ModelId::Track] = std::make_unique<Entity>("Resources/objects/train/track_forward.glb");
        m_gameModels[Thomas::ModelId::TrackLeft] = std::make_unique<Entity>("Resources/objects/train/track_left.glb");
        m_gameModels[Thomas::ModelId::TrackRight] = std::make_unique<Entity>("Resources/objects/train/track_right.glb");
        m_gameModels[Thomas::ModelId::Building1] = std::make_unique<Entity>("Resources/objects/train/building_1.glb");
        m_gameModels[Thomas::ModelId::CubeBasic] = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_gameModels[Thomas::ModelId::CubeGeometry] = std::make_unique<Entity>(Entity::SimpleShape::Cube);
    }

    void RenderSystem::update(Renderer& renderer)
    {
        m_timer.tic();
        

        for (auto& entity : m_entities)
        {
            Thomas::Transform2& transform               = Thomas::ECS::getComponent<Thomas::Transform2>(entity);
            Thomas::RenderComponent& renderComponent    = Thomas::ECS::getComponent<Thomas::RenderComponent>(entity);

            if (renderComponent.mobility == Thomas::RenderComponent::Mobility::Static)
                continue;

            const Thomas::GameModel& gameModel = Thomas::GameModelTable::getModelById(renderComponent.modelId);
            
            m_batchedPoses[renderComponent.modelId][m_batchIdMap[entity]] = { transform.getMat4() * gameModel.transform.model };
            //m_batchedMaterials[renderComponent.modelId][m_batchIdMap[entity]] = ...;
        }

        for (auto& model : m_gameModels)
        {
            model.second->setPosesWithMaterials(m_batchedPoses[model.first], m_batchedMaterials[model.first]);
            renderer.draw(Render::DrawType::Shadows, *model.second);
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
        auto& transform         = Thomas::ECS::getComponent<Thomas::Transform2>(entity);

        const Thomas::GameModel& gameModel = Thomas::GameModelTable::getModelById(renderComponent.modelId);

        m_batchedPoses[renderComponent.modelId].push_back(transform.getMat4() * gameModel.transform.model);
        m_batchedMaterials[renderComponent.modelId].push_back(m_gameModels[renderComponent.modelId]->localMaterial());

        m_batchIdMap[entity] = m_batchedPoses[renderComponent.modelId].size() - 1;
    }

    void RenderSystem::onEntityRemoved(Thomas::Entity entity)
    {
        // TODO: add remove
    }
