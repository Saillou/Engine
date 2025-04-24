#pragma once

#include <unordered_map>
#include "Panels/Panel.hpp"

struct Game : Event::Subscriber
{
    Game();
    ~Game();

    bool wantQuit() const;

private:
    enum class StepGame {
        Story,
        Idle,
        Fight,
    } _current_step = StepGame::Idle;

    void _state_updated(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_click(const CommonEvents::MouseButton& btn);
    void _on_move(const CommonEvents::MouseMoved& btn);

    // Members
    Scene& m_scene;
    CanvasEntity m_canvas;
    std::unordered_map<StepGame, std::shared_ptr<Panel>> m_panels;

    bool _want_quit = false;
};