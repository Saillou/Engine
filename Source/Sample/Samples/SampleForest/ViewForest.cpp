#include "ViewForest.hpp"

#include <ctime>
#include <random>
#include <sstream>
#include <algorithm>

#include <glm/gtx/string_cast.hpp>

#include <Engine/Utils/Physic/RayCaster.hpp>

// Random engine
static std::default_random_engine gen;
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

ViewForest::ViewForest() :
    m_fireGrid({
        glm::vec3(0, 0, 0),
        {
            size_t(2500),
            Model::Create(Model::Cube)
        }
    }),
    m_mousePos(0.0f, 0.0f)
{
    // Load models
    m_timer.tic();
    {
        m_models[_ObjectId::Locomotive] = Model::Create("Resources/objects/train/locomotive.glb");
        m_models[_ObjectId::Tree]       = Model::Create("Resources/objects/tree/tree.glb");
        m_models[_ObjectId::Character]  = Model::Create("Resources/objects/character/character.glb");

        m_models[_ObjectId::Grid]   = Model::Create(Model::Cube);
        m_models[_ObjectId::Cube]   = Model::Create(Model::Cube);
        m_models[_ObjectId::Sphere] = Model::Create(Model::Sphere);
        m_models[_ObjectId::Target] = Model::Create(Model::Sphere);
    }
    std::cout << "Models loaded in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Populate objects
    m_timer.tic();
    {
        _initObjects();
        _initFilters();
    }
    std::cout << "Objects initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Create particules
    m_timer.tic();
    {
        _initParticles();
    }
    std::cout << "Particules initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Enable events
    _subscribe(&ViewForest::_draw);
    _subscribe(&ViewForest::_post_draw);
    _subscribe(&ViewForest::_on_resize);
}

// Methods
void ViewForest::_draw(const SceneEvents::Draw&) {
    Scene& scene = Service<Window>::get().scene();

    static float dt_since_last_draw = 0.0f;

    dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();

    _setParticles(dt_since_last_draw);
    _setObjects();

    static float dt_draw = 0.0f;
    m_timer.tic();

    // Lights
    {
        m_models[_ObjectId::Sphere]->transforms.clear();
        m_models[_ObjectId::Sphere]->materials.clear();

        for (auto& light : scene.lights()) {
            const glm::mat4& Q = glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f));

            m_models[_ObjectId::Sphere]->transforms.push_back(Q);
            m_models[_ObjectId::Sphere]->materials.push_back(light.color);
        }
        scene.renderer().draw(Render::DrawType::Basic, m_models[_ObjectId::Sphere]);
    }

    // List of targets
    std::vector<glm::mat4> targets;

    // Draw objects
    for (const _Object& obj : m_objects) {
        scene.renderer().draw(Render::DrawType::Shadows, m_models[obj.id]);
    }

    // Ray cast
    for (const _Object& obj : m_objects) {
        for (const auto& quat : obj.quats) {
            auto cast_res = RayCaster::Intersect(m_mousePos, scene.camera(), m_models[obj.id], quat);
            if (!cast_res)
                continue;

            // Highlight
            m_models[obj.id]->localMaterial = glm::vec4(0.2f, 0.7f, 0.7f, 1);
            m_models[obj.id]->transforms = { quat };
            scene.renderer().draw(Render::DrawType::Geometry, m_models[obj.id]);

            // Intersection
            targets.push_back(
                glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(cast_res.value())), glm::vec3(0.1f, 0.1f, 0.01f))
            );
        }
    }

    // Draw targets
    if (!targets.empty()) {
        m_models[_ObjectId::Target]->transforms = targets;
        m_models[_ObjectId::Target]->localMaterial = glm::vec4(0.2f, 1.0f, 0.7f, 0.5f);
        scene.renderer().draw(Render::DrawType::Basic, m_models[_ObjectId::Target]);
    }

    // Particles
    {
        scene.renderer().draw(Render::DrawType::Basic, m_fireGrid.particles.object);
    }

    // Draw ground
    {
        // Grid
        {
            m_models[_ObjectId::Grid]->localMaterial = glm::vec4(0.2f, 0.2f, 0.2f, 1);
            scene.renderer().draw(Render::DrawType::Geometry, m_models[_ObjectId::Grid]);
        }

        // Shadow
        {
            m_models[_ObjectId::Grid]->localMaterial = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
            scene.renderer().draw(Render::DrawType::Shadows, m_models[_ObjectId::Grid]);
        }
    }

    // Prepare next
    dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();
}

