#include <Engine/Graphic/Window.hpp>

#include "App/Controller.hpp"

// -- Entry point --
int main() {
    // Setup
    Window      window(1600, 900, "Sample scene");
    View        view(window.scene());
    Controller  controller(view);

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