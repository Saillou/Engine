#pragma once

#include <unordered_map>

#include "../Editor.hpp"
#include "WorldEditorMenu.hpp"

class WorldEditor : public Editor
{
public:
    WorldEditor(Scene& scene);
    ~WorldEditor() = default;

    void onEnter()  override;
    void onExit()   override;
    void onUpdate() override;

private:
    void _drawScene();
    void _drawMenu();

    WorldEditorMenu m_menu;
    std::unordered_map<std::string, Entity> m_entities;
};
