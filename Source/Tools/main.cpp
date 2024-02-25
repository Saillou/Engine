#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Window.hpp>

#include "App/App.hpp"

// Entry point
int main() {
    Service<Window>::build(1600, 900, "Tools");
    App app(Service<Window>::get());

    do {  
        // Think about the lifes's meaning
    } while (Service<Window>::get().update());

    Service<Window>::destroy();
    return 0;
}
