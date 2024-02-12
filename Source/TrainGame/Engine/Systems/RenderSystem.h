#pragma once

#include <map>
#include <memory>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Framework/ECS/ECS.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Model.hpp>

#include "TrainGame/App/Objects/GameModel.hpp"


class RenderSystem : public System
{
public:
    void init();
    void loadModels();

    void update(Renderer& renderer);

    void addEntity(Entity entity) override;
    void removeEntity(Entity entity) override;

private:
    struct GameModel
    {
        Model::Ref model;
        Render::DrawType drawType = Render::DrawType::Lights;
    };
    std::unordered_map<Thomas::ModelId, GameModel> m_gameModels;

    std::map<Thomas::ModelId, std::vector<Pose>> m_batchedPoses;
    std::map<Thomas::ModelId, std::vector<Material>> m_batchedMaterials;
    std::map<Entity, size_t> m_batchIdMap;
    std::map<size_t, Entity> m_positionMap;
    std::map<Entity, Thomas::ModelId> m_batchModelMap;

    Timer::Chronometre m_timer;
};