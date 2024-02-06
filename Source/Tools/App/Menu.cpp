#include "Menu.hpp"

#include "../_imgui/imgui.h"
#include "../_imgui/imgui_impl_glfw.h"
#include "../_imgui/imgui_impl_opengl3.h"

// Start ImGui
Menu::Menu(GLFWwindow* backend) 
{
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(backend, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

// Cleanup
Menu::~Menu() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}

// Copied from imgui_demo.cpp
void Menu::ShowMenuBar()
{
    if (!ImGui::BeginMainMenuBar())
        return;

    if (ImGui::BeginMenu("File")) {
        ShowMenuFile();
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Editors")) {
        if (ImGui::MenuItem("Clear")) {
            state.editorId = EditorId::None;
        }

        if (ImGui::MenuItem("Material Editor")) {
            state.editorId = EditorId::Material;
        }

        if (ImGui::MenuItem("Model Editor")) {
            state.editorId = EditorId::Model;
        }

        if (ImGui::MenuItem("World Editor")) {
            state.editorId = EditorId::World;
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void Menu::ShowMenuFile()
{
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowMenuFile(); // lol it's fun
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}

    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Border);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Colors"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }

    if (ImGui::MenuItem("Checked", NULL, true)) {}
    ImGui::Separator();

    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void Menu::Prepare()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Menu::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
