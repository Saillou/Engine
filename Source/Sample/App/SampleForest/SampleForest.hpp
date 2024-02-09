#pragma once

#include "../Sample.hpp"
#include "ViewForest.hpp"

struct SampleForest : public Sample {
    SampleForest();

    virtual bool wantQuit() const;

protected:
    // Events
    void _on_state_update(const CommonEvents::StateUpdated& evt);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_mouse_moved(const CommonEvents::MouseMoved& evt);
    void _on_mouse_clicked(const CommonEvents::MouseButton& evt);

private:
    Scene& m_scene;
    bool m_want_quit = false;
    std::unique_ptr<ViewForest> m_view;
    std::vector<Light> m_pontential_lights;
    Timer::Chronometre m_timer;
};