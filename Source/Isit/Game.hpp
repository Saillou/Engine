#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Events/CommonEvents.hpp>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/SceneFrame.hpp>

#include "CanvasElement.hpp"

struct Game : Event::Subscriber
{
    Game();
    ~Game();

    bool wantQuit() const;

private:
    void _state_updated(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_click(const CommonEvents::MouseButton& btn);

    // Members
    Scene& m_scene;
    CanvasElement m_canvas;
    SceneFrame m_frame;

    bool _want_quit = false;

};