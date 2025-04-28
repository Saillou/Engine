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

    void _change_panel(StepGame);

    // Members
    Scene& m_scene;
    std::unordered_map<StepGame, std::shared_ptr<Panel>> m_panels;

    bool _want_quit = false;
};