#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Filter/BaseFilter.hpp>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

struct View : public BaseScene {
    View(int widthHint = 0, int heightHint = 0);

    void _draw() override;
    void _post_draw() override;
    void _on_resize() override;

    void mouse_on(int x, int y);

    bool enable_filter = false;

private:
    void _initFilters();

    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Entity>> m_scene_objects;

    BaseFilter m_filter;
    glm::vec2 m_mousePos;
    Timer::Chronometre m_timer;
};