#pragma once

#include <memory>
#include <vector>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

#include "ModelEditorMenu.hpp"

class ModelEditor
{
public:
    ModelEditor(Scene& scene);
    ~ModelEditor();

    void onEnter();
    void onExit();
    void onUpdate();

private:
    Scene& scene;

    std::unique_ptr<Entity>  m_center;
    std::unique_ptr<Entity>  m_model;
    ModelEditorMenu          m_menu;
};
