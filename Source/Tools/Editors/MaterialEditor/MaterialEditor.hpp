#pragma once

#include "../Editor.hpp"
#include "MaterialEditorMenu.hpp"

class MaterialEditor : public Editor
{
public:
    MaterialEditor(Scene& scene);
    ~MaterialEditor() = default;

    void onEnter()  override;
    void onExit()   override;
    void onUpdate() override;

private:
    Model::Ref         m_model;
    MaterialEditorMenu m_menu;
};
