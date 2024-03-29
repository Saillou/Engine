#include "AppMenu.hpp"

#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Window.hpp>

#include "_imgui/imgui.h"
#include "_imgui/imgui_impl_glfw.h"
#include "_imgui/imgui_impl_opengl3.h"

#include "App.hpp"

// Start ImGui
AppMenu::AppMenu()
{
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Service<Window>::get().backend(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    _prepare();

    // Events
    _subscribe([=](const SceneEvents::RenderStarted&)  { _prepare(); });
    _subscribe([=](const SceneEvents::RenderFinished&) { _render();  });
}

// Cleanup
AppMenu::~AppMenu()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}

// Draw methods
void AppMenu::_prepare()
{
    if (_frame_ready)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    _frame_ready = true;

    if (!ImGui::BeginMainMenuBar())
        return;

    if (ImGui::BeginMenu("Samples")) {
        if (ImGui::MenuItem("While lights cover cubes")) {
            Service<App>::get().changeSample(SampleId::Cube);
        }
        if (ImGui::MenuItem("A forest, a loco and the tortle")) {
            Service<App>::get().changeSample(SampleId::Forest);
        }
        if (ImGui::MenuItem("Trainmania - Desert track")) {
            Service<App>::get().changeSample(SampleId::Train);
        }
        if (ImGui::MenuItem("Have a break")) {
            Service<App>::get().changeSample(SampleId::Breaker);
        }
        if (ImGui::MenuItem("Is it snow?")) {
            Service<App>::get().changeSample(SampleId::Snow);
        }        
        if (ImGui::MenuItem("Feed me, now !")) {
            Service<App>::get().changeSample(SampleId::Canvas);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void AppMenu::_render()
{
    if (!_frame_ready)
        return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    _frame_ready = false;
}
