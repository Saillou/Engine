#include <Engine/Graphic/Window.hpp>

#include "App/View.hpp"
#include "App/Controller.hpp"
#include "App/Ui.hpp"

// -- Entry point --
int main() {
    // Setup
    Window     window(1600, 900, "Sample scene");
    View       view(window.scene());
    Ui         ui(window.scene());
    Controller controller(ui, view);

    // Loop
    do {
        // Inputs
        for (auto key : window.keyPressed()) {
            switch (key) {
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