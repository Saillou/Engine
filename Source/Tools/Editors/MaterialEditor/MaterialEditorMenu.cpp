#include "MaterialEditorMenu.hpp"

// Copied from imgui_sample.cpp
void MaterialEditorMenu::show()
{
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");

    ImGui::Checkbox("Enable light", &state.lightEnabled);
    ImGui::SliderFloat("alpha", &state.color.w, 0.0f, 1.0f);
    ImGui::ColorEdit3("color", (float*)&state.color);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void MaterialEditorMenu::reset()
{
    state = {
        true,
        { 1.0f, 0.7f, 0.3f, 1.0f }
    };
}
