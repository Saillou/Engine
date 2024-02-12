#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Framework/Service.hpp>

#include "AppMenu.hpp"
#include "App.hpp"

// -- Entry point --
int main() {
    // Setup
    Service<Window>::build(1600, 900, "Sample scene");
    Service<App>::build();
    Service<AppMenu>::build();

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
    Service<AppMenu>::destroy();
    Service<App>::destroy();
    Service<Window>::destroy();
    return 0;
}