#include "WorldEditorMenu.hpp"

void WorldEditorMenu::show()
{
    ImGui::Begin("Tool");
    ImGui::End();
}

void WorldEditorMenu::reset()
{
    state = { };
}
