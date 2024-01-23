#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Model/Primitive/Sphere.hpp>

#include "Menu.hpp"

#include "MaterialEditor/MaterialEditor.h"
#include "ModelEditor/ModelEditor.h"

struct App: Event::Subscriber {
    App(Window& window);

    void App::pre_draw();
    void App::draw(const SceneEvents::Draw&);
    void App::end_draw(const SceneEvents::RenderFinished&);

    Menu menu;
    MaterialEditor materialEditor;
    ModelEditor modelEditor;

    bool showMaterialEditor = false;
    bool showModelEditor = false;
};

// Entry point
int main() {
    Window window(1280, 720, "Tools");
    App app(window);

    do {
        app.pre_draw();
    } while (window.update());

    return 0;
}

// App
App::App(Window& window) :
    menu(window.backend()), 
    materialEditor(window.scene()),
    modelEditor(window.scene())
{
    _subscribe(&App::draw);
    _subscribe(&App::end_draw);

    window.scene().directDraw(true);
}

void App::pre_draw() {
    if (!menu.state.goToNewEditor)
        goto Update;

    menu.state.goToNewEditor = false;

    switch (menu.state.editorId) {
    case EditorId::None:
        showMaterialEditor = false;
        showModelEditor    = false;
        materialEditor.onExit();
        modelEditor.onExit();
        break;

    case EditorId::Material:
        showMaterialEditor = true;
        showModelEditor    = false;
        materialEditor.onEnter();
        modelEditor.onExit();
        break;

    case EditorId::Model:
        showMaterialEditor = false;
        showModelEditor    = true;
        materialEditor.onExit();
        modelEditor.onEnter();
        break;
    }

Update:
    menu.Prepare();
    menu.ShowMenuBar();
}

void App::draw(const SceneEvents::Draw&) {
    if (showMaterialEditor)
        materialEditor.onUpdate();

    if (showModelEditor)
        modelEditor.onUpdate();
}

void App::end_draw(const SceneEvents::RenderFinished&) {
    menu.Render();
}