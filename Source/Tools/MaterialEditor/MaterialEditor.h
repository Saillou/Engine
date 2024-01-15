#pragma once

#include <memory>
#include <vector>

#include <Engine/Graphic/Base/Model/Entity.hpp>
#include <Engine/Graphic/Window.hpp>
#include "MaterialEditorMenu.hpp"

class MaterialEditor
{
public:
    MaterialEditor(Window* window);
    ~MaterialEditor();

    void onEnter();
    void onExit();
    void onUpdate();

private:
    Window* m_window;

    std::unique_ptr<Entity> m_entity;
    std::vector<Light>      m_lights;
    MaterialEditorMenu      m_menu;
};
