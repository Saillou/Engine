#include "Game.hpp"

#include "Panels/PanelStory.hpp"
#include "Panels/PanelFight.hpp"
#include "Panels/PanelIdle.hpp"

/*
    histoire:   une balade en foret to eternity 
    idle:       cultivation,  elevage de pets
    fight:      tower defence
*/

Game::Game() :
    m_scene(Service<Window>::get().scene()),
    m_canvas(CanvasEntity())
{
    m_canvas.get()
        .ctx()
        .dimensions(CanvasContext::Dimensions::Absolute)
        .colors(CanvasContext::ColorFormat::Uint8);

    m_panels[StepGame::Story] = std::make_shared<PanelStory>(m_scene);
    m_panels[StepGame::Fight] = std::make_shared<PanelFight>(m_scene);
    m_panels[StepGame::Idle]  = std::make_shared<PanelIdle>(m_scene);

    // Events
    _subscribe(&Game::_on_key_pressed);
    _subscribe(&Game::_on_click);
    _subscribe(&Game::_on_move);
    _subscribe(&Game::_state_updated);

    _subscribe(m_panels[StepGame::Fight], [&](const FightEvents::Result& fe) {
        m_panels[StepGame::Fight]->reset();

        switch (fe._result) {
        case FightEvents::Result::Win:
            break;

        case FightEvents::Result::Lose:
            break;
        }

        _current_step = StepGame::Story;
        m_panels[_current_step]->reset();
    });

    _subscribe(m_panels[StepGame::Story], [&](const StoryEvents::Choice& c) {
        m_panels[StepGame::Story]->reset();

        switch (c._choice) {
        case StoryEvents::Choice::Idle:
            _current_step = StepGame::Idle;
            break;

        case StoryEvents::Choice::Fight:
            _current_step = StepGame::Fight;
            break;
        }

        m_panels[_current_step]->reset();
    });

    _subscribe(m_panels[StepGame::Idle], [&](const IdleEvents::Back&) {
        m_panels[StepGame::Idle]->reset();

        _current_step = StepGame::Story;
        m_panels[_current_step]->reset();
    });
}
Game::~Game() {
    _unsubscribeAll();
}

bool Game::wantQuit() const
{
	return _want_quit;
}

void Game::_state_updated(const CommonEvents::StateUpdated&)
{
    m_canvas.get().clear();

    m_panels[_current_step]->draw(m_canvas, m_scene);
}
void Game::_on_key_pressed(const CommonEvents::KeyPressed& evt) 
{
    switch (evt.key)
    {
        case KeyCode::Escape: _want_quit = true; break;
    }
}
void Game::_on_click(const CommonEvents::MouseButton& btn)
{
    m_panels[_current_step]->mouse_clicked(btn);
}

void Game::_on_move(const CommonEvents::MouseMoved& btn)
{
    m_panels[_current_step]->mouse_moved(btn);
}
