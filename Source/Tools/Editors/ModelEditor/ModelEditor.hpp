#pragma once

#include "../Editor.hpp"
#include "ModelEditorMenu.hpp"

class ModelEditor : public Editor
{
public:
    ModelEditor(Scene& scene);
    ~ModelEditor() = default;

    void onEnter()  override;
    void onExit()   override;
    void onUpdate() override;

private:
    Model::Ref m_center;
    Model::Ref m_model;
    ModelEditorMenu m_menu;
};
