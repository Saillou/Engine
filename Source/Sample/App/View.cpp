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
    camera().position = glm::vec3(0, -7, 3);
    camera().direction = glm::vec3(0,  0, 0);

    // Lightnings
    lights() = {
        Light(glm::vec3{  0,  -1.50f, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
        Light(glm::vec3{  0,  +1.50f, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  0,    0,    0.7f }, glm::vec4{ 0.3, 1, 0.7, 1 }),
        Light(glm::vec3{  -1.50f,  0, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  +1.50f,  0, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
    };

    // Entities
    m_entities["Ground"]  = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Cube"]    = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Sphere"]  = std::make_shared<Entity>(Entity::SimpleShape::Sphere);
    m_entities["Lantern"] = std::make_shared<Entity>(Entity::SimpleShape::Sphere);

    // Scene objects
    Material stone = { glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) };
    Material metal = { glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };
    Material glass = { glm::vec4(0.3f, 1.0f, 1.0f, 0.5f) };

    m_entities["Ground"]->material = stone;
    m_objects = {{ 
        m_entities["Ground"], glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.1f)),
    }};
    m_entities["Ground"]->model.setBatch({ glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.1f)) });

    m_entities["Cube"]->material = stone;
    {
        std::vector<glm::mat4> _cubeBatch;
        const int n_side = 1;
        for (int x = -n_side; x <= n_side; x++) {
            for (int y = -n_side; y <= n_side; y++) {
                _cubeBatch.push_back(
                    glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)), glm::vec3(10.0 * x, 10.0f * y, 1.0f))
                );

                m_objects.push_back({
                    m_entities["Cube"], _cubeBatch.back(),
                });
            }
        }

        m_entities["Cube"]->model.setBatch(_cubeBatch);
    }

    // Decors
    m_entities["Sphere"]->material = glass;
    m_target = { 
        m_entities["Sphere"], glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.03f, 0.03f))
    };
}

// Callbacks
void View::_on_resize() {
    // ..
}

void View::mouse_on(int x, int y) {
    m_mousePos.x = (float)x / m_width;
    m_mousePos.y = (float)y / m_height;
}

// Methods
void View::_prepare_draw() {
    // ..
}

void View::_draw_shadow(Shader& sh) {
    m_entities["Ground"]->model.drawElements(sh);
    m_entities["Cube"]->model.drawElements(sh);
}

void View::_draw() {
    m_timer.tic();

    // Main scene
    BaseScene::clear();

    // Draw lights
    for (const Light& light : lights()) {
        const glm::mat4& Q = glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f));

        m_entities["Lantern"]->material.diffuse_color = light.color;
        renderer().drawOne(Render::DrawType::Basic, *m_entities["Lantern"], Q);
    }

    // Draw objects
    renderer().draw(Render::DrawType::Shadows, *m_entities["Ground"]);
    renderer().draw(Render::DrawType::Shadows, *m_entities["Cube"]);

    // Draw objects
    for (const _Object& obj : m_objects) {
        // Draw intersections
        auto intersect_result = RayCaster::Intersect(m_mousePos, camera(), *obj.entity, obj.transform);
        if (!intersect_result.has_value())
            continue;

        const glm::mat4& Q = glm::translate(glm::mat4(1.0f), glm::vec3(intersect_result.value()));
        renderer().drawOne(Render::DrawType::Basic, *m_target.entity, Q * m_target.transform);
    }

    // Draw debug
    _drawText();
}

void View::_drawText() {
    float dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000.0f;

    TextEngine::Write("Cam: " + glm::to_string(camera().position),
        15.0f, m_height - 20.0f, 0.4f, glm::vec3(1, 1, 1));

    TextEngine::Write("Mouse: " + std::to_string(m_width * m_mousePos.x) + " x " + std::to_string(m_height * m_mousePos.y),
        15.0f, m_height - 40.0f, 0.4f, glm::vec3(1, 1, 1));

    TextEngine::Write("draw: " + std::to_string(dt_draw) + "ms", 
        15.0f, m_height - 60.0f, 0.4f, glm::vec3(1, 1, 1));
}
