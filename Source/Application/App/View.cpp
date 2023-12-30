#include "View.hpp"

View::View() :
    BaseScene()
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -5.0f, 0.0f);
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
    
    // Prepare next
    m_timer.tic();
}

void View::_initObjects() {
    // Trees
    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(+3, 0, -10)),         // Translation
            glm::vec3(0.3f, 0.3f, 0.3f)         // Scale
        )
    });

    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-3, 0, -10)),         // Translation
            glm::vec3(0.3f, 0.3f, 0.3f)         // Scale
        )
    });

    // Main character
    m_objects.push_back({_ObjecId::Character,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(0, 0, -4)),           // Translation
            glm::vec3(0.05f, 0.05f, 0.05f)      // Scale
        )
    });
}
