#pragma once

#include <unordered_map>
#include <Engine/Events/CommonEvents.hpp>

#include "WorldEditorMenu.hpp"
#include "../Editor.hpp"

class WorldEditor : 
    public Editor,
    private Event::Subscriber
{
public:
    WorldEditor(Scene& scene);
    ~WorldEditor() = default;

    void onEnter()  override;
    void onExit()   override;
    void onUpdate() override;

private:
    void _drawScene();
    void _drawInfo();

    // - Members -
    WorldEditorMenu m_menu;
    std::unordered_map<std::string, Model::Ref> m_models;
};
