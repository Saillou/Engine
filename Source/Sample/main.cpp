#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Utils/Service.hpp>

#include "Menu.hpp"
#include "App.hpp"

// -- Entry point --
int main() {
    // Setup
    Service<Window>::build(1600, 900, "Sample scene");
    Service<Menu>::build();
    Service<App>::build();

    // Loop
    do {
        // Check stuff
        if (Service<App>::get().wantQuit())
        {
            Service<Window>::get().close();
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (Service<Window>::get().update());

    // Cleanup
    Service<App>::destroy();
    Service<Menu>::destroy();
    Service<Window>::destroy();
    return 0;
}