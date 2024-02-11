#include <Engine/Graphic/Window.hpp>

#include "App.hpp"
#include "Menu.hpp"
#include "../Editors/MaterialEditor/MaterialEditor.hpp"
#include "../Editors/ModelEditor/ModelEditor.hpp"
#include "../Editors/WorldEditor/WorldEditor.hpp"

#include <unordered_map>

App::App(Window& window) :
    menu(window.backend()),
    current_editor_id(EditorId::None)
{
    editors[EditorId::Material] = std::make_shared<MaterialEditor>(window.scene());
    editors[EditorId::Model]    = std::make_shared<ModelEditor>(window.scene());
    editors[EditorId::World]    = std::make_shared<WorldEditor>(window.scene());

    _subscribe(&App::pre_draw);
    _subscribe(&App::draw);
    _subscribe(&App::end_draw);

    window.scene().directDraw(true);
}

void App::pre_draw(const SceneEvents::PreDraw&) {
    if (menu.state.editorId != current_editor_id)
        _changeEditor();

    menu.Prepare();
    menu.ShowMenuBar();
}

void App::draw(const SceneEvents::Draw&) {
    // Nothing to update
    if (current_editor_id == EditorId::None)
        return;

    editors[current_editor_id]->onUpdate();
}

void App::end_draw(const SceneEvents::RenderFinished&) {
    menu.Render();
}

// Private
void App::_changeEditor() {
    // Previous editor: exit
    if (current_editor_id != EditorId::None)
        editors[current_editor_id]->onExit();

    // New editor: enter
    if (menu.state.editorId != EditorId::None)
        editors[menu.state.editorId]->onEnter();
    
    // App state
    current_editor_id = menu.state.editorId;
}
