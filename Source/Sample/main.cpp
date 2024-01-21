#include <Engine/Graphic/Window.hpp>

#include "App/Controller.hpp"

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
            }
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (window.update());

    // Cleanup
    return 0;
}