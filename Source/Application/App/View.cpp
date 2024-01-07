#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>

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
    })
{
    // Camera
    m_camera.position  = glm::vec3(0.0f, -6.0f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Lightnings
    m_lights = {
        Light(glm::vec3{  0,  0, 0.70f }, glm::vec4{ 1,1,1,1 })
    };

    // Load models
    m_timer.tic();
    {
        m_models[_ObjecId::Tree]      = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");
        m_models[_ObjecId::Character] = std::make_unique<ObjectModel>("Resources/objects/character/character.glb");
    }
    std::cout << "Models loaded in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Populate objects
    m_timer.tic();
    {
        _initObjects();
    }
    std::cout << "Objects initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Create particules
    m_timer.tic();
    {
        _initParticles();
    }
    std::cout << "Particules initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;
}

void View::draw() {
    float dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();

    BaseScene::_update_camera();
    _setParticles(dt_since_last_draw);

    // Shadow scene
    BaseScene::Viewport(m_shadowRender.width(), m_shadowRender.height());
    m_shadowRender.render(m_camera, m_lights[0], [=](Shader& sh) {
        // Draw objects
        for (const _Object& obj : m_objects) {
            sh.use().set("model", obj.quat);
            m_models[obj.id]->drawElements();
        }

        // Box
        {
            sh.use().set("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0, 0.15f)));
            m_model_box_shadow->drawElements();
        }

        // Ground
        for (const glm::mat4& cell_quat : m_grid->m_grid_cells) {
            sh.use().set("model", cell_quat);
            m_model_box->drawElements();
        }
    });

    // Main scene
    BaseScene::Viewport(m_width, m_height);
    BaseScene::clear();

    {
        // Lights
        for (auto& light : m_lights) {
            m_model_sphere->get(Cookable::CookType::Solid)->use().set("diffuse_color", light.color);
            m_model_sphere->draw(m_camera, light.position);
        }

        // Draw objects
        for (const _Object& obj : m_objects) {
            m_models[obj.id]->draw(m_camera, obj.quat, m_lights);
        }

        // Draw box
        {
            m_model_box_shadow->draw(m_camera, glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0, 0.15f)), m_lights);
        }

        //// Particles
        //{
        //    m_fireGrid.particles.object->drawBatch(m_fireGrid.particles.amount, m_camera);
        //}

        // Draw ground with shadow
        m_shadowRender.bindTexture(GL_TEXTURE1);
        for (const glm::mat4& cell_quat : m_grid->m_grid_cells) {
            m_model_box->draw(m_camera, cell_quat, m_lights);
        }

        // Skybox
        m_skybox->draw(m_camera);
    }
    
    // Some static texts
    TextEngine::Write("Sample scene", 50, 50, 1.0f, glm::vec3(1, 1, 1));

    // Prepare next
    float dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
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
    m_model_box_shadow->addRecipe(Cookable::CookType::Solid, glm::vec4(1.0f, 0.7f, 0.3f, 1.0f));

    // Ground - Grid
    m_model_box = std::make_unique<Box>(1.0f);
    m_model_box->addRecipe(Cookable::CookType::Shadow, glm::vec4(0.5f, 0.5f, 0.5f, 1))
               ->addRecipe(Cookable::CookType::Geometry, glm::vec4(0.2f, 0.2f, 0.2f, 1));

    m_grid = std::make_unique<_Grid>(_Grid{ 0.3f, 30, {} });
    m_grid->m_grid_cells.resize(size_t(m_grid->n_side * m_grid->n_side));
    std::generate(m_grid->m_grid_cells.begin(), m_grid->m_grid_cells.end(), [id = 0, S = m_grid->cell_size, N = m_grid->n_side]() mutable
    { 
        const glm::vec2 cell_pos = glm::vec2(id%N - N/2, id/N - N/2);
        const glm::vec3 T_pos    = S * glm::vec3(cell_pos, -0.5f);
        const glm::vec3 T_scale  = S * glm::vec3(1);

        id++;
        return glm::scale(glm::translate(glm::mat4(1.0f), T_pos), T_scale);
    });

    // Lanterns
    m_model_sphere = std::make_unique<Sphere>(0.1f);
    m_model_sphere->addRecipe(Cookable::CookType::Solid);

    // Trees
    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(+0.95f, 0, +0.95f)),  // Translation
            glm::vec3(0.1f, 0.1f, 0.1f)         // Scale
        )
    });

    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-0.90f, 0, -0.95f)),  // Translation
            glm::vec3(0.1f, 0.1f, 0.1f)         // Scale
        )
    });

    // Character
    m_objects.push_back({_ObjecId::Character,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-1, 0, 1)),           // Translation
            glm::vec3(0.01f, 0.01f, 0.01f)      // Scale
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
    });

    _setParticles();

    // Cook
    m_fireGrid.particles.object->addRecipe(Cookable::CookType::Batch);
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

            model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.07f, 0.0f, 0.5f + y * 0.02f));
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
    // For example: resize framebuffers ..
}