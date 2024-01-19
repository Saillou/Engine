#include "View.hpp"

#include <ctime>
#include <sstream>
#include <algorithm>

#include <glm/gtx/string_cast.hpp>
#include <Engine/Utils/RayCaster.hpp>

View::View(int widthHint, int heightHint):
    BaseScene(widthHint, heightHint),
    m_mousePos(0.0f, 0.0f)
{
    // Camera
    m_camera.position  = glm::vec3(0, -5, 1);
    m_camera.direction = glm::vec3(0,  0, 0);

    // Lightnings
    m_lights = {
        Light(glm::vec3{  0,  -1.50f, 1.0f }, glm::vec4{ 1, 0.7, 0.3, 1 })
    };

    // Entities
    m_entities["Cube"]   = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Sphere"] = std::make_shared<Entity>(Entity::SimpleShape::Sphere);
    m_entities["Tortle"] = std::make_shared<Entity>("Resources/objects/character/character.glb");

    // Objects
    m_objects.push_back(_Object{m_entities["Cube"], glm::vec4(1,1,1,1), 
        glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 0.1f))
    });

    m_objects.push_back(_Object{m_entities["Cube"], glm::vec4(1,0,0,1), 
        glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)), glm::vec3(0,0,1.0f))
    });

    m_objects.push_back(_Object{ m_entities["Tortle"], glm::vec4(1, 0.3f, 0.5f, 1),
        glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0,+0.50f,0.50f)), 1.57f, glm::vec3(1,0,0))
    });

    m_target = _Object{ m_entities["Sphere"], glm::vec4(0.3f, 1, 1, 0.5f),
        glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.03f, 0.03f))
    };
}

// Methods
void View::draw() {
    m_timer.tic();

    // Shadow scene
    BaseScene::Viewport(m_shadowRender.width(), m_shadowRender.height());
    m_shadowRender.render(m_camera, m_lights[0], [=](Shader& sh) {
        for (const _Object& obj : m_objects) {
            obj.entity->model.drawElements(sh);
        }
    });

    // Main scene
    BaseScene::Viewport(width(), height());
    BaseScene::clear();

    // Draw objects
    m_shadowRender.bindTexture(GL_TEXTURE1);
    for (const _Object& obj : m_objects) {
        obj.entity->get(Cookable::CookType::Shadow)->use().set("diffuse_color", obj.color).set("depthMap", 1);
        obj.entity->drawOne(Cookable::CookType::Shadow, m_camera, obj.transform, m_lights);

        // Intersection
        auto intersect_result = RayCaster::Intersect(m_mousePos, m_camera, *obj.entity, obj.transform);
        if (!intersect_result.has_value())
            continue;

        const glm::mat4& Q = glm::translate(glm::mat4(1.0f), glm::vec3(intersect_result.value()));

        m_target.entity->get(Cookable::CookType::Basic)->use().set("diffuse_color", m_target.color);
        m_target.entity->drawOne(Cookable::CookType::Basic, m_camera, Q * m_target.transform);
    }

    // For debug
    _drawText();

    m_timer.tic();
}

// Callbacks
void View::mouse_on(int x, int y) {
    m_mousePos.x = (float)x / m_width;
    m_mousePos.y = (float)y / m_height;
}

void View::_drawText() {
    float dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000.0f;

    TextEngine::Write("Cam: " + glm::to_string(m_camera.position), 
        15.0f, m_height - 20.0f, 0.4f, glm::vec3(1, 1, 1));

    TextEngine::Write("Mouse: " + std::to_string(m_width * m_mousePos.x) + " x " + std::to_string(m_height * m_mousePos.y),
        15.0f, m_height - 40.0f, 0.4f, glm::vec3(1, 1, 1));

    TextEngine::Write("draw: " + std::to_string(dt_draw) + "ms", 
        15.0f, m_height - 60.0f, 0.4f, glm::vec3(1, 1, 1));
}
