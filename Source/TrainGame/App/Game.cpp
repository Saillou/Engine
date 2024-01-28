#include "Game.hpp"

#include "TrainGame/App/Components/TrainController.h"
#include "TrainGame/App/Components/GridComponent.h"
#include "TrainGame/App/Components/ConstructComponent.h"
#include "TrainGame/App/Components/InventoryComponent.h"
#include "TrainGame/App/Components/ProduceComponent.h"
#include "TrainGame/App/Components/DropComponent.h"

#include "TrainGame/TrainGame/Grid.hpp"

#include "TrainGame/App/Objects/Item.h"

#include "TrainGame/Engine/Core/ECS.h"
#include "TrainGame/Engine/Components/Transform2.h"
#include "TrainGame/Engine/Components/TrainController2.h"
#include "TrainGame/Engine/Components/Grid2.h"
#include "TrainGame/Engine/Components/RenderComponent.h"

namespace Thomas
{
    static uint64_t gs_id = 0;

    Game::Game()
    {
        _subscribe(&Game::onStateUpdate);
        _subscribe(&Game::onKeyPressed);
        _subscribe(&Game::onMouseMoved);
        _subscribe(&Game::onSceneFinishedRender);

        ECS::registerComponent<Transform2>();
        ECS::registerComponent<TrainController2>();
        ECS::registerComponent<Grid2>();
        ECS::registerComponent<RenderComponent>();

        m_trainControllerSystem = ECS::registerSystem<TrainControllerSystem>();
        {
            Signature signature;
            signature.set(ECS::getComponentType<Transform2>());
            signature.set(ECS::getComponentType<TrainController2>());

            ECS::setSystemSignature<TrainControllerSystem>(signature);
        }

        m_window = std::make_unique<Window>(1600, 900, "Train game");
        m_view = std::make_shared<View>(m_window->scene());
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
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position  = { 0.f, 0.f,1.f };
            transform.scale     = { 1.f,1.f,1.f };
            transform.rotation  = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Locomotive;
            renderComponent.mobility = RenderComponent::Mobility::Dynamic;
            ECS::addComponent(ent, renderComponent);

            TrainController2 trainController2;
            trainController2.m_points.push_back({ Grid::getPosition(x + 0, y + 0).x, Grid::getPosition(x + 0, y + 0).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x + 4, y - 1).x, Grid::getPosition(x + 4, y - 2).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 7, y - 2).x, Grid::getPosition(x + 7, y - 2).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 31, y - 2).x, Grid::getPosition(x + 31, y - 2).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 35, y - 1).x, Grid::getPosition(x + 35, y - 2).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 38, y + 0).x, Grid::getPosition(x + 38, y + 0).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x + 39, y + 4).x, Grid::getPosition(x + 40, y + 4).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 40, y + 7).x, Grid::getPosition(x + 40, y + 7).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 40, y + 23).x, Grid::getPosition(x + 40, y + 23).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 39, y + 27).x, Grid::getPosition(x + 40, y + 27).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 38, y + 30).x, Grid::getPosition(x + 38, y + 30).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x + 35, y + 31).x, Grid::getPosition(x + 35, y + 32).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 31, y + 32).x, Grid::getPosition(x + 31, y + 32).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 7, y + 32).x, Grid::getPosition(x + 7, y + 32).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 4, y + 31).x, Grid::getPosition(x + 4, y + 32).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 0, y + 30).x, Grid::getPosition(x + 0, y + 30).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x - 1, y + 27).x, Grid::getPosition(x - 2, y + 27).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x - 2, y + 23).x, Grid::getPosition(x - 2, y + 23).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x - 2, y + 7).x, Grid::getPosition(x - 2, y + 7).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x - 2, y + 4).x, Grid::getPosition(x - 2, y + 4).y, z }); // turn helper

            trainController2.m_minDistance = 0.03f;
            trainController2.m_speed = 0.7f;
            trainController2.m_rotationSpeed = 2.5f;

            ECS::addComponent(ent, trainController2);
        }

        for(size_t i = 0; i < 6; i++)
        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = { 0.5f * i, 0.5f * i,1.f };
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Locomotive;
            ECS::addComponent(ent, renderComponent);

            TrainController2 trainController2;
            trainController2.m_points.push_back({ Grid::getPosition(x + 0, y + 0).x, Grid::getPosition(x + 0, y + 0).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x + 4, y - 1).x, Grid::getPosition(x + 4, y - 2).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 7, y - 2).x, Grid::getPosition(x + 7, y - 2).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 31, y - 2).x, Grid::getPosition(x + 31, y - 2).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 35, y - 1).x, Grid::getPosition(x + 35, y - 2).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 38, y + 0).x, Grid::getPosition(x + 38, y + 0).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x + 39, y + 4).x, Grid::getPosition(x + 40, y + 4).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 40, y + 7).x, Grid::getPosition(x + 40, y + 7).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 40, y + 23).x, Grid::getPosition(x + 40, y + 23).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 39, y + 27).x, Grid::getPosition(x + 40, y + 27).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 38, y + 30).x, Grid::getPosition(x + 38, y + 30).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x + 35, y + 31).x, Grid::getPosition(x + 35, y + 32).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 31, y + 32).x, Grid::getPosition(x + 31, y + 32).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 7, y + 32).x, Grid::getPosition(x + 7, y + 32).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x + 4, y + 31).x, Grid::getPosition(x + 4, y + 32).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x + 0, y + 30).x, Grid::getPosition(x + 0, y + 30).y, z }); // turn
            trainController2.m_points.push_back({ Grid::getPosition(x - 1, y + 27).x, Grid::getPosition(x - 2, y + 27).y, z }); // turn helper
            trainController2.m_points.push_back({ Grid::getPosition(x - 2, y + 23).x, Grid::getPosition(x - 2, y + 23).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x - 2, y + 7).x, Grid::getPosition(x - 2, y + 7).y, z });
            trainController2.m_points.push_back({ Grid::getPosition(x - 2, y + 4).x, Grid::getPosition(x - 2, y + 4).y, z }); // turn helper

            trainController2.m_minDistance = 0.01f;
            trainController2.m_speed = 0.7f;
            trainController2.m_rotationSpeed = 2.5f;

            ECS::addComponent(ent, trainController2);
        }

        // tracks
        // 
        // class Level l1;
        // l1.loadFromFile();

        
        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x, y);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::TrackRight;
            ECS::addComponent(ent, renderComponent);
        }

        // bottom horizontal tracks

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 7, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 11, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 15, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 19, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 23, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 27, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 31, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        // left vertical tracks

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x - 2, y + 7);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x - 2, y + 11);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x - 2, y + 15);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x - 2, y + 19);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x - 2, y + 23);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x, y + 30);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, -1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::TrackRight;
            ECS::addComponent(ent, renderComponent);
        }

        // top horizontal tracks

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 7, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 11, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 15, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 19, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 23, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 27, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 31, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 38, y + 30);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::TrackLeft;
            ECS::addComponent(ent, renderComponent);
        }

        // right vertical tracks

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 40, y + 23);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 40, y + 19);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 40, y + 15);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 40, y + 11);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 40, y + 7);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform2 transform;
            transform.position = Grid::getPosition(x + 38, y);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, -1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::TrackLeft;
            ECS::addComponent(ent, renderComponent);
        }

    }

    void Game::onStateUpdate(const CommonEvents::StateUpdated& evt)
    {
        float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f / 1000.f;

        // game loop
        /*
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
        */

        m_trainControllerSystem->update(dt_ms);

        //m_view->drawGrid(Grid::getCells());
        //Grid::clearCells();

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

            Item item{ Item::ItemType::Item1 };
            ProduceComponent* produce = new ProduceComponent(item, 5.f, 1);
            obj->components.push_back(produce);

            const auto size = Grid::getCellSize();
            const glm::vec3 p1 = { size.x * -5, size.y * -5, 0 };
            const glm::vec3 p2 = { size.x * 5, size.y * 5, 0 };
            DropComponent* dropComponent = new DropComponent(&obj->transform, p1,p2);

            for (const auto& obj : m_objects)
            {
                auto it = std::find_if(obj.second->components.begin(), obj.second->components.end(), [](Component* cmpt) {return cmpt->m_id == ComponentId::InventoryComponent; });

                if (it != obj.second->components.end())
                {
                    std::cout << "Found object with inventory component!\n";
                    dropComponent->setInventoryComponent((InventoryComponent*)*it);
                    break;
                }
            }

            dropComponent->setProduceComponent(produce);
            obj->components.push_back(dropComponent);

            // progress bar
            GameObject* progressBar1 = new GameObject({ gs_id++, ModelId::CubeBasic, {{-0.1,0,1}, {0.2f, 0.1f, 0.1f}, {0.785f,0,0} } });
            m_objects[progressBar1->id] = progressBar1;

            GameObject* progressBar2 = new GameObject({ gs_id++, ModelId::CubeGeometry, {{0,0,1}, {0.3f, 0.1f, 0.1f}, {0.785f,0,0.f} } });
            m_objects[progressBar2->id] = progressBar2;

            
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

        Transform2& transform = ECS::getComponent<Transform2>(0);
        transform.position += 0.05f * dir;
        transform.rotation.z += 0.05f * rot;
        transform.scale += 0.05f * scale;
    }

    void Game::onMouseMoved(const CommonEvents::MouseMoved& evt)
    {

    }
    void Game::onSceneFinishedRender(const SceneEvents::RenderFinished& evt)
    {
        if(m_ui && m_ui->ready)
            m_ui->Render();
    }
} // namespace Thomas