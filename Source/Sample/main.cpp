#include <Engine/Graphic/Window.hpp>

#include "App/Controller.hpp"
#include "App/View.hpp"

// -- Entry point --
int main() {
    // Setup
    Window window(1600, 900, "Sample scene");
    Controller controller(window.scene(std::make_shared<View>(window.width(), window.height())));

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

        if (window.mouseMoved()) {
            Event::Emit(CommonEvents::MouseMoved(
                (int)window.mousePos().x, (int)window.mousePos().y
            ));
        }

        for (auto button : window.buttonPressed()) {
            Event::Emit(CommonEvents::MouseClicked(button));
            break;
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (window.update());

    // Cleanup
    return 0;
}