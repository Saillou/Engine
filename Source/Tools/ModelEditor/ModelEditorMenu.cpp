#include "ModelEditorMenu.hpp"

// Copied from imgui_sample.cpp
void ModelEditorMenu::ShowMovableOptions()
{
    ImGui::Begin("Hello, world!");

    static char path[128];
    path[0] = '\0';
    if (ImGui::InputText("##AssetPath", path, 128))
    {
        state.path = path;
        state.updateAsset = true;
    }
    ImGui::Text("Scale");
    ImGui::SliderFloat("Scale X", &state.scale.x, 0.01f, 1.0f);
    ImGui::SliderFloat("Scale Y", &state.scale.y, 0.01f, 1.0f);
    ImGui::SliderFloat("Scale Z", &state.scale.z, 0.01f, 1.0f);
    ImGui::Text("Translation");
    ImGui::SliderFloat("Pos X", &state.position.x, -3.0f, 3.0f);
    ImGui::SliderFloat("Pos Y", &state.position.y, -3.0f, 3.0f);
    ImGui::SliderFloat("Pos Z", &state.position.z, -3.0f, 3.0f);
    //ImGui::ColorEdit3("color", (float*)&state.color);
    ImGui::Text("Rotation");
    static bool rotationX = false;
    static bool rotationY = false;
    static bool rotationZ = false;

    ImGui::SliderFloat("Angle", &state.rotation, 0.f, 6.28f);
    if (ImGui::Checkbox("Rotation X", &rotationX))
    {
        state.rotationVector.x = rotationX ? 1.f : 0.f;
    }

    if (ImGui::Checkbox("Rotation Y", &rotationY))
    {
        state.rotationVector.y = rotationY ? 1.f : 0.f;
    }

    if (ImGui::Checkbox("Rotation Z", &rotationZ))
    {
        state.rotationVector.z = rotationZ ? 1.f : 0.f;
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void ModelEditorMenu::resetState()
{
    state =
    {
        {0,0,0},
        {0.5f,0,0},
        {0.1f,0.1f,0.1f},
        1.5f,
        "Resources/objects/train/wagon_no_wheels.glb",
        true
    };
}
