#include "Game.hpp"

#include "TrainGame/App/Components/TrainController.h"
#include "TrainGame/App/Components/GridComponent.h"
#include "TrainGame/App/Components/ConstructComponent.h"

#include "TrainGame/TrainGame/Grid.hpp"

namespace Thomas
{
    static uint64_t gs_id = 0;

    Game::Game()
    {
        _subscribe(&Game::onStateUpdate);
        _subscribe(&Game::onKeyPressed);
        _subscribe(&Game::onMouseMoved);
        _subscribe(&Game::onSceneFinishedRender);

        m_window = std::make_unique<Window>(1600, 900, "Train game");
        m_view = std::make_shared<View>(m_window->width(), m_window->height());
        m_window->scene(m_view);

        m_ui = std::make_unique<MainUI>(m_window->backend());
    }

    Game::~Game()
    {
        // clean all objects here
    }

    void Game::createScene()
    {
        Grid::initAtPosition({ 0.f,0.f,0.05f }, { 0.1f,0.1f });

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
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(20,-39), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            m_objects[obj->id] = obj;
        }
    }

    void Game::onStateUpdate(const CommonEvents::StateUpdated& evt)
    {
        float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f / 1000.f;

        // game loop
        for (auto& obj : m_objects)
        {
            // TODO: delete properly
            if (!obj.second || obj.second->selectedForDeletion)
                continue;

            for (auto& comp : obj.second->components)
            {
                comp->update(dt_ms);
                if (!comp->isAlive())
                {
                    //delete comp;
                    //comp = nullptr;
                }
            }

            obj.second->components.erase(std::remove_if(obj.second->components.begin(),
                obj.second->components.end(),
                [&](const auto& comp)-> bool
                { return !comp; }),
                obj.second->components.end());

            m_view->draw(obj.second->modelId, obj.second->transform);
        }

        m_view->drawGrid(Grid::getCells());
        Grid::clearCells();

        m_ui->Prepare();

        m_ui->showBuildingSelection();
        m_ui->ready = true;

        if (m_ui->state.building)
        {
            m_ui->state.building = false;
            GameObject* obj = new GameObject({ gs_id++, ModelId::Building1, {m_view->m_bigtime, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            ConstructComponent* constructComponent = new ConstructComponent(&obj->transform);
            obj->components.push_back(constructComponent);

            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,5 }, { -2,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Construct);
            obj->components.push_back(gridComponent);

            constructComponent->addGridComponent(gridComponent);
            constructComponent->addObjectDeletionPtr(&obj->selectedForDeletion);
            m_objects[obj->id] = obj;
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
        m_objects[0]->transform.rotation.z += 0.05f * rot;
        m_objects[0]->transform.scale += 0.05f * scale;
    }

    void Game::onMouseMoved(const CommonEvents::MouseMoved& evt)
    {

    }
    void Game::onSceneFinishedRender(const CommonEvents::SceneFinishedRender& evt)
    {
        if(m_ui && m_ui->ready)
            m_ui->Render();
    }
} // namespace Thomas