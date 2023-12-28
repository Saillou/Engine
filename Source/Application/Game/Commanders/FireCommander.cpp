#include "FireCommander.hpp"
#include <glm/gtx/string_cast.hpp>

#include <Engine/Physx/Physx.hpp>

using namespace glm;


// Instance
FireCommander::FireCommander(std::shared_ptr<BaseScene> scene):
    BaseCommander(scene),
    m_scene(std::dynamic_pointer_cast<FireScene>(scene))
{
    // ..
}

void FireCommander::_on_state_update(const CommonEvents::StateUpdated& evt) {
    // ..
}

// Events
void FireCommander::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    switch (evt.key) 
    {
    case Key::ArrowUp:
        m_scene->move_ball(glm::vec3(0, 0, +0.01f));
        break;

    case Key::ArrowDown:
        m_scene->move_ball(glm::vec3(0, 0, -0.01f));
        break;

    case Key::ArrowRight:
        m_scene->move_ball(glm::vec3(+0.01f, 0, 0));
        break;

    case Key::ArrowLeft:
        m_scene->move_ball(glm::vec3(-0.01f, 0, 0));
        break;
    }
}

void FireCommander::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}
