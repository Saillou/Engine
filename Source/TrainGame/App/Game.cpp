#include "Game.hpp"

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Service.hpp>

#include "TrainGame/TrainGame/Grid.hpp"

#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/TrainController.h"
#include "TrainGame/Engine/Components/Grid.h"
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

        ECS::registerComponent<Transform>();
        ECS::registerComponent<TrainController>();
        ECS::registerComponent<Grid>();
        ECS::registerComponent<RenderComponent>();

        m_trainControllerSystem = ECS::registerSystem<TrainControllerSystem>();
        m_trainControllerSystem->init();

        m_gridSystem = ECS::registerSystem<GridSystem>();
        m_gridSystem->init(); 
        m_gridSystem->start({ 0.f, 0.f, 0.2f }, { 0.1f,0.1f });

        Service<Window>::build(1600, 900, "Train game");
        m_view = std::make_shared<View>(Service<Window>::get().scene());
        m_ui = std::make_unique<MainUI>(Service<Window>::get().backend());
    }

    Game::~Game()
    {
    }

    void Game::createScene()
    {
        m_curr_level = std::make_unique<Sandbox>(); // ntm
        m_curr_level->load();
    }

    void Game::onStateUpdate(const CommonEvents::StateUpdated& evt)
    {
        float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f / 1000.f;

        m_trainControllerSystem->update(dt_ms);
        m_gridSystem->update(dt_ms);

        m_ui->Prepare();
        m_ui->showBuildingSelection();
        m_ui->ready = true;

        m_timer.tic();
    }

    void Game::onKeyPressed(const CommonEvents::KeyPressed& evt)
    {
        glm::vec3 dir(0, 0, 0);
        float scale = 0.f;
        float rot = 0.f;

        switch (evt.key)
        {
        case KeyCode::ArrowRight: dir.x = +1.0f; break;
        case KeyCode::ArrowLeft:  dir.x = -1.0f; break;
        case KeyCode::ArrowUp:    dir.y = +1.0f; break;
        case KeyCode::ArrowDown:  dir.y = -1.0f; break;
        case 'Q':             dir.z = +1.0f; break;
        case 'W':             dir.z = -1.0f; break;
        case 'A':             rot = +1.f; break;
        case 'S':             rot = -1.f; break;
        case 'Z':             scale = -1.f; break;
        case 'X':             scale = +1.f; break;
        }

        // Crazy ..
        Transform& transform = ECS::getComponent<Transform>(3);
        BodyComponent& body = ECS::getComponent<BodyComponent>(3);

        transform.position += 0.05f * dir;
        transform.rotation.z += 0.05f * rot;
        transform.scale += 0.005f * scale;

        body.transform.world = transform.getMat4();
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