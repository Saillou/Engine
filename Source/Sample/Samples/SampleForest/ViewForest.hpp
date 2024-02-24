#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Framework/Service.hpp>

#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Utils/Timer.hpp>

#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Model.hpp>
#include <Engine/Graphic/Base/Model/Skybox.hpp>

#include "../ManagedEntity.hpp"

struct ViewForest : Event::Subscriber {
     ViewForest();
    
    void mouse_on(int x, int y);

protected:
    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _post_draw(const SceneEvents::PostDraw&);

private:
    enum class _ObjectId {
        Character, Tree, Locomotive,
        Cube, Sphere, Target,
        Grid
    };

    void _initObjects();
    void _initParticles();
    void _setParticles(float dt = 0.0f);

    ManagedEntity& _create_entity(_ObjectId, Model::Ref);
    std::unordered_map<_ObjectId, std::vector<SharedEntity>> m_entities;

    // Scene objects
    std::unique_ptr<Skybox> m_skybox;

    // Fire grid
    struct FireGrid 
    {
        struct Particle 
        {
            SharedEntity entity;
            glm::vec4 speed;
        };

        Model::Ref model;
        const size_t amount;
        std::vector<Particle> particles = {};
    } m_fireGrid;

    glm::vec2 m_mousePos;
    Timer::Chronometre m_timer;
};