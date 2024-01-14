#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>

#include <sstream>
#include <Engine/Utils/RayCaster.hpp>

#define DRAW_REAL

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
            std::make_unique<Box>(glm::vec3(0.010f))
        }
    }),
    framebuffer_main(Framebuffer::Multisample, m_width, m_height),
    m_mousePos(0.0f, 0.0f)
{
    // Camera
    m_camera.position  = glm::vec3(0.0f, -6.0f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Lightnings
    m_lights = {
        Light(glm::vec3{  0,  0, 0.50f }, glm::vec4{ 1, 0.7, 0.3, 1 })
    };

    // Load models
    m_timer.tic();
    {
        m_models[_ObjecId::Locomotive] = std::make_unique<ObjectModel>("Resources/objects/train/locomotive.glb");
        m_models[_ObjecId::Tree]       = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");
        m_models[_ObjecId::Character]  = std::make_unique<ObjectModel>("Resources/objects/character/character.glb");
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

void View::mouse_on(int x, int y) {
    m_mousePos.x = (float)x / m_width;
    m_mousePos.y = (float)y / m_height;
}

void View::draw() {
    static float dt_since_last_draw = 0.0f;
    static float dt_draw = 0.0f;

    dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();

    BaseScene::_update_camera();
    _setParticles(dt_since_last_draw);

    // Define box position
    const auto box_quat = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0, 0.15f));

    // Shadow scene
    BaseScene::Viewport(m_shadowRender.width(), m_shadowRender.height());
    m_shadowRender.render(m_camera, m_lights[0], [=](Shader& sh) {
        // Draw objects
        for (const _Object& obj : m_objects) {
            sh.use().set("model", obj.quat);
            m_models[obj.id]->drawElements(sh);
        }

        // Box
        {
            sh.use().set("model", box_quat);
            m_model_box_shadow->drawElements(sh);
        }

        // Ground
        for (const glm::mat4& cell_quat : m_grid->m_grid_cells) {
            sh.use().set("model", cell_quat);
            m_model_box->drawElements(sh);
        }
    });


    // Main scene
    BaseScene::Viewport(width(), height());
    framebuffer_main.bind();
    framebuffer_main.clear();
    {
        // Lights
        for (auto& light : m_lights) {
            m_model_sphere->get(Cookable::CookType::Solid)->use().set("diffuse_color", light.color);
            m_model_sphere->draw(m_camera, light.position);
        }

        // Draw objects
        for (const _Object& obj : m_objects) {
            m_models[obj.id]->draw(m_camera, obj.quat, m_lights);

            if (RayCaster::Intersect(m_mousePos, m_camera, *m_models[obj.id], obj.quat)) {
                m_models[obj.id]->get(Cookable::CookType::ModelGeometry)->use().set("diffuse_color", glm::vec4(0.2f, 0.7f, 0.7f, 1));
                m_models[obj.id]->drawGeometry(m_camera, obj.quat);
            }
        }

        // Draw box
        {
            bool highlight = RayCaster::Intersect(m_mousePos, m_camera, *m_model_box_shadow, box_quat);

            const glm::vec4 border_color = highlight ?
                glm::vec4(0.2f, 1.0f, 1.0f, 1) :
                glm::vec4(0.2f, 0.2f, 0.2f, 1);

            m_model_box_shadow->get(Cookable::CookType::Geometry)->use().set("diffuse_color", border_color);
            m_model_box_shadow->draw(m_camera, box_quat, m_lights);
        }

        // Particles
        {
            m_fireGrid.particles.object->drawBatch(m_fireGrid.particles.amount, m_camera);
        }

        // Draw ground with shadow
        m_shadowRender.bindTexture(GL_TEXTURE1);
        for (const glm::mat4& cell_quat : m_grid->m_grid_cells) {
            m_model_box->draw(m_camera, cell_quat, m_lights);
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
        BaseScene::drawFrame(m_filter.framebuffer);
    }
    else {
        BaseScene::drawFrame(framebuffer_main);
    }

    // Debug texts
    {
        std::ostringstream ss;
        ss << "Cam: x: " << int(m_camera.position.x * 10) / 10.0f
            << ", y: " << int(m_camera.position.y * 10) / 10.0f
            << ", z: " << int(m_camera.position.z * 10) / 10.0f;
        TextEngine::Write(ss.str(), 20.0f, m_height - 30.0f, 0.5f, glm::vec3(1, 1, 1));

        ss = {};
        ss << "Mouse: x: " << int(m_width * m_mousePos.x * 10) / 10.0f
            << ", y: " << int(m_height * m_mousePos.y * 10) / 10.0f;
        TextEngine::Write(ss.str(), 20.0f, m_height - 60.0f, 0.5f, glm::vec3(1, 1, 1));

        ss = {};
        ss << "draw: " << int(dt_draw * 1000 * 1000) / 1000 << "ms";
        TextEngine::Write(ss.str(), 20.0f, m_height - 90.0f, 0.5f, glm::vec3(1, 1, 1));
    }

    // Prepare next
    dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();
}

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

    // Box - Test shadow
    m_model_box_shadow = std::make_unique<Box>(0.3f);
    m_model_box_shadow->addRecipe(Cookable::CookType::Solid, glm::vec4(1.0f, 0.7f, 0.3f, 1.0f))
                      ->addRecipe(Cookable::CookType::Geometry, glm::vec4(0.2f, 0.2f, 0.2f, 1));

    // Ground - Grid
    m_model_box = std::make_unique<Box>(1.0f);
    m_model_box->addRecipe(Cookable::CookType::Shadow, glm::vec4(0.5f, 0.5f, 0.5f, 1))
               ->addRecipe(Cookable::CookType::Geometry, glm::vec4(0.2f, 0.2f, 0.2f, 1));

    m_grid = std::make_unique<_Grid>(_Grid{ 0.3f, 15, {} });
    m_grid->m_grid_cells.resize(size_t(m_grid->n_side * m_grid->n_side));
    std::generate(m_grid->m_grid_cells.begin(), m_grid->m_grid_cells.end(), [id = 0, S = m_grid->cell_size, N = m_grid->n_side]() mutable
        { 
            const glm::vec2 cell_pos = glm::vec2(id%N - N/2, id/N - N/2);
            const glm::vec3 T_pos    = S * glm::vec3(cell_pos, -0.5f);
            const glm::vec3 T_scale  = S * glm::vec3(1);

            id++;
            return glm::scale(glm::translate(glm::mat4(1.0f), T_pos), T_scale);
        }
    );

    // Lanterns
    m_model_sphere = std::make_unique<Sphere>(0.1f);
    m_model_sphere->addRecipe(Cookable::CookType::Solid);

    // Locomotive
    m_objects.push_back({ _ObjecId::Locomotive,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),      // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(1.0f, .30f, .0f)), // Translation
            glm::vec3(2.0f)         // Scale
        )
    });

    // Trees
    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(1.5f, 0.10f, 0.5f)),  // Translation
            glm::vec3(2.0f)         // Scale
        )
    });

    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-0.90f, 0.10f, -0.95f)),  // Translation
            glm::vec3(2.0f)         // Scale
        )
    });

    // Character
    m_objects.push_back({_ObjecId::Character,
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-1, 0, 1)           // Translation
        )
    });
}

void View::_initParticles() {
    // - Generate batch parameters
    const glm::vec3 color(1.0f, 0.7f, 0.3f);

    // Define particles
    m_fireGrid.particles.models.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.speeds.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.colors.resize(m_fireGrid.particles.amount);

    // Create batch
    m_fireGrid.particles.object->createBatch(
        m_fireGrid.particles.colors,
        m_fireGrid.particles.models
    );

    std::generate(m_fireGrid.particles.colors.begin(), m_fireGrid.particles.colors.end(), [&, particules_id = 0]() mutable -> glm::vec4
        {
            particules_id++;
            float ratio = particules_id / float(m_fireGrid.particles.amount);

            return glm::min(glm::vec4(1.5f * ratio * color, 0.0f) + glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    );

    _setParticles();

    // Cook
    m_fireGrid.particles.object->addRecipe(Cookable::CookType::Batch);
}

void View::_initFilters() {
    m_filter.shader.
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
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


void View::_setParticles(float dt) {
    // Move
    for (int particules_id = 0; particules_id < m_fireGrid.particles.amount; particules_id++) {
        glm::vec4& speed = m_fireGrid.particles.speeds[particules_id];
        glm::mat4& model = m_fireGrid.particles.models[particules_id];

        const bool hasEnded = model[0][0] < 1e-2f || model[1][1] < 1e-2f || model[2][2] < 1e-2f; // also true for first draw

        if (hasEnded) {
            const int SIZE = (int)sqrt(m_fireGrid.particles.amount);
            int x = particules_id % SIZE - SIZE / 2;
            int y = particules_id / SIZE - SIZE / 2;

            model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.007f, 1.5f, 0.25f + y * 0.002f));
            speed = glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen), 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
        }
        else {
            model = glm::scale(glm::translate(model, m_fireGrid.pos + dt * glm::vec3(speed)), glm::vec3(speed.a));
        }
    }

    // Update
    m_fireGrid.particles.object->updateBatch(
        m_fireGrid.particles.colors,
        m_fireGrid.particles.models
    );
}


void View::_onResize() {
    framebuffer_main.resize(m_width, m_height);
    m_filter.resize(m_width, m_height);
}
