#pragma once

#include "../Editor.hpp"
#include "ModelEditorMenu.hpp"
#include <Engine/Framework/Helper/ManagedEntity.hpp>

class ModelEditor : public Editor
{
public:
    ModelEditor(Scene& scene);
    ~ModelEditor() = default;

    void onEnter()  override;
    void onExit()   override;
    void onUpdate() override;

private:
    SharedEntity m_center;
    SharedEntity m_model;
    ModelEditorMenu m_menu;
};
