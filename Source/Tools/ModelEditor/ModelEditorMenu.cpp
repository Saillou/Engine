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
    {
        if (ImGui::Button("-##1", { 20,20 }))
        {
            state.scale.x -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Scale X", &state.scale.x, 0.01f, 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+##1", { 20,20 }))
        {
            state.scale.x += 0.005f;
        }
    }

    {
        if (ImGui::Button("-##2", { 20,20 }))
        {
            state.scale.y -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Scale Y", &state.scale.y, 0.01f, 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+##2", { 20,20 }))
        {
            state.scale.y += 0.005f;
        }
    }

    {
        if (ImGui::Button("-##3", { 20,20 }))
        {
            state.scale.z -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Scale Z", &state.scale.z, 0.01f, 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+##3", { 20,20 }))
        {
            state.scale.z += 0.005f;
        }
    }

    ImGui::Text("Translation");
    {
        if (ImGui::Button("-##4", { 20,20 }))
        {
            state.position.x -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Pos X", &state.position.x, -3.0f, 3.0f);
        ImGui::SameLine();
        if (ImGui::Button("+##4", { 20,20 }))
        {
            state.position.x += 0.005f;
        }
    }

    {
        if (ImGui::Button("-##5", { 20,20 }))
        {
            state.position.y -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Pos Y", &state.position.y, -3.0f, 3.0f);
        ImGui::SameLine();
        if (ImGui::Button("+##5", { 20,20 }))
        {
            state.position.y += 0.005f;
        }
    }

    {
        if (ImGui::Button("-##6", { 20,20 }))
        {
            state.position.z -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Pos Z", &state.position.z, -3.0f, 3.0f);
        ImGui::SameLine();
        if (ImGui::Button("+##6", { 20,20 }))
        {
            state.position.z += 0.005f;
        }
    }

    ImGui::Text("Rotation");
    static bool rotationX = state.rotationVector.x > 0.1f;
    static bool rotationY = state.rotationVector.y > 0.1f;
    static bool rotationZ = state.rotationVector.z > 0.1f;

    {
        if (ImGui::Button("-##7", { 20,20 }))
        {
            state.rotation -= 0.005f;
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Angle", &state.rotation, 0.f, 6.28f);
        ImGui::SameLine();
        if (ImGui::Button("+##7", { 20,20 }))
        {
            state.rotation += 0.005f;
        }
    }

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

    ImGui::Begin("Center position");
    ImGui::Text("Center position");
    ImGui::SliderFloat("Center X", &state.centerPosition.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Center Y", &state.centerPosition.y, -1.0f, 1.0f);
    ImGui::SliderFloat("Center Z", &state.centerPosition.z, -1.0f, 1.0f);

    ImGui::Text("World position");
    ImGui::SliderFloat("Pos X", &state.worldPosition.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Pos Y", &state.worldPosition.y, -1.0f, 1.0f);
    ImGui::SliderFloat("Pos Z", &state.worldPosition.z, -1.0f, 1.0f);

    ImGui::Text("Camera settings");
    ImGui::Checkbox("Wireframe", &state.showWireFrame);

    if (ImGui::RadioButton("Camera Side", &state.cameraType, 0))
    {

    }
    if (ImGui::RadioButton("Camera Top", &state.cameraType, 1))
    {

    }
    if (ImGui::RadioButton("Camera Front", &state.cameraType, 2))
    {

    }
    ImGui::End();
}

void ModelEditorMenu::resetState()
{
    state =
    {
        {0,0,0},                                        // position
        {1.f,0,0},                                      // rotation vector
        {0.1f,0.1f,0.1f},                               // scale
        0.f,                                            // rotation
        "Resources/objects/train/wagon_no_wheels.glb",  // asset
        true,                                           // should update asset

        {0,0,0},                                        // center
        {0,0,0},                                        // world pos
        false,                                          // show Wireframe
        0,                                              // camera type
    };
}
