#pragma once

#include <memory>
#include <vector>

#include <Engine/Graphic/Window.hpp>
#include "ModelEditorMenu.hpp"

#include <Engine/Graphic/Base/Model/Entity.hpp>

class ModelEditor
{
public:
    ModelEditor(Window* window);
    ~ModelEditor();

    void onEnter();
    void onExit();
    void onUpdate();

private:
    Window* m_window;

    std::unique_ptr<Entity>  m_center;
    std::unique_ptr<Entity>  m_model;
    std::vector<Light>       m_lights;
    ModelEditorMenu          m_menu;
};
