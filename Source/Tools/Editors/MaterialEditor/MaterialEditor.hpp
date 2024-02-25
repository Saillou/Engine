#pragma once

#include "../Editor.hpp"
#include "MaterialEditorMenu.hpp"
#include <Engine/Framework/Helper/ManagedEntity.hpp>

class MaterialEditor : public Editor
{
public:
    MaterialEditor(Scene& scene);
    ~MaterialEditor() = default;

    void onEnter()  override;
    void onExit()   override;
    void onUpdate() override;

private:
    SharedEntity m_sphere;
    MaterialEditorMenu m_menu;
};
