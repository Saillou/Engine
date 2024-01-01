#include "View.hpp"

#include <algorithm>

View::View(int widthHint, int heightHint):
    BaseScene(widthHint, heightHint)
{
    // Camera
    m_camera.position  = glm::vec3(0.0f, -6.0f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Lightings
    m_lights = {
        Light(glm::vec3{  0,  0, 0.70f }, glm::vec4{ 1,1,1,1 }),
        Light(glm::vec3{ -1,  0, 0.35f }, glm::vec4{ 1,0,0,1 }),
        Light(glm::vec3{  0, -1, 0.35f }, glm::vec4{ 0,1,0,1 }),
        Light(glm::vec3{ +1,  0, 0.35f }, glm::vec4{ 1,1,0,1 }),
        Light(glm::vec3{  0, +1, 0.35f }, glm::vec4{ 0,1,1,1 }),
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
}

void View::draw() {
    float dt_s = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;

    // Main scene
    BaseScene::_update_camera();
    BaseScene::clear();
    {
        // Draw objects
        for (const _Object& obj : m_objects) {
            m_models[obj.id]->draw(m_camera, obj.quat);
        }

        // Lights
        for (auto& light : m_lights) {
            m_model_sphere->get(Cookable::CookType::Solid)->use().set("color", light.color);
            m_model_sphere->draw(m_camera, light.position);
        }

        // Draw grid
        for (const glm::mat4& cell_quat : m_grid->m_grid_cells) {
            m_model_box->draw(m_camera, cell_quat, m_lights);
        }

        // Draw box with shadow
        {
            m_model_box_shadow->draw(m_camera, glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0, 0.15f)), m_lights);
        }

        // Skybox
        m_skybox->draw(m_camera);
    }
    
    // Text for test
    TextEngine::Write("Hello", 50, 50, 1.0f, glm::vec3(1, 1, 1));

    // Prepare next
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
    m_model_box_shadow->addRecipe(Cookable::CookType::Solid, glm::vec4(1,1,1,1));

    // Ground - Grid
    m_model_box = std::make_unique<Box>(1.0f);
    m_model_box->addRecipe(Cookable::CookType::Solid,    glm::vec4(0.7f, 0.7f, 0.7f, 1));
    m_model_box->addRecipe(Cookable::CookType::Geometry, glm::vec4(0.2f, 0.2f, 0.2f, 1));

    m_grid = std::make_unique<_Grid>(_Grid{ 0.3f, 10, {} });
    m_grid->m_grid_cells.resize(size_t(m_grid->n_side * m_grid->n_side));
    std::generate(m_grid->m_grid_cells.begin(), m_grid->m_grid_cells.end(), [id = 0, S = m_grid->cell_size, N = m_grid->n_side]() mutable
    { 
        const glm::vec2 cell_pos = glm::vec2(id%N - N/2, id/N - N/2);
        const glm::vec3 T_pos    = S * glm::vec3(cell_pos, -0.5f);
        const glm::vec3 T_scale  = S * glm::vec3(1,1,1);

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
            glm::vec3(0.02f, 0.02f, 0.02f)      // Scale
        )
    });
}

void View::_onResize() {
    // For example: resize framebuffers ..
}