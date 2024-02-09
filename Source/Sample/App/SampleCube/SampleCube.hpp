#pragma once

#include "../Sample.hpp"
#include "ViewCube.hpp"
#include "UiCube.hpp"

struct SampleCube : public Sample {
    SampleCube();

    bool wantQuit() const;

protected:
    // Events
    void _on_state_update(const CommonEvents::StateUpdated& evt);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_mouse_moved(const CommonEvents::MouseMoved& evt);
    void _on_mouse_button(const CommonEvents::MouseButton& evt);

    void _on_ui_update(const CommonEvents::StateUpdated& evt);

private:
    std::unique_ptr<UiCube>   m_ui;
    std::unique_ptr<ViewCube> m_view;
    std::vector<Light>    m_pontential_lights;

    float m_distance = 6.0f;
    float m_theta    = 0.0f;

    bool m_enable_filter      = false;
    bool m_enable_interaction = false;
};
