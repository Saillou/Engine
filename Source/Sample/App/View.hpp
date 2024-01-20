#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

struct View : public BaseScene {
    View(int widthHint = 0, int heightHint = 0);

    void _prepare_draw()          override;
    void _draw_shadow(Shader& sh) override;
    void _draw()                  override;

    void _on_resize()             override;

    void mouse_on(int x, int y);

private:
    struct _Object {
        std::shared_ptr<Entity> entity = nullptr;
        glm::mat4 transform = {};
        Cookable::CookType shade = Cookable::CookType::Basic;
    };

    void _drawText();

    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::vector<_Object> m_objects;
    _Object m_target;

    glm::vec2 m_mousePos;
    Timer::Chronometre m_timer;
};
