#include "Sandbox.hpp"

#include <Engine/Framework/ECS/ECS.hpp>

#include "TrainGame/Engine/Components/RenderComponent.h"
#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/Grid.h"
#include "TrainGame/Engine/Components/TrainController.h"

#include "TrainGame/TrainGame/Grid.hpp" // temporary, need it to place entities correctly

namespace Thomas
{
    Sandbox::Sandbox()
    {
        _subscribe(&Sandbox::_on_key_pressed);
    }

    void Sandbox::load()
    {
        const int x = -14;
        const int y = -14;
        const float z = 0.15f;
        OldGrid::initAtPosition({ 0.f,0.f,0.15f }, { 0.1f,0.1f });

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = { 0.f, 0.f,1.f };
            //transform.scale     = { 1.f,1.f,1.f };
            transform.scale = { 0.05f,0.05f,0.05f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::CubeBasic;
            renderComponent.mobility = RenderComponent::Mobility::Dynamic;
            renderComponent.material.diffuse_color = { 1.f,1.f,1.f,1.f };
            ECS::addComponent(ent, renderComponent);

            TrainController trainController;
            trainController.m_points.push_back({ OldGrid::getPosition(x + 0, y + 0).x, OldGrid::getPosition(x + 0, y + 0).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x + 4, y - 1).x, OldGrid::getPosition(x + 4, y - 2).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 7, y - 2).x, OldGrid::getPosition(x + 7, y - 2).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 31, y - 2).x, OldGrid::getPosition(x + 31, y - 2).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 35, y - 1).x, OldGrid::getPosition(x + 35, y - 2).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 38, y + 0).x, OldGrid::getPosition(x + 38, y + 0).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x + 39, y + 4).x, OldGrid::getPosition(x + 40, y + 4).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 40, y + 7).x, OldGrid::getPosition(x + 40, y + 7).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 40, y + 23).x, OldGrid::getPosition(x + 40, y + 23).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 39, y + 27).x, OldGrid::getPosition(x + 40, y + 27).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 38, y + 30).x, OldGrid::getPosition(x + 38, y + 30).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x + 35, y + 31).x, OldGrid::getPosition(x + 35, y + 32).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 31, y + 32).x, OldGrid::getPosition(x + 31, y + 32).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 7, y + 32).x, OldGrid::getPosition(x + 7, y + 32).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 4, y + 31).x, OldGrid::getPosition(x + 4, y + 32).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 0, y + 30).x, OldGrid::getPosition(x + 0, y + 30).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x - 1, y + 27).x, OldGrid::getPosition(x - 2, y + 27).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x - 2, y + 23).x, OldGrid::getPosition(x - 2, y + 23).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x - 2, y + 7).x, OldGrid::getPosition(x - 2, y + 7).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x - 2, y + 4).x, OldGrid::getPosition(x - 2, y + 4).y, z }); // turn helper

            trainController.m_minDistance = 0.03f;
            trainController.m_speed = 0.7f;
            trainController.m_rotationSpeed = 2.5f;

            //ECS::addComponent(ent, trainController);

            Grid grid;
            grid.m_size = { 5,5 };
            grid.m_state = Grid::GridComponentState::Construct;
            grid.m_offset = { -2,-2 };
            grid.createCells();

            ECS::addComponent(ent, grid);
        }

        for (size_t i = 0; i < 1; i++)
        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = { 0.5f * i, 0.5f * i,1.f };
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Locomotive;
            renderComponent.mobility = RenderComponent::Mobility::Dynamic;
            ECS::addComponent(ent, renderComponent);

            TrainController trainController;
            trainController.m_points.push_back({ OldGrid::getPosition(x + 0, y + 0).x, OldGrid::getPosition(x + 0, y + 0).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x + 4, y - 1).x, OldGrid::getPosition(x + 4, y - 2).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 7, y - 2).x, OldGrid::getPosition(x + 7, y - 2).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 31, y - 2).x, OldGrid::getPosition(x + 31, y - 2).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 35, y - 1).x, OldGrid::getPosition(x + 35, y - 2).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 38, y + 0).x, OldGrid::getPosition(x + 38, y + 0).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x + 39, y + 4).x, OldGrid::getPosition(x + 40, y + 4).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 40, y + 7).x, OldGrid::getPosition(x + 40, y + 7).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 40, y + 23).x, OldGrid::getPosition(x + 40, y + 23).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 39, y + 27).x, OldGrid::getPosition(x + 40, y + 27).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 38, y + 30).x, OldGrid::getPosition(x + 38, y + 30).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x + 35, y + 31).x, OldGrid::getPosition(x + 35, y + 32).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 31, y + 32).x, OldGrid::getPosition(x + 31, y + 32).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 7, y + 32).x, OldGrid::getPosition(x + 7, y + 32).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x + 4, y + 31).x, OldGrid::getPosition(x + 4, y + 32).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x + 0, y + 30).x, OldGrid::getPosition(x + 0, y + 30).y, z }); // turn
            trainController.m_points.push_back({ OldGrid::getPosition(x - 1, y + 27).x, OldGrid::getPosition(x - 2, y + 27).y, z }); // turn helper
            trainController.m_points.push_back({ OldGrid::getPosition(x - 2, y + 23).x, OldGrid::getPosition(x - 2, y + 23).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x - 2, y + 7).x, OldGrid::getPosition(x - 2, y + 7).y, z });
            trainController.m_points.push_back({ OldGrid::getPosition(x - 2, y + 4).x, OldGrid::getPosition(x - 2, y + 4).y, z }); // turn helper

            trainController.m_minDistance = 0.01f;
            trainController.m_speed = 0.7f;
            trainController.m_rotationSpeed = 2.5f;

            ECS::addComponent(ent, trainController);

            Grid grid;
            grid.m_size = { 5,5 };
            grid.m_state = Grid::GridComponentState::Visible;
            grid.m_offset = { -3,-3 };
            grid.createCells();

            ECS::addComponent(ent, grid);
        }

        // tracks
        // 
        // class Level l1;
        // l1.loadFromFile();


        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x, y);
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

            Transform transform;
            transform.position = OldGrid::getPosition(x + 7, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 11, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 15, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 19, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 23, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 27, y - 2);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 31, y - 2);
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

            Transform transform;
            transform.position = OldGrid::getPosition(x - 2, y + 7);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x - 2, y + 11);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x - 2, y + 15);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x - 2, y + 19);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x - 2, y + 23);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x, y + 30);
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

            Transform transform;
            transform.position = OldGrid::getPosition(x + 7, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 11, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 15, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 19, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 23, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 27, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 31, y + 32);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 38, y + 30);
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

            Transform transform;
            transform.position = OldGrid::getPosition(x + 40, y + 23);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 40, y + 19);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 40, y + 15);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 40, y + 11);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 40, y + 7);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, 0.f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::Track;
            ECS::addComponent(ent, renderComponent);
        }

        {
            Entity ent = ECS::createEntity();

            Transform transform;
            transform.position = OldGrid::getPosition(x + 38, y);
            transform.scale = { 1.f,1.f,1.f };
            transform.rotation = { 0.f, 0.f, -1.57f };
            ECS::addComponent(ent, transform);

            RenderComponent renderComponent;
            renderComponent.modelId = ModelId::TrackLeft;
            ECS::addComponent(ent, renderComponent);
        }
    }

    void Sandbox::unload()
    {
    }

    void Sandbox::update(float dt)
    {

    }

    void Sandbox::_on_key_pressed(const CommonEvents::KeyPressed& evt)
    {
        if (evt.action == InputAction::Pressed)
        {
            switch (evt.key)
            {
            case KeyCode::Escape: m_ui.showPauseMenu(); break;
            }
        }
    }
}