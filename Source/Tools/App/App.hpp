#include <unordered_map>

#include <Engine/Graphic/Window.hpp>

#include "../Editors/MaterialEditor/MaterialEditor.h"
#include "../Editors/ModelEditor/ModelEditor.h"
#include "../Editors/WorldEditor/WorldEditor.h"

#include "Menu.hpp"

struct App : Event::Subscriber {
    App(Window& window);

    void pre_draw();
    void draw(const SceneEvents::Draw&);
    void end_draw(const SceneEvents::RenderFinished&);

private:
    void _changeEditor();

    Menu menu;
    EditorId current_editor_id;
    std::unordered_map<EditorId, std::shared_ptr<Editor>> editors;
};
