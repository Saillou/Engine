#pragma once

#include <map>
#include <memory>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Model.hpp>

#include "../Components/RenderComponent.h"


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
    std::map<Breaker::ModelId, GameModel> m_gameModels;
    std::map<Breaker::ModelId, std::vector<glm::mat4>> m_batchedPoses;
    std::map<Breaker::ModelId, std::vector<glm::vec4>> m_batchedMaterials;
    std::map<Entity, size_t> m_batchIdMap;
    std::map<size_t, Entity> m_positionMap;
    std::map<Entity, Breaker::ModelId> m_batchModelMap;

    Timer::Chronometre m_timer;
};