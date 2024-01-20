#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

struct View : public BaseScene {
    View(int widthHint = 0, int heightHint = 0);

    void draw() override;
    void mouse_on(int x, int y);

private:
    struct _Object {
        std::shared_ptr<Entity> entity = nullptr;
        glm::vec4 color = {};
        glm::mat4 transform = {};
        Cookable::CookType shade = Cookable::CookType::Basic;
    };

    void _drawText();

    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::vector<_Object> m_objects;
    _Object m_target;

    glm::vec2 m_mousePos;
    ShadowRender m_shadowRender;
    Timer::Chronometre m_timer;
};
