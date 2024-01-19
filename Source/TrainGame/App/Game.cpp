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
        const int x = -14;
        const int y = -14;
        const float z = 0.15f;
        // todo: see what Z is doing
        Grid::initAtPosition({ 0.f,0.f,0.05f }, { 0.1f,0.1f });

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Locomotive, {{Grid::getPosition(x,y).x,Grid::getPosition(x,y).y,z}, {1.f, 1.f, 1.f}, {0,0,0} } });
            TrainController* trainController = new TrainController(&obj->transform);
            obj->components.push_back(trainController);

            GridComponent* gridComponent = new GridComponent(&obj->transform, { 6,6 }, { 0,0 });
            gridComponent->setState(GridComponent::GridComponentState::Construct);
            obj->components.push_back(gridComponent);

            m_objects[obj->id] = obj;

            trainController->addPoint({ Grid::getPosition(x, y).x, Grid::getPosition(x, y).y, z }); // turn
            trainController->addPoint({ Grid::getPosition(x + 4, y - 1).x, Grid::getPosition(x + 4, y - 1).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x + 7, y - 2).x, Grid::getPosition(x + 7, y - 2).y, z });
            trainController->addPoint({ Grid::getPosition(x + 31, y - 2).x, Grid::getPosition(x + 31, y - 2).y, z });
            trainController->addPoint({ Grid::getPosition(x + 35, y - 1).x, Grid::getPosition(x + 35, y - 1).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x + 38, y).x, Grid::getPosition(x + 38, y).y, z }); // turn
            trainController->addPoint({ Grid::getPosition(x + 39, y + 4).x, Grid::getPosition(x + 39, y + 4).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x + 40, y + 7).x, Grid::getPosition(x + 40, y + 7).y, z });
            trainController->addPoint({ Grid::getPosition(x + 40, y + 23).x, Grid::getPosition(x + 40, y + 23).y, z });
            trainController->addPoint({ Grid::getPosition(x + 39, y + 27).x, Grid::getPosition(x + 39, y + 27).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x + 38, y + 30).x, Grid::getPosition(x + 38, y + 30).y, z }); // turn
            trainController->addPoint({ Grid::getPosition(x + 35, y + 31).x, Grid::getPosition(x + 35, y + 31).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x + 31, y + 32).x, Grid::getPosition(x + 31, y + 32).y, z });
            trainController->addPoint({ Grid::getPosition(x + 7, y + 32).x, Grid::getPosition(x + 7, y + 32).y, z });
            trainController->addPoint({ Grid::getPosition(x + 4, y + 31).x, Grid::getPosition(x + 4, y + 31).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x, y + 30).x, Grid::getPosition(x, y + 30).y, z }); // turn
            trainController->addPoint({ Grid::getPosition(x - 1, y + 27).x, Grid::getPosition(x - 1, y + 27).y, z }); // turn helper
            trainController->addPoint({ Grid::getPosition(x - 2, y + 23).x, Grid::getPosition(x - 2, y + 23).y, z });
            trainController->addPoint({ Grid::getPosition(x - 2, y + 7).x, Grid::getPosition(x - 2, y + 7).y, z });
            trainController->addPoint({ Grid::getPosition(x - 1, y + 4).x, Grid::getPosition(x - 1, y + 4).y, z }); // turn helper
        }

        // tracks

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackRight, {Grid::getPosition(x,y), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent1 = new GridComponent(&obj->transform, { 5,6 }, { -4,-1 });
            gridComponent1->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent1);

            GridComponent* gridComponent2 = new GridComponent(&obj->transform, { 6,5 }, { -1,-4 });
            gridComponent2->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent2);
            m_objects[obj->id] = obj;
        }

        // bottom horizontal tracks

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 7,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 11,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 15,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 19,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 23,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 27,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 31,y - 2), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        // left vertical tracks

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x - 2,y + 7), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x - 2,y + 11), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x - 2,y + 15), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x - 2,y + 19), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x - 2,y + 23), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackRight, {Grid::getPosition(x,y + 30), {1.f, 1.f, 1.f}, {0,0,-1.57f} } });
            GridComponent* gridComponent1 = new GridComponent(&obj->transform, { 6,5 }, { -1,0 });
            gridComponent1->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent1);

            GridComponent* gridComponent2 = new GridComponent(&obj->transform, { 5,6 }, { -4,-4 });
            gridComponent2->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent2);
            m_objects[obj->id] = obj;
        }

        // top horizontal tracks

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 7,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 11,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 15,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 19,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 23,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 27,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 31,y + 32), {1.f, 1.f, 1.f}, {0,0,1.57f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 5,3 }, { -2,-1 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackLeft, {Grid::getPosition(x + 38,y + 30), {1.f, 1.f, 1.f}, {0,0,0} } });
            GridComponent* gridComponent1 = new GridComponent(&obj->transform, { 6,5 }, { -4,0 });
            gridComponent1->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent1);

            GridComponent* gridComponent2 = new GridComponent(&obj->transform, { 5,6 }, { 0,-4 });
            gridComponent2->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent2);
            m_objects[obj->id] = obj;
        }

        // right vertical tracks

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 40,y + 23), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 40,y + 19), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 40,y + 15), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 40,y + 11), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::Track, {Grid::getPosition(x + 40,y + 7), {1.f, 1.f, 1.f}, {0,0,0.f} } });
            GridComponent* gridComponent = new GridComponent(&obj->transform, { 3,5 }, { -1,-2 });
            gridComponent->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent);
            m_objects[obj->id] = obj;
        }

        {
            GameObject* obj = new GameObject({ gs_id++, ModelId::TrackLeft, {Grid::getPosition(x + 38,y), {1.f, 1.f, 1.f}, {0,0,-1.57f} } });
            GridComponent* gridComponent1 = new GridComponent(&obj->transform, { 6,5 }, { -4,-4 });
            gridComponent1->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent1);

            GridComponent* gridComponent2 = new GridComponent(&obj->transform, { 5,6 }, { 0,0 });
            gridComponent2->setState(GridComponent::GridComponentState::Visible);
            obj->components.push_back(gridComponent2);
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
            GameObject* obj = new GameObject({ gs_id++, ModelId::Building1, {{0,0,0}, {1.f, 1.f, 1.f}, {0,0,1.57f} } });
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