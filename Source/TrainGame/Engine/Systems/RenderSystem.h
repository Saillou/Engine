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
    void onEntityDeleted(Thomas::Entity entity) override;

private:
    struct GameModel
    {
        std::unique_ptr<Entity> entity;
        Render::DrawType        drawType = Render::DrawType::Lights;
    };
    std::unordered_map<Thomas::ModelId, GameModel> m_gameModels;

    std::map<Thomas::ModelId, std::vector<Pose>> m_batchedPoses;
    std::map<Thomas::ModelId, std::vector<Material>> m_batchedMaterials;
    std::map<Thomas::Entity, size_t> m_batchIdMap;
    std::map<size_t, Thomas::Entity> m_positionMap;
    std::map<Thomas::Entity, Thomas::ModelId> m_batchModelMap;

    Timer::Chronometre m_timer;
};