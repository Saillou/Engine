#include "View.hpp"

View::View() :
    BaseScene()
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -5.0f, 3.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // Start
    m_timer.tic();
    {
        m_models[_ObjecId::Tree]        = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");
        m_models[_ObjecId::Character]   = std::make_unique<ObjectModel>("Resources/objects/character/character.glb");
    }
    std::cout << "Models loaded in: " << m_timer.elapsed<Timer::microsecond>() / 1'000.0f << "ms." << std::endl;

    // Populate objects
    _initObjects();
}

void View::draw() {
    float dt_s = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    BaseScene::clear();

    // Draw objects
    for (const _Object& obj : m_objects) {
        m_models[obj.id]->draw(m_camera, obj.quat);
    }

    // Draw grid
    for (int i = 0; i < 100; i++) {
        int x = i % 10 - 5;
        int y = i / 10 - 5;

        m_model_box->draw(m_camera, glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.3f, y * 0.3f, 0)));
    }
    
    // Prepare next
    m_timer.tic();
}

void View::_initObjects() {
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
