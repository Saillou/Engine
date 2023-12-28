#include "BaseCommander.hpp"
#include "../../Physx/Physx.hpp"

#include <iostream>

BaseCommander::BaseCommander(std::shared_ptr<BaseScene> scene) : 
    m_scene(scene) 
{
    // Clean physx
    Physx::Clear();

    // Root events
    _subscribe(&BaseCommander::_on_state_update);
    _subscribe(&BaseCommander::_on_key_pressed);
    _subscribe(&BaseCommander::_on_mouse_moved);
}

void BaseCommander::_on_state_update(const CommonEvents::StateUpdated& evt) {
    // To be overrided ..
}

void BaseCommander::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    // To be overrided ..
}

void BaseCommander::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // To be overrided ..
}
