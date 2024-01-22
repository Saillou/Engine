#include <Engine/Graphic/Window.hpp>

#include "App/View.hpp"

#include "TrainGame/TrainGame/TrainGameModels.hpp"

#include "App/Game.hpp"

void initGameModels();

// -- Entry point --
int main() {
    Thomas::Game game;
    game.createScene();

    initGameModels();

    // Loop
    do {
        // Inputs
        for (auto key : game.window()->keyPressed()) {
            switch (key) 
            {
            case GLFW_KEY_ESCAPE:
                game.window()->close();
                break;

            default:
                Event::Emit(CommonEvents::KeyPressed(key));
                break;
            }
        }

        Event::Emit(CommonEvents::MouseMoved(
            (int)game.window()->mousePos().x, (int)game.window()->mousePos().y
        ));

        for (auto button : game.window()->buttonPressed()) {
            Event::Emit(CommonEvents::MouseButton(button, Action::Pressed));
            break;
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (game.window()->update());

    // Cleanup
    return 0;
}

void initGameModels()
{
    // TODO: Load models here

    // Misha: is it really a good way of doing it ?
    // TODO: brainstorm about it
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::Locomotive, { { -0.04f, 0.f, -0.12f }, { 1.f, 1.f, 1.f }, {1.57f, -1.57f, 0.f} });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::Wagon, { { -1.75f, -0.175f, -0.155f }, { 0.01f, 0.01f, 0.01f }, { 1,0,0 } });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::Track, { { -0.05f, 0.f, -0.245f }, { 1.f, 1.f, 1.f }, { 1.57, -1.57,0 } });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::TrackLeft, { { +0.057f, -0.02f, -0.534f }, { 1.f, 1.f, 1.f }, { 1.57f, 0.785f,0.f } });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::TrackRight, { { -0.09f, -0.06f, -0.495f }, { 1.f, 1.f, 1.f }, { 1.57f, 0.785f,0 } });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::Building1, { { 0.f, +0.1f, -0.22f }, { 0.75f, 0.75f, 0.75f }, { 1.57f, -1.57f,0 } });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::CubeBasic, { { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f,0.f } });
    Thomas::GameModelTable::addGameModel(Thomas::ModelId::CubeGeometry, { { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f,0.f } });
}