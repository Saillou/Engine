#pragma once

#include <memory>
#include <vector>

#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Window.hpp>
#include "ModelEditorMenu.hpp"

#include <Engine/Graphic/Base/Model/ObjectModel.hpp>

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

    std::unique_ptr<Sphere>         m_center;
    std::unique_ptr<ObjectModel>    m_model;
    std::vector<Light>              m_lights;
    ModelEditorMenu                 m_menu;
};
