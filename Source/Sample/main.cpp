#include <Engine/Utils/Service.hpp>
#include <Engine/Graphic/Window.hpp>

#include "App/View.hpp"
#include "App/Controller.hpp"
#include "App/Ui.hpp"

// -- Entry point --
int main() {
    // Setup
    Service<Window>::build(1600, 900, "Sample scene");

    View       view;
    Ui         ui;
    Controller controller(ui, view);

    // Loop
    do {
        // Check stuff
        if (ui.wantQuit())
            Service<Window>::get().close();

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (Service<Window>::get().update());

    // Cleanup
    Service<Window>::destroy();
    return 0;
}