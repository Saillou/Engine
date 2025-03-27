#include <Engine/Graphic/Window.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Framework/Service.hpp>

#include "Game.hpp"

int main() {
    Window& window = Service<Window>::build(1600, 900, "Isit");
    Game game;

    do {
        // Check stuff
        if (game.wantQuit())
        {
            window.close();
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (window.update());

    return 0;
}