#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Filter/BaseFilter.hpp>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

#include <Engine/Events/CommonEvents.hpp>

struct View : Event::Subscriber {
    View(Scene& scene);

    Scene& scene();
    bool enable_filter = false;

protected:
    // Events
    void _draw          (const SceneEvents::Draw&);
    void _post_process  (const SceneEvents::PostDraw&);
    void _on_resize     (const SceneEvents::Resized&);
    void _on_mouse_moved(const CommonEvents::MouseMoved&);

private:
    void _initFilters();

    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Entity>> m_interact_objects;

    Scene& m_scene;
    BaseFilter m_filter;
    glm::vec2 m_mousePos;
    Timer::Chronometre m_timer;
};