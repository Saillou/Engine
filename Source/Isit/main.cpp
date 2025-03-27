#include <iostream>

#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Window.hpp>

#include "Game.hpp"

int main() {
    Service<Window>::build(1600, 900, "Isit");
    Service<Game>::build();

    do {
        // Check stuff
        if (Service<Game>::get().wantQuit())
        {
            Service<Window>::get().close();
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (Service<Window>::get().update());

    return 0;
}