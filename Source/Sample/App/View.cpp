#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>

#include <sstream>
#include <Engine/Utils/RayCaster.hpp>

// Random engine
static std::default_random_engine gen;

static std::uniform_real_distribution<float> dstr_pi(-glm::pi<float>(), +glm::pi<float>());
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

View::View(int widthHint, int heightHint):
    BaseScene(widthHint, heightHint),
    m_fireGrid({
        glm::vec3(0, 0, 0),
        { 
            size_t(2500),
            std::make_unique<Entity>(Entity::SimpleShape::Cube)
        }
    }),
    framebuffer_main(Framebuffer::Multisample, m_width, m_height),
    m_mousePos(0.0f, 0.0f)
{
    // Camera
    m_camera.position  = glm::vec3(+2.0f, -9.0f, 7.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0f, 0.0f);

    // Lightnings
    m_lights = {
        Light(glm::vec3{  0,  +1.50f, 1.0f }, glm::vec4{ 1, 0.7, 0.3, 1 })
    };

    // Load models
    m_timer.tic();
    {
        m_entities[_ObjectId::Locomotive] = std::make_unique<Entity>("Resources/objects/train/locomotive.glb");
        m_entities[_ObjectId::Tree]       = std::make_unique<Entity>("Resources/objects/tree/tree.glb");
        m_entities[_ObjectId::Character]  = std::make_unique<Entity>("Resources/objects/character/character.glb");

        m_entities[_ObjectId::Grid]       = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_entities[_ObjectId::Cube]       = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_entities[_ObjectId::Sphere]     = std::make_unique<Entity>(Entity::SimpleShape::Sphere);
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
}


// Methods
void View::draw() {
    static float dt_since_last_draw = 0.0f;
    static float dt_draw = 0.0f;

    dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();

    BaseScene::_update_camera();
    _setParticles(dt_since_last_draw);
    _setObjects();

    // Shadow scene
    BaseScene::Viewport(m_shadowRender.width(), m_shadowRender.height());
    m_shadowRender.render(m_camera, m_lights[0], [=](Shader& sh) {
        // Draw objects
        for (const _Object& obj : m_objects) {
            m_entities[obj.id]->model.drawElements(sh);
        }

        // Particles
        {
            m_fireGrid.particles.object->model.drawElements(sh);
        }
    });

    // Main scene
    BaseScene::Viewport(width(), height());
    framebuffer_main.bind();
    framebuffer_main.clear();
    {
        // Lights
        for (auto& light : m_lights) {
            m_entities[_ObjectId::Sphere]->get(Cookable::CookType::Basic)
                                         ->use().set("diffuse_color", light.color);

            m_entities[_ObjectId::Sphere]->drawOne(m_camera, glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f)));
        }

        // Draw objects
        for (const _Object& obj : m_objects) {
            // Normal colors
            m_shadowRender.bindTexture(GL_TEXTURE1);
            m_entities[obj.id]->get(Cookable::CookType::Shadow)
                              ->use().set("diffuse_color", obj.material_color)
                                     .set("depthMap", 1);

            m_entities[obj.id]->drawShadow(m_camera, m_lights[0]);

            // Highlight
            if (RayCaster::Intersect(m_mousePos, m_camera, *m_entities[obj.id], obj.quats[0])) {
                m_entities[obj.id]->get(Cookable::CookType::Geometry)
                                  ->use().set("diffuse_color", glm::vec4(0.2f, 0.7f, 0.7f, 1))
                                         .set("Projection", m_camera.projection)
                                         .set("View", m_camera.modelview);

                m_entities[obj.id]->model.drawElements(*m_entities[obj.id]->get(Cookable::CookType::Geometry));
            }
        }

        // Particles
        {
            m_fireGrid.particles.object->drawBatch(m_camera);
        }

        // Draw ground
        {
            // Grid
            {
                m_entities[_ObjectId::Grid]->get(Cookable::CookType::Geometry)
                                           ->use().set("diffuse_color", glm::vec4(0.2f, 0.2f, 0.2f, 1))
                                                  .set("View", m_camera.modelview)
                                                  .set("Projection", m_camera.projection);

                m_entities[_ObjectId::Grid]->model.drawElements(*m_entities[_ObjectId::Grid]->get(Cookable::CookType::Geometry));
            }

            // Shadow
            {
                m_shadowRender.bindTexture(GL_TEXTURE1);
                m_entities[_ObjectId::Grid]->get(Cookable::CookType::Shadow)
                                           ->use().set("diffuse_color", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).
                                                   set("depthMap", 1);

                m_entities[_ObjectId::Grid]->drawShadow(m_camera, m_lights[0]);
            }
        }

        // Skybox
        m_skybox->draw(m_camera);
    }
    framebuffer_main.unbind();

    // Draw on final frame
    BaseScene::clear();

    // Apply filter
    if(enable_filter) {
        m_filter.apply(framebuffer_main);
        BaseScene::drawFrame(m_filter.frame());
    }
    else {
        BaseScene::drawFrame(framebuffer_main);
    }

    // Debug texts
    {
        std::ostringstream ss;
        ss  << "Cam: x: " << int(m_camera.position.x * 10) / 10.0f
            << ", y: "    << int(m_camera.position.y * 10) / 10.0f
            << ", z: "    << int(m_camera.position.z * 10) / 10.0f;
        TextEngine::Write(ss.str(), 20.0f, m_height - 30.0f, 0.5f, glm::vec3(1, 1, 1));

        ss = {};
        ss  << "Mouse: x: " << int(m_width  * m_mousePos.x * 10) / 10.0f
            << ", y: "      << int(m_height * m_mousePos.y * 10) / 10.0f;
        TextEngine::Write(ss.str(), 20.0f, m_height - 60.0f, 0.5f, glm::vec3(1, 1, 1));

        ss = {};
        ss << "draw: " << int(dt_draw * 1000 * 1000) / 1000 << "ms";
        TextEngine::Write(ss.str(), 20.0f, m_height - 90.0f, 0.5f, glm::vec3(1, 1, 1));
    }

    // Prepare next
    dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();
}

