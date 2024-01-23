#pragma once

#include <memory>
#include <vector>

#include <Engine/Graphic/Base/Model/Entity.hpp>
#include <Engine/Graphic/Base/Scene.hpp>

#include "MaterialEditorMenu.hpp"

class MaterialEditor
{
public:
    MaterialEditor(Scene& scene);
    ~MaterialEditor();

    void onEnter();
    void onExit();
    void onUpdate();

private:
    Scene& scene;

    std::unique_ptr<Entity> m_entity;
    MaterialEditorMenu      m_menu;
};
