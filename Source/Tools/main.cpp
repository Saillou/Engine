#include <Engine/Utils/Service.hpp>
#include <Engine/Graphic/Window.hpp>

#include "App/App.hpp"

// Entry point
int main() {
    Service<Window>::build(1600, 900, "Tools");
    App app(Service<Window>::get());

    do {
        app.pre_draw();
    } while (Service<Window>::get().update());

    return 0;
}