// Allocate static memory
void View::_initObjects() {
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
            const glm::vec2 cell_pos = glm::vec2(id%N - N/2, id/N - N/2);
            const glm::vec3 T_pos    = S * glm::vec3(cell_pos, -0.5f);
            const glm::vec3 T_scale  = S * glm::vec3(0.5f);

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
                    glm::rotate(glm::mat4(1.0f),      // Identity
                        1.5f, glm::vec3(1, 0, 0)),  // Rotation
                    glm::vec3(1.0f, .30f, .0f)), // Translation
                glm::vec3(2.0f)         // Scale
            )
        }
    });

    // Trees
    std::vector<glm::mat4> forest;
    forest.resize(100);
    std::generate(forest.begin(), forest.end(), [id = 0]() mutable
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
                        1.5f, glm::vec3(1, 0, 0)),  // Rotation
                            tr),                     // Translation
                glm::vec3(dstr_one(gen)*3.0f)       // Scale
            );

            id++;
            return transf;
        }
    );
    m_objects.push_back({_ObjectId::Tree, glm::vec4(0), forest });

    // Character
    m_objects.push_back({_ObjectId::Character, glm::vec4(0),
        {
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-1, 0, 1)           // Translation
            )
        }
    });
}

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
            )_main_").str()
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
            )_main_").str()
        )
        .link();
}

void View::_initParticles() {
    // - Generate batch parameters
    const glm::vec3 color(1.0f, 0.7f, 0.3f);

    // Define particles
    m_fireGrid.particles.models.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.speeds.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.colors.resize(m_fireGrid.particles.amount);

    // Create batch
    std::generate(m_fireGrid.particles.colors.begin(), m_fireGrid.particles.colors.end(), [&, particules_id = 0]() mutable -> glm::vec4
        {
            particules_id++;
            float ratio = particules_id / float(m_fireGrid.particles.amount);

            return glm::min(glm::vec4(1.5f * ratio * color, 0.0f) + glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    );
}


// Update dynamic memory
void View::_setParticles(float dt) {
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
    m_fireGrid.particles.object->model.setBatch(m_fireGrid.particles.colors, m_fireGrid.particles.models);
}

void View::_setObjects() {
    // Grid
    m_entities[_ObjectId::Grid]->model.setBatch({}, m_grid->grid_cells);

    // Objects
    for (const _Object& obj : m_objects) {
        m_entities[obj.id]->model.setBatch({}, obj.quats);
    }
}

// Callbacks
void View::mouse_on(int x, int y) {
    m_mousePos.x = (float)x / m_width;
    m_mousePos.y = (float)y / m_height;
}

void View::_onResize() {
    framebuffer_main.resize(m_width, m_height);
    m_filter.resize(m_width, m_height);
}