void ViewForest::_post_draw(const SceneEvents::PostDraw&) {
    Scene& scene = Service<Window>::get().scene();

    // Skybox
    scene.framebuffer_main().bind();
    {
        m_skybox->draw(scene.camera());
    }
    scene.framebuffer_main().unbind();

    // Apply filter
    if (enable_filter) {
        m_filter.apply(scene.framebuffer_main());
        scene.drawFrame(m_filter.frame());
    }
    else {
        scene.drawFrame(scene.framebuffer_main());
    }

    // Debug texts
    scene.renderer().text("Cam: " + glm::to_string(scene.camera().position), 15.0f, scene.height() - 20.0f, 0.4f);
    scene.renderer().text("Mouse: " + std::to_string(scene.width() * m_mousePos.x) + " x " + std::to_string(scene.height() * m_mousePos.y), 15.0f, scene.height() - 40.0f, 0.4f);
}

// Allocate static memory
void ViewForest::_initObjects() {
    constexpr float hpi = glm::half_pi<float>();

    // Sky
    m_skybox = std::make_unique<Skybox>(std::array<std::string, 6> {
        "Resources/textures/skybox/right.jpg",
        "Resources/textures/skybox/left.jpg",
        "Resources/textures/skybox/top.jpg",
        "Resources/textures/skybox/bottom.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/back.jpg"
    });

    // Ground - Grid
    m_grid = std::make_unique<_Grid>(_Grid{ 0.3f, 36, {} });
    m_grid->grid_cells.resize(size_t(m_grid->n_side * m_grid->n_side));
    std::generate(m_grid->grid_cells.begin(), m_grid->grid_cells.end(), [id = 0, S = m_grid->cell_size, N = m_grid->n_side]() mutable
        {
            const glm::vec2 cell_pos = glm::vec2(id % N - N / 2, id / N - N / 2);
            const glm::vec3 T_pos = S * glm::vec3(cell_pos, -0.5f);
            const glm::vec3 T_scale = S * glm::vec3(0.5f);

            id++;
            return glm::scale(glm::translate(glm::mat4(1.0f), T_pos), T_scale);
        }
    );

    // Box
    m_objects.push_back({ _ObjectId::Cube, glm::vec4(1.0f, 0.7f, 0.3f ,1.0),
        {
            glm::scale(
                glm::translate(
                    glm::mat4(1.0f), glm::vec3(0.3f, 0, 0.10f)),
                glm::vec3(0.1f)
            )
        }
    });

    // Locomotive
    m_objects.push_back({ _ObjectId::Locomotive, glm::vec4(0),
        {
            glm::scale(
                glm::translate(
                    glm::rotate(glm::mat4(1.0f),    // Identity
                        hpi, glm::vec3(1, 0, 0)),  // Rotation
                    glm::vec3(1.0f, .30f, .0f)),    // Translation
                glm::vec3(2.0f)                     // Scale
            )
        }
    });

    // Trees
    std::vector<glm::mat4> forest;
    forest.resize(100);
    std::generate(forest.begin(), forest.end(), [id = 0, hpi]() mutable
        {
            float x = (float)(id % 10 - 5);
            float y = (float)(id / 10 - 5);

            if (abs(x) < 2 && abs(y) < 2) {
                x = (x > 0 ? 1 : -1) * std::max(3.0f, abs(x)) + x / 10.0f;
                y = (y > 0 ? 1 : -1) * std::max(3.0f, abs(y)) + y / 10.0f;
            }

            glm::vec3 tr(-0.90f + x, 0.10f, -0.95f + y);  // Translation

            auto transf = glm::scale(
                glm::translate(
                    glm::rotate(glm::mat4(1.0f),    // Identity
                        hpi, glm::vec3(1, 0, 0)),  // Rotation
                    tr),                    // Translation
                glm::vec3(dstr_one(gen) * 3.0f)       // Scale
            );

            id++;
            return transf;
        }
    );
    m_objects.push_back({ _ObjectId::Tree, glm::vec4(0), forest });

    // Character
    m_objects.push_back({ _ObjectId::Character, glm::vec4(0),
        {
            glm::translate(
                glm::rotate(glm::mat4(1.0f),  // Identity
                    hpi, glm::vec3(1, 0, 0)), // Rotation
                glm::vec3(-1, 0.2f, -1.2f)       // Translation
            )
        }
    });
}

