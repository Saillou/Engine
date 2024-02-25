#include <unordered_map>

#include <Engine/Graphic/Window.hpp>

#include "../Editors/MaterialEditor/MaterialEditor.hpp"
#include "../Editors/ModelEditor/ModelEditor.hpp"
#include "../Editors/WorldEditor/WorldEditor.hpp"

#include "Menu.hpp"

struct App : Event::Subscriber {
    App(Window& window);

    void pre_draw(const SceneEvents::RenderStarted&);
    void end_draw(const SceneEvents::RenderFinished&);

private:
    void _changeEditor();

    Menu menu;
    EditorId current_editor_id;
    std::unordered_map<EditorId, std::shared_ptr<Editor>> editors;
};
