#pragma once

#include <Engine/Physx/Physx.hpp>
#include <Engine/Graphic/Base/BaseCommander.hpp>
#include <Engine/Utils/Timer.hpp>

#include "../Scenes/AppScene.hpp"

struct AppCommander : public BaseCommander {
    AppCommander(std::shared_ptr<BaseScene> scene);

protected:
    // Events
    void _on_state_update(const CommonEvents::StateUpdated& evt) override;
    void _on_key_pressed(const CommonEvents::KeyPressed& evt) override;
    void _on_mouse_moved(const CommonEvents::MouseMoved& evt) override;

private:
    // Members
    std::shared_ptr<AppScene> m_scene;
    Timer::Chronometre m_time;
};