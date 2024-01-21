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
    camera().position  = glm::vec3(0, -4, 0.25f);
    camera().direction = glm::vec3(0,  0, 0);

    // Entities
    m_entities["Ground"]  = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Cube"]    = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Box"]     = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Target"]  = std::make_shared<Entity>(Entity::SimpleShape::Sphere);
    m_entities["Lantern"] = std::make_shared<Entity>(Entity::SimpleShape::Sphere);

    // Scene objects
    Material stone = { glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) };
    Material paper = { glm::vec4(0.3f, 1.0f, 1.0f, 0.2f) };
    Material glass = { glm::vec4(0.3f, 1.0f, 1.0f, 0.5f), false };

    m_entities["Ground"]->localMaterial() = stone;
    m_entities["Ground"]->setLocalPose(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.1f)));
    m_entities["Ground"]->setPoses({ glm::mat4(1.0f) });

    m_entities["Box"]->localMaterial() = stone;
    m_entities["Box"]->setPoses({ glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)), glm::vec3(0, 0, 1.0f)) });

    m_entities["Cube"]->localMaterial() = paper;
    {
        std::vector<Pose> poses;
        const int n_side = 1;
        for (int x = -n_side; x <= n_side; x++) {
            for (int y = -n_side; y <= n_side; y++) {
                if (x == 0 && y == 0)
                    continue;

                poses.push_back(
                    glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)), glm::vec3(10.0 * x, 10.0f * y, 1.0f))
                );
            }
        }
        m_entities["Cube"]->setPoses(poses);
    }

    m_scene_objects = { 
        m_entities["Ground"], 
        m_entities["Cube"] 
    };

    // Decors
    m_entities["Target"]->localMaterial() = glass;
    m_entities["Target"]->setLocalPose(glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.03f, 0.03f)));

    // Lights
    m_entities["Lantern"]->localMaterial().cast_shadow = true;
}

// Callbacks
void View::mouse_on(int x, int y) {
    m_mousePos.x = (float)x / m_width;
    m_mousePos.y = (float)y / m_height;
}

// Methods
void View::_draw() {
    m_timer.tic();

    // Draw lights
    {
        std::vector<Pose> Qs;
        std::vector<Material> Ms;
        for (const Light& light : lights()) {
            Qs.push_back(glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f)));
            Ms.push_back(Material{ light.color });
        }
        m_entities["Lantern"]->setPosesWithMaterials(Qs, Ms);
    }
    renderer().draw(Render::DrawType::Basic, *m_entities["Lantern"]);

    // Draw objects
    renderer().draw(Render::DrawType::Shadows, *m_entities["Ground"]);
    renderer().draw(Render::DrawType::Shadows, *m_entities["Cube"]);
    renderer().draw(Render::DrawType::Shadows, *m_entities["Box"]);


    // Draw intersections
    for (auto& obj : m_scene_objects) {
        for (auto& pose : obj->poses()) {
            auto intersect_result = RayCaster::Intersect(m_mousePos, camera(), *obj, pose);
            if (!intersect_result.has_value())
                continue;

            const glm::mat4& Q = glm::translate(glm::mat4(1.0f), glm::vec3(intersect_result.value()));
            m_entities["Target"]->setPoses({ Q });
            renderer().draw(Render::DrawType::Basic, *m_entities["Target"]);
        }
    }

    // Draw debug texts
    float dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000.0f;

    renderer().text("Cam: "   + glm::to_string(camera().position), 15.0f, m_height - 20.0f, 0.4f);
    renderer().text("Mouse: " + std::to_string(m_width * m_mousePos.x) + " x " + std::to_string(m_height * m_mousePos.y), 15.0f, m_height - 40.0f, 0.4f);
}