void ViewForest::_initFilters() {
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

                if(int(tex_id.x) % 3 != 0 || int(tex_id.y) % 2 != 0) {
                    FragColor = vec4(0, 0, 0, 1);
                    return;
                }

                vec4 texture_color = texture(quadTexture, TexCoords);
                vec3 night_color = vec3(0.0, 0.5f*texture_color.r + 0.5f*texture_color.g, texture_color.b);
                FragColor = vec4(night_color.rgb, texture_color.a);
            )_main_")
        )
        .link();
}

void ViewForest::_initParticles() {
    // - Generate batch parameters
    const glm::vec3 color(1.0f, 0.7f, 0.3f);

    // Define particles
    m_fireGrid.particles.models.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.speeds.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.materials.resize(m_fireGrid.particles.amount);

    // Create batch
    std::generate(m_fireGrid.particles.materials.begin(), m_fireGrid.particles.materials.end(), [&, particules_id = 0]() mutable -> glm::vec4
        {
            particules_id++;
            float ratio = particules_id / float(m_fireGrid.particles.amount);

            return glm::min(glm::vec4(1.5f * ratio * color, 0.0f) + glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    );
}


// Update dynamic memory
void ViewForest::_setParticles(float dt) {
    // Move
    for (int particules_id = 0; particules_id < m_fireGrid.particles.amount; particules_id++) {
        glm::vec4& speed = m_fireGrid.particles.speeds[particules_id];
        glm::mat4& model = m_fireGrid.particles.models[particules_id];

        const bool hasEnded = model[0][0] < 1e-4f || model[1][1] < 1e-4f || model[2][2] < 1e-4f; // also true for first draw

        if (hasEnded) {
            const int SIZE = (int)sqrt(m_fireGrid.particles.amount);
            int x = particules_id % SIZE - SIZE / 2;
            int y = particules_id / SIZE - SIZE / 2;

            model = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
            model = glm::translate(model, glm::vec3(x * 0.7f, 50.0f, 25.0f + y * 0.5f));
            speed = glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen), 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
        }
        else {
            model = glm::scale(glm::translate(model, m_fireGrid.pos + dt * 100.0f * glm::vec3(speed)), glm::vec3(speed.a));
        }
    }

    // Update
    m_fireGrid.particles.object->transforms     = m_fireGrid.particles.models;
    m_fireGrid.particles.object->materials = m_fireGrid.particles.materials;
}

void ViewForest::_setObjects() {
    // Grid
    m_models[_ObjectId::Grid]->transforms = m_grid->grid_cells;

    // Objects
    for (const _Object& obj : m_objects) {
        m_models[obj.id]->transforms = obj.quats;
    }

    // Cube material
    m_models[_ObjectId::Cube]->localMaterial = glm::vec4(0.7f, 0.7f, 0.7f, 1);
}

// Callbacks
void ViewForest::mouse_on(int x, int y) {
    Scene& scene = Service<Window>::get().scene();

    m_mousePos.x = (float)x / scene.width();
    m_mousePos.y = (float)y / scene.height();
}

void ViewForest::_on_resize(const SceneEvents::Resized& evt) {
    m_filter.resize(evt.width, evt.height);
}
