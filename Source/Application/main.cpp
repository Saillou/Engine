#include <Engine/Graphic/Window.hpp>

#include "App/Controller.hpp"
#include "App/View.hpp"

#include "Application/TrainGame/TrainGameModels.hpp"

void initGameModels();

// -- Entry point --
int main() {
    // Setup
    Window window(1600, 900, "Sample scene");
    Controller controller(window.scene(std::make_shared<View>(window.width(), window.height())));

    initGameModels();

    // Loop
    do {
        // Inputs
        for (auto key : window.keyPressed()) {
            switch (key) 
            {
            case GLFW_KEY_ESCAPE:
                window.close(); 
                break;

            default:
                Event::Emit(CommonEvents::KeyPressed(key));
                break;
            }
        }

        Event::Emit(CommonEvents::MouseMoved(
            (int)window.mousePos().x, (int)window.mousePos().y
        ));

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (window.update());

    // Cleanup
    return 0;
}

void initGameModels()
{
    // Misha: is it really a good way of doing it ?
    // TODO: brainstorm about it
    GameModelTable::addGameModel(ModelId::Locomotive,   { -0.75f, -1.68f, -0.135f }, { 0.01f, 0.01f, 0.01f }, { 1,0,0 }, 0.f);
    GameModelTable::addGameModel(ModelId::Wagon,        { -1.75f, -0.175f, -0.155f }, { 0.01f, 0.01f, 0.01f }, { 1,0,0 }, 0.f);
}