#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>

#include "Menu.hpp"

#include "MaterialEditor/MaterialEditor.h"
#include "ModelEditor/ModelEditor.h"

// Entry point
int main() 
{
    Window window(1280, 720, "Tools");
    Menu menu(window.backend());

    MaterialEditor materialEditor(&window);
    bool showMaterialEditor = false;

    ModelEditor modelEditor(&window);
    bool showModelEditor = false;

    while (window.update()) 
    {
        window.scene()->clear();

        //window.scene()->camera().lookAt(glm::vec3(0, 0, 0.01f));
        
        if (menu.state.goToNewEditor)
        {
            menu.state.goToNewEditor = false;

            switch (menu.state.editorId)
            {
            case EditorId::None:
                showMaterialEditor = false;
                showModelEditor = false;
                materialEditor.onExit();
                modelEditor.onExit();
                break;
            case EditorId::Material:
                showMaterialEditor = true;
                showModelEditor = false;
                materialEditor.onEnter();
                modelEditor.onExit();
                break;
            case EditorId::Model:
                showMaterialEditor = false;
                showModelEditor = true;
                materialEditor.onExit();
                modelEditor.onEnter();
                break;
            }
        }

        menu.Prepare();
        menu.ShowMenuBar();

        if(showMaterialEditor)
            materialEditor.onUpdate();

        if (showModelEditor)
            modelEditor.onUpdate();

        menu.Render();
    }

    return 0;
}
