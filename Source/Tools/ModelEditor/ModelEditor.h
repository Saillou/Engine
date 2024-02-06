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
    std::unique_ptr<Entity>  m_center;
    std::unique_ptr<Entity>  m_model;
    ModelEditorMenu          m_menu;
};
