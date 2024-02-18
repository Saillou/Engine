#include "ViewCube.hpp"

#include <ctime>
#include <sstream>
#include <algorithm>

#include <Engine/Graphic/Window.hpp>
#include <Engine/Framework/Service.hpp>
#include <Engine/Utils/Physic/RayCaster.hpp>

ViewCube::ViewCube():
    m_mousePos(0.0f, 0.0f)
{
    // Root events
    _subscribe(&ViewCube::_draw);
    _subscribe(&ViewCube::_on_resize);
    _subscribe(&ViewCube::_post_process);
    _subscribe(&ViewCube::_on_mouse_moved);

    // Entities
    m_models["Ground"]  = Model::Create(Model::Cube);
    m_models["Cube"]    = Model::Create(Model::Cube);
    m_models["Target"]  = Model::Create(Model::Sphere);
    m_models["Lantern"] = Model::Create(Model::Sphere);

    // Scene objects
    glm::vec4 stone = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    glm::vec4 paper = glm::vec4(0.8f, 0.1f, 0.6f, 0.4f);
    glm::vec4 glass = glm::vec4(0.3f, 1.0f, 1.0f, 0.5f);

    m_models["Ground"]->localMaterial = stone;
    m_models["Ground"]->localTransform     = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.01f));
    m_models["Ground"]->transforms         = { glm::mat4(1.0f) };

    m_models["Cube"]->localMaterial   = paper;
    m_models["Cube"]->localTransform       = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));
    {
        const float n_size = 0.5f;
        const int n_side = 2;
        for (int x = -n_side; x <= n_side; x++) {
            for (int y = -n_side; y <= n_side; y++) {
                for (int z = 0; z <= n_side; z++) {
                    m_models["Cube"]->transforms.push_back(
                        glm::translate(glm::mat4(1.0f), glm::vec3(n_size * x, n_size * y, 0.1f + n_size * z))
                    );
                }
            }
        }
    }

    m_interact_objects = {
        m_models["Ground"]
    };

    // Decors
    m_models["Target"]->localMaterial = glass;
    m_models["Target"]->localTransform     = glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.03f, 0.03f));

    m_skybox = std::make_unique<Skybox>(std::array<std::string, 6> {
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/front.jpg",
    });

    // Others render elements
    _initFilters();
}

// Callbacks
void ViewCube::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    Scene& scene = Service<Window>::get().scene();

    m_mousePos.x = (float)evt.x / scene.width();
    m_mousePos.y = (float)evt.y / scene.height();
}

void ViewCube::_on_resize(const SceneEvents::Resized& evt) {
    Scene& scene = Service<Window>::get().scene();

    m_filter.resize(scene.width(), scene.height());
}

void ViewCube::_draw(const SceneEvents::Draw&) {
    m_timer.tic();

    _drawLights();
    _drawObjects();

    if (enable_interaction)
        _drawTarget();
}

void ViewCube::_drawLights() {
    Scene& scene = Service<Window>::get().scene();

    {
        std::vector<glm::mat4> Qs;
        std::vector<glm::vec4> Ms;
        for (const Light& light : scene.lights()) {
            Qs.push_back(glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f)));
            Ms.push_back(glm::vec4{ light.color });
        }
        m_models["Lantern"]->transforms = Qs;
        m_models["Lantern"]->materials = Ms;
    }
    scene.renderer().draw(Render::DrawType::Basic, m_models["Lantern"]);
}

void ViewCube::_drawObjects() {
    Scene& scene = Service<Window>::get().scene();

    scene.renderer().draw(Render::DrawType::Shadows, m_models["Ground"]);
    scene.renderer().draw(Render::DrawType::Shadows, m_models["Cube"]);
}

void ViewCube::_drawTarget() {
    Scene& scene = Service<Window>::get().scene();

    for (auto& obj : m_interact_objects) {
        for (auto& pose : obj->transforms) {
            auto intersect_result = RayCaster::Intersect(m_mousePos, scene.camera(), obj, pose);
            if (!intersect_result.has_value())
                continue;

            const glm::mat4& Q = glm::translate(glm::mat4(1.0f), glm::vec3(intersect_result.value()));
            m_models["Target"]->transforms = { Q };
            scene.renderer().draw(Render::DrawType::Basic, m_models["Target"]);
        }
    }
}

void ViewCube::_post_process(const SceneEvents::PostDraw&) {
    Scene& scene = Service<Window>::get().scene();

    // Filters
    if (enable_filter) {
        m_filter.shader().use().set("quadTexture", 0);
        m_filter.apply(scene.framebuffer_main(), 0);
    }

    // Draw decors
    scene.framebuffer_main().bind();
    {
        m_skybox->draw(scene.camera());
    }
    scene.framebuffer_main().unbind();
}

// Helpers
void ViewCube::_initFilters() {
    Scene& scene = Service<Window>::get().scene();

    m_filter.shader()
        .attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
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

    m_filter.resize(scene.width(), scene.height());
}