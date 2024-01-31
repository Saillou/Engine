#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>


#include <Engine/Events/CommonEvents.hpp>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Skybox.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

#include <Engine/Utils/Timer.hpp>

#include "TrainGame/Engine/Systems/RenderSystem.h"

    struct View : private Event::Subscriber {
        View(Scene& scene);

        Scene& scene;

    private:
        // Events
        void _draw(const SceneEvents::Draw&);
        void _post_process(const SceneEvents::PostDraw&);
        void _on_resize(const SceneEvents::Resized&);
        void _on_mouse_moved(const CommonEvents::MouseMoved& evt);

        struct _EntityModel {
            Thomas::Transform transform;
            std::unique_ptr<Entity> entity;
        };

        void _initObjects();

        std::unique_ptr<Entity> m_model_sphere;
        std::unique_ptr<Entity> m_target;
        _EntityModel m_groundEntity;
        std::unique_ptr<Skybox> m_skybox;

        // Other members
        Timer::Chronometre m_timer;

        glm::vec2 m_mousePos;
        std::shared_ptr<RenderSystem> m_renderSystem;
    };