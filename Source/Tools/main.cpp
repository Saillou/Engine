#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>

#include "Gui/Menu.hpp"

// Entry point
int main() 
{
    Window window(1280, 720, "Model creation");
    Menu menu(window.backend());

    Light light(glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 });

    Sphere sphere_model(0.15f);
    sphere_model.addRecipe(Cookable::CookType::Solid);

    while (window.update()) 
    {
        window.scene()->clear();
        
        {
            UShader& shader = sphere_model.get(Cookable::CookType::Solid);
            shader->use();
            shader->set("color", glm::vec4(menu.state.color.x, menu.state.color.y, menu.state.color.z, menu.state.color.w));

            if (menu.state.lightEnabled)
                sphere_model.draw(window.scene()->camera(), {}, {}, { light });
            else
                sphere_model.draw(window.scene()->camera());
        }

        menu.Prepare();
        {
            menu.ShowMenuBar();
            menu.ShowMovableOptions();
        }
        menu.Render();
    }

    return 0;
}
