#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Filter/BaseFilter.hpp>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>
#include <Engine/Graphic/Base/Model/Skybox.hpp>

#include <Engine/Events/CommonEvents.hpp>

struct ViewCube : Event::Subscriber {
    ViewCube();

    bool enable_filter = false;
    bool enable_interaction = false;

protected:
    // Events
    void _draw          (const SceneEvents::Draw&);
    void _post_process  (const SceneEvents::PostDraw&);
    void _on_resize     (const SceneEvents::Resized&);
    void _on_mouse_moved(const CommonEvents::MouseMoved&);

private:
    void _initFilters();

    void _drawLights();
    void _drawObjects();
    void _drawTarget();

    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Entity>> m_interact_objects;
    std::unique_ptr<Skybox> m_skybox;

    BaseFilter m_filter;
    glm::vec2 m_mousePos;
    Timer::Chronometre m_timer;
};