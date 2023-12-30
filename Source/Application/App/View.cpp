#include "View.hpp"

View::View() :
    BaseScene()
{
    // Camera
    m_camera.position  = glm::vec3(0.0f, -6.0f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Load models
    m_timer.tic();
    {
        m_models[_ObjecId::Tree]      = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");
        m_models[_ObjecId::Character] = std::make_unique<ObjectModel>("Resources/objects/character/character.glb");
    }
    std::cout << "Models loaded in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Populate objects
    _initObjects();
}

void View::draw() {
    float dt_s = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;

    // Main scene
    BaseScene::clear();
    {
        // Draw objects
        for (const _Object& obj : m_objects) {
            m_models[obj.id]->draw(m_camera, obj.quat);
        }

        // Draw grid
        const int N = 10;
        const float s = 0.3f;
        for (int i = 0; i < N*N; i++) {
            int x = i % N - N/2;
            int y = i / N - N/2;

            m_model_box->draw(m_camera, glm::translate(glm::mat4(1.0f), s*glm::vec3(x, y, 0)));
        }

        // Skybox
        m_skybox->draw(m_camera);
    }
    
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

    // Ground
    m_model_box = std::make_unique<Box>(0.3f);
    m_model_box->addRecipe(Cookable::CookType::Solid,    glm::vec4(0.1f, 0.12f, 0.1f, 1));
    m_model_box->addRecipe(Cookable::CookType::Geometry, glm::vec4(0.2f, 0.2f, 0.2f, 1));

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

    // Main character
    m_objects.push_back({_ObjecId::Character,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(0, 0, 0)),            // Translation
            glm::vec3(0.02f, 0.02f, 0.02f)      // Scale
        )
    });
}

void View::_onResize() {
    // For example: resize framebuffers ..
}