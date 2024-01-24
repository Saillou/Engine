#include "View.hpp"

#include <ctime>
#include <sstream>
#include <algorithm>

#include <Engine/Utils/RayCaster.hpp>

View::View(Scene& scene) :
    m_scene(scene),
    m_mousePos(0.0f, 0.0f)
{
    // Root events
    _subscribe(&View::_draw);
    _subscribe(&View::_on_resize);
    _subscribe(&View::_post_process);
    _subscribe(&View::_on_mouse_moved);

    // Entities
    m_entities["Ground"]    = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Cube"]      = std::make_shared<Entity>(Entity::SimpleShape::Cube);
    m_entities["Target"]    = std::make_shared<Entity>(Entity::SimpleShape::Sphere);
    m_entities["Lantern"]   = std::make_shared<Entity>(Entity::SimpleShape::Sphere);

    // Scene objects
    Material stone = { glm::vec4(0.7f, 0.7f, 0.7f, 1.0f) };
    Material paper = { glm::vec4(0.8f, 0.1f, 0.6f, 0.4f) };
    Material glass = { glm::vec4(0.3f, 1.0f, 1.0f, 0.5f), false };

    m_entities["Ground"]->localMaterial() = stone;
    m_entities["Ground"]->localPose()     = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.01f));
    m_entities["Ground"]->poses()         = { glm::mat4(1.0f) };

    m_entities["Cube"]->localMaterial()   = paper;
    m_entities["Cube"]->localPose()       = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));
    {
        const float n_size = 0.5f;
        const int n_side = 2;
        for (int x = -n_side; x <= n_side; x++) {
            for (int y = -n_side; y <= n_side; y++) {
                for (int z = 0; z <= n_side; z++) {
                    m_entities["Cube"]->poses().push_back(
                        glm::translate(glm::mat4(1.0f), glm::vec3(n_size * x, n_size * y, 0.1f + n_size * z))
                    );
                }
            }
        }
    }

    m_interact_objects = {
        m_entities["Ground"]
    };

    // Decors
    m_entities["Target"]->localMaterial() = glass;
    m_entities["Target"]->localPose()     = glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.03f, 0.03f));

    m_skybox = std::make_unique<Skybox>(std::array<std::string, 6> {
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
    });

    // Lights
    m_entities["Lantern"]->localMaterial().cast_shadow = true;

    // Others render elements
    _initFilters();
}

// Accessors
Scene& View::scene() {
    return m_scene;
}

// Callbacks
void View::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    m_mousePos.x = (float)evt.x / m_scene.width();
    m_mousePos.y = (float)evt.y / m_scene.height();
}

void View::_on_resize(const SceneEvents::Resized& evt) {
    m_filter.resize(m_scene.width(), m_scene.height());
}

void View::_draw(const SceneEvents::Draw&) {
    m_timer.tic();

    _drawLights();
    _drawObjects();

    if (enable_interaction)
        _drawTarget();
}

void View::_drawLights() {
    {
        std::vector<Pose> Qs;
        std::vector<Material> Ms;
        for (const Light& light : m_scene.lights()) {
            Qs.push_back(glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f)));
            Ms.push_back(Material{ light.color });
        }
        m_entities["Lantern"]->setPosesWithMaterials(Qs, Ms);
    }
    m_scene.renderer().draw(Render::DrawType::Basic, *m_entities["Lantern"]);
}

void View::_drawObjects() {
    m_scene.renderer().draw(Render::DrawType::Shadows, *m_entities["Ground"]);
    m_scene.renderer().draw(Render::DrawType::Shadows, *m_entities["Cube"]);
}

void View::_drawTarget() {
    for (auto& obj : m_interact_objects) {
        for (auto& pose : obj->poses()) {
            auto intersect_result = RayCaster::Intersect(m_mousePos, m_scene.camera(), *obj, pose);
            if (!intersect_result.has_value())
                continue;

            const glm::mat4& Q = glm::translate(glm::mat4(1.0f), glm::vec3(intersect_result.value()));
            m_entities["Target"]->poses() = { Q };
            m_scene.renderer().draw(Render::DrawType::Basic, *m_entities["Target"]);
        }
    }
}

void View::_post_process(const SceneEvents::PostDraw&) {
    // Filters
    if (enable_filter) {
        m_filter.shader().use().set("quadTexture", 0);
        m_filter.apply(m_scene.framebuffer_main(), 0);
    }

    // Draw decors
    m_scene.framebuffer_main().bind();
    {
        m_skybox->draw(m_scene.camera());
    }
    m_scene.framebuffer_main().unbind();
}

// Helpers
void View::_initFilters() {
    m_filter.shader()
        .attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_")
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "quadTexture")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                vec2 tex_size   = textureSize(quadTexture, 0); // default -> [1600 x 900]
                vec2 tex_offset = 1.0 / tex_size;
                vec2 tex_id     = TexCoords/tex_offset;

                vec3 seg_color = distance(texture(quadTexture, TexCoords).rgb, vec3(0,0,0)) * vec3(1,1,1);
                FragColor = vec4(seg_color, 1.0);
            )_main_")
        )
        .link();

    m_filter.resize(m_scene.width(), m_scene.height());
}