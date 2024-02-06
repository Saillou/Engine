#pragma once

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
    WorldEditorMenu m_menu;
};
