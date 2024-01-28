#pragma once

#include <map>
#include <memory>

#include "TrainGame/Engine/Core/Types.h"

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>
#include "TrainGame/App/Objects/GameModel.hpp"

#include <Engine/Utils/Timer.hpp>

class RenderSystem : public Thomas::System
{
public:
    void init();
    void loadModels();

    void update(Renderer& renderer);

    void onEntityAdded(Thomas::Entity entity) override;
    void onEntityRemoved(Thomas::Entity entity) override;

private:
    std::unique_ptr<Entity> m_model;
    std::unordered_map<Thomas::ModelId, std::unique_ptr<Entity>> m_gameModels;

    std::map<Thomas::ModelId, std::vector<Pose>> m_batchedPoses;
    std::map<Thomas::ModelId, std::vector<Material>> m_batchedMaterials;
    std::map<Thomas::Entity, size_t> m_batchIdMap;

    Timer::Chronometre m_timer;
};