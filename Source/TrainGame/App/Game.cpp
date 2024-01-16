#include "Game.hpp"

#include "TrainGame/App/Components/TrainController.h"
#include "TrainGame/App/Components/GridComponent.h"

#include "TrainGame/TrainGame/Grid.hpp"

namespace Thomas
{
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
        Grid::initAtPosition({ 0.f,0.f,0.2f }, { 0.1f,0.1f });

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Locomotive, {{0,0,1.f}, {1.f, 1.f, 1.f}, {0,0,0} } });
            TrainController* trainController = new TrainController(&obj->transform);
            obj->components.push_back(trainController);

            GridComponent* gridComponent = new GridComponent(&obj->transform, { 6,6 }, { 0,0 });
            gridComponent->setState(GridComponent::GridComponentState::Construct);
            obj->components.push_back(gridComponent);

            m_objects[obj->id] = obj;
        }

        // tracks
        const float x = 0.75f;
        const float y = 1.f;
        const float z = 0.2f;

        const float dX = 0.44f;

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(0,-14), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,2 }, { -2,0 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);

            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(4,-14), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,2 }, { -2,0 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(8,-14), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,2 }, { -2,0 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(12,-14), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,2 }, { -2,0 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackRight, {Grid::getPosition(16,-14), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,2 }, { -2,0 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(16,-20), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 2,5 }, { 0,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(20,-39), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        /*
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x,y,z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - dX,y,z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - (dX * 2),y,z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - (dX * 3),y,z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - (dX * 4),y,z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackLeft, {{x + dX,y,z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackLeft, {{x + dX + 0.34f,y - 0.3f,z}, {1.f, 1.f, 1.f}, {0,0,0.785f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x + dX + 0.34f + 0.02f,y - 0.3f - 0.45f,z}, {1.f, 1.f, 1.f}, {0,0,0.f} } });
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x + dX + 0.34f + 0.02f,y - 0.3f - 0.45f - dX,z}, {1.f, 1.f, 1.f}, {0,0,0.f} } });
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x + dX + 0.34f + 0.02f,y - 0.3f - 0.45f - (dX*2),z}, {1.f, 1.f, 1.f}, {0,0,0.f} } });
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x + dX + 0.34f + 0.02f,y - 0.3f - 0.45f - (dX * 3),z}, {1.f, 1.f, 1.f}, {0,0,0.f} } });
            m_objects[obj->id] = obj;
        }
        
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackLeft, {{x + dX + 0.34f + 0.02f,y - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,0.f} } });
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackLeft, {{x + dX + 0.04f,y - 0.35f - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,-0.785f} } });
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x,y - 0.35f - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - dX,y - 0.35f - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - (dX * 2),y - 0.35f - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - (dX * 3),y - 0.35f - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {{x - (dX * 4),y - 0.35f - 0.3f - 0.45f - (dX * 4),z}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
        */
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

        m_view->drawGrid(Grid::getCells());
        Grid::clearCells();

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
        m_objects[0]->transform.rotation.z += 0.05f * rot;
        m_objects[0]->transform.scale += 0.05f * scale;
    }

    void Game::onMouseMoved(const CommonEvents::MouseMoved& evt)
    {

    }
} // namespace Thomas