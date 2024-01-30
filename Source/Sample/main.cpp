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
        // Check stuff
        if (ui.wantQuit())
            window.close();

        //// States, Physics ..
        //{
        //    Event::Emit(CommonEvents::StateUpdated());
        //}
    } while (window.update());

    // Cleanup
    return 0;
}