#include "AppCommander.hpp"
#include <glm/gtx/string_cast.hpp>

#include <Engine/Physx/Physx.hpp>

using namespace glm;


// Instance
AppCommander::AppCommander(std::shared_ptr<BaseScene> scene):
    BaseCommander(scene),
    m_scene(std::dynamic_pointer_cast<AppScene>(scene))
{
    // ..
}

void AppCommander::_on_state_update(const CommonEvents::StateUpdated& evt) {
    // ..
}

// Events
void AppCommander::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    // ..
    if(evt.key == Key::Space)
        m_scene->redraw = true;
}

void AppCommander::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}
