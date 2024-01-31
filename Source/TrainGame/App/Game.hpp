#pragma once

#include <Engine/Graphic/Window.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Utils/Timer.hpp>

#include "TrainGame/App/View.hpp"

#include "UI/MainUI.hpp"

#include "TrainGame/Engine/Systems/TrainControllerSystem.h"
#include "TrainGame/Engine/Systems/GridSystem.h"

namespace Thomas
{
    class Game : private Event::Subscriber
    {
    public:
        Game();
        ~Game();

        Window* window() const { return m_window.get(); }

        void createScene();
    private:
        void onStateUpdate(const CommonEvents::StateUpdated& evt);
        void onKeyPressed(const CommonEvents::KeyPressed& evt);
        void onMouseMoved(const CommonEvents::MouseMoved& evt);
        void onSceneFinishedRender(const SceneEvents::RenderFinished& evt);

        Timer::Chronometre      m_timer;
        std::unique_ptr<Window> m_window;
        std::shared_ptr<View>   m_view;
        std::unique_ptr<MainUI> m_ui;

        std::shared_ptr<TrainControllerSystem> m_trainControllerSystem;
        std::shared_ptr<GridSystem> m_gridSystem;
    };
} // namespace Thomas