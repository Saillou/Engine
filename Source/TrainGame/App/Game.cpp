#include "Game.hpp"

#include "TrainGame/App/Components/TrainController.h"

static uint64_t gs_id = 0;

Game::Game()
{
    _subscribe(&Game::onStateUpdate);
    _subscribe(&Game::onKeyPressed);
    _subscribe(&Game::onMouseMoved);

    m_window = std::make_unique<Window>(1600, 900, "Train game");
    m_view = std::make_shared<View>(m_window->width(), m_window->height());
    m_window->scene(m_view);
}

Game::~Game()
{
    // clean all objects here
}

void Game::createScene()
{
    GameObject* obj = new GameObject({ gs_id++, ModelId::Locomotive, {{0,0,0}, {1.f, 1.f, 1.f}, {0,0,1}, 0.f } });
    TrainController* trainController = new TrainController(&obj->transform);
    obj->components.push_back(trainController);

    m_objects[obj->id] = obj;
}

void Game::onStateUpdate(const CommonEvents::StateUpdated& evt)
{
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f / 1000.f;

    // game loop
    for (auto& obj : m_objects)
    {
        for (auto& comp : obj.second->components)
        {
            comp->update(dt_ms);
        }

        m_view->draw(obj.second->modelId, obj.second->transform);
    }

    m_timer.tic();
}

void Game::onKeyPressed(const CommonEvents::KeyPressed& evt)
{
    glm::vec3 dir(0, 0, 0);
    float scale = 0.f;
    float rot = 0.f;

    switch (evt.key)
    {
    case Key::ArrowRight: dir.x = +1.0f; break;
    case Key::ArrowLeft:  dir.x = -1.0f; break;
    case Key::ArrowUp:    dir.y = +1.0f; break;
    case Key::ArrowDown:  dir.y = -1.0f; break;
    case 'Q':             dir.z = +1.0f; break;
    case 'W':             dir.z = -1.0f; break;
    case 'A':             rot = +1.f; break;
    case 'S':             rot = -1.f; break;
    case 'Z':             scale = -1.f; break;
    case 'X':             scale = +1.f; break;
    }

    switch (evt.key)
    {
    case 'R': m_view->enable_filter = true; break;
    case 'T': m_view->enable_filter = false; break;
    }

    m_objects[0]->transform.position += 0.05f * dir;
    m_objects[0]->transform.rotation += 0.05f * rot;
    m_objects[0]->transform.scale += 0.05f * scale;
}

void Game::onMouseMoved(const CommonEvents::MouseMoved& evt)
{

}
