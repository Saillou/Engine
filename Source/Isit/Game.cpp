#include "Game.hpp"

#include "Panels/Story/AnimationManager.hpp"
#include "Panels/PanelStory.hpp"
#include "Panels/PanelFight.hpp"
#include "Panels/PanelIdle.hpp"

/*
    histoire:   une balade en foret to eternity 
    idle:       cultivation,  elevage de pets
    fight:      tower defence
*/

/*
    TODO:
     - convert qi to elemental energy
     - use element energy to grow pets
     - create puzzles for tribulation
     - better fight
*/

Game::Game() :
    m_scene(Service<Window>::get().scene())
{
    AnimationManager::Instance().RegisterAll();

    m_panels[StepGame::Story] = std::make_shared<PanelStory>();
    m_panels[StepGame::Fight] = std::make_shared<PanelFight>();
    m_panels[StepGame::Idle]  = std::make_shared<PanelIdle>();

    // Events
    _subscribe(&Game::_on_key_pressed);
    _subscribe(&Game::_state_updated);

    _subscribe(m_panels[StepGame::Fight], [&](const PanelFight::Events::Result& fe) {
        switch (fe._result) 
        {
        case PanelFight::Events::Result::Win:
            /* do something */ 
            break;

        case PanelFight::Events::Result::Lose:
            /* do something */ 
            break;
        }
        _change_panel(StepGame::Story);
    });
    _subscribe(m_panels[StepGame::Story], [&](const PanelStory::Events::Choice& c) {
        switch (c._choice) 
        {
        case PanelStory::Events::Choice::Idle:
            _change_panel(StepGame::Idle);
            break;

        case PanelStory::Events::Choice::Fight:
            _change_panel(StepGame::Fight);
            break;
        }
    });
    _subscribe(m_panels[StepGame::Idle], [&](const PanelIdle::Events::Back&) {
        _change_panel(StepGame::Story);
    });
}
Game::~Game() {
    AnimationManager::Instance().UnregisterAll();

    _unsubscribeAll();
}

bool Game::wantQuit() const {
	return _want_quit;
}

void Game::_state_updated(const CommonEvents::StateUpdated&)
{
    m_panels[_current_step]->clear();
    m_panels[_current_step]->draw();
}
void Game::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    switch (evt.key)
    {
        case KeyCode::Escape: _want_quit = true; break;
    }
}

void Game::_change_panel(StepGame step)
{
    m_panels[_current_step]->clear();

    _current_step = step;

    m_panels[_current_step]->reset();
    m_panels[_current_step]->draw();
}
