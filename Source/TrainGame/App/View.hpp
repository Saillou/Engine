#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>


#include <Engine/Utils/Timer.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Model.hpp>
#include <Engine/Graphic/Base/Model/Skybox.hpp>

struct View : private Event::Subscriber {
    View(Scene& scene);
    ~View();

    Scene& scene;

private:
    // Events
    void _draw(const CommonEvents::StateUpdated&);
    void _post_process(const SceneEvents::PostDraw&);
    void _on_resize(const SceneEvents::Resized&);
    void _on_mouse_moved(const CommonEvents::MouseMoved& evt);

    void _initObjects();

    Entity m_model_sphere;
    Entity m_target;
    Entity m_ground;
    std::unique_ptr<Skybox> m_skybox;

    // Other members
    Timer::Chronometre m_timer;

    glm::vec2 m_mousePos;
};