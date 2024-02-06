#include "WorldEditor.h"

WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter()
{
    m_menu.reset();
}

void WorldEditor::onExit()
{

}

void WorldEditor::onUpdate()
{
    m_menu.show();
}