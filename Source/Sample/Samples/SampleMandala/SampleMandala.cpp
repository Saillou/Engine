#include "SampleMandala.hpp"

#include "../SampleTrain/HelperTrain.hpp"

// -------- Editor --------
SampleMandala::SampleMandala() { 
    Scene& scene(Service<Window>::get().scene());

    scene.lights = { };
    scene.camera.position = glm::vec3(0, -5.0f, 0);
    scene.camera.direction = glm::vec3(0, 0, 0);

    m_entities["Mandala"] = HelperTrain::tile_with_texture("Resources/textures/mandala.png");
    ECS::getComponent<BodyComponent>(m_entities["Mandala"]).transform.local = 
        glm::scale(glm::rotate(glm::mat4(1.0f), glm::pi<float>()/2.0f, glm::vec3(1,0,0)), glm::vec3(1.0f, 1.0f, 1.0f));

    _subscribe([&](const CommonEvents::KeyPressed& evt) {
        glm::vec2 dir(0,0);
        switch (evt.key) {
            case KeyCode::ArrowLeft:  dir.x = +1.0f; break;
            case KeyCode::ArrowRight: dir.x = -1.0f; break;
            case KeyCode::ArrowUp:    dir.y = -1.0f; break;
            case KeyCode::ArrowDown:  dir.y = +1.0f; break;
        }
        scene.camera.position  += 1e-3f * glm::vec3(dir.x, 0, dir.y);
        scene.camera.direction += 1e-3f * glm::vec3(dir.x, 0, dir.y);
    });
}

SampleMandala::~SampleMandala()
{
    for (auto entity : m_entities) {
        ECS::destroyEntity(entity.second);
    }
}
