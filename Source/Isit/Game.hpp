#pragma once

#include <Engine/Events/Events.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Utils/Animator.hpp>

#include "CanvasEntity.hpp"

struct Game : Event::Subscriber
{
    Game();
    ~Game();

    bool wantQuit() const;

private:
    void _state_updated(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_click(const CommonEvents::MouseButton& btn);

    void _redraw_canvas();

    // Members
    Scene& m_scene;
    CanvasEntity m_canvas;

    bool _want_quit = false;

    // UI
    struct CircleButton {
        float x = 0.0f;
        float y = 0.0f;
        float r = 0.0f;

        bool is_pressed = false;
    } m_button_counter;

    struct Animation
    {
        Animator::Tweet tweet_load;
        Animator::Tweet tweet_fade;
        int tweet_playing = 0;
        float load_time = 2.0f;

    } m_square_animation;
};