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
    m_camera.position  = glm::vec3(0, -7, 3);
    m_camera.direction = glm::vec3(0,  0, 0);

    // Lightnings
    m_lights = {
        Light(glm::vec3{  0,  -1.50f, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
        Light(glm::vec3{  0,  +1.50f, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  0,    0,    0.7f }, glm::vec4{ 0.3, 1, 0.7, 1 }),
        Light(glm::vec3{  -1.50f,  0, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  +1.50f,  0, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
    };

    // Entities
    m_entities["Cube"]   = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Sphere"] = std::make_shared<Entity>(Entity::SimpleShape::Sphere);

    // Scene objects
    m_objects = {
        { 
            m_entities["Cube"], glm::vec4(1, 1, 1, 1),
            glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 0.1f)),
            Cookable::CookType::Shadow
        },
    };

    const int n_side = 4;
    for (int x = -n_side; x <= n_side; x++) {
        for (int y = -n_side; y <= n_side; y++) {
            m_objects.push_back({
                m_entities["Cube"], glm::vec4(1, 1, 1, 1),
                glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)), glm::vec3(3.5f*x, 3.5f*y, 1.0f)),
                Cookable::CookType::Basic
            });
        }
    }

    // Decors
    m_target = { 
        m_entities["Sphere"], glm::vec4(0.3f, 1, 1, 0.5f),
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
            obj.entity->model.setBatch({ obj.transform });
            obj.entity->model.drawElements(sh);
        }
    });

    // Main scene
    BaseScene::Viewport(width(), height());
    BaseScene::clear();

    // Draw lights
    for (const Light& light : m_lights) {
        const glm::mat4& Q = glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f));

        m_entities["Sphere"]->get(Cookable::CookType::Basic)->use().set("diffuse_color", light.color);
        m_entities["Sphere"]->drawOne(Cookable::CookType::Basic, m_camera, Q);
    }

    // Draw objects
    m_shadowRender.bindTexture(GL_TEXTURE1);
    for (const _Object& obj : m_objects) {
        obj.entity->get(obj.shade)->use().set("diffuse_color", obj.color);
        obj.entity->drawOne(obj.shade, m_camera, obj.transform, m_lights);

        // Draw intersections
        auto intersect_result = RayCaster::Intersect(m_mousePos, m_camera, *obj.entity, obj.transform);
        if (!intersect_result.has_value())
            continue;

        const glm::mat4& Q = glm::translate(glm::mat4(1.0f), glm::vec3(intersect_result.value()));

        m_target.entity->get(Cookable::CookType::Basic)->use().set("diffuse_color", m_target.color);
        m_target.entity->drawOne(Cookable::CookType::Basic, m_camera, Q * m_target.transform);
    }

    // Draw debug
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
