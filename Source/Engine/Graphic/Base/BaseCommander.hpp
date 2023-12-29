#pragma once

#include <memory>

#include "BaseScene.hpp"
#include "../../Events/CommonEvents.hpp"

// -- Scene --
struct BaseCommander : public Event::Subscriber {
    BaseCommander(std::shared_ptr<BaseScene> scene);
    virtual ~BaseCommander() = default;

protected:
    virtual void _on_state_update(const CommonEvents::StateUpdated& evt);
    virtual void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    virtual void _on_mouse_moved(const CommonEvents::MouseMoved& evt);

    std::shared_ptr<BaseScene> m_scene;
};