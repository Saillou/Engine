#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Some helper functions
void ShowMenuBar();
void ShowMenuFile();

// Entry point
int main() {
    // Create a surface to draw on
    Window window(1280, 720, "Model creation");
    window.scene()->lights() = {
        Light(glm::vec3{ 0, -0.5f, 0.5f }, glm::vec4{ 1,1,1,1 }),
    };

    // Create an object
    Sphere sphere_model(0.15f);
    sphere_model.addRecipe(Cookable::CookType::Solid);

    // Setup Dear ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad; 

    ImGui_ImplGlfw_InitForOpenGL(window.backend(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGui::StyleColorsDark();

    // State
    bool lightEnabled(true);
    ImVec4 color(1.0f, 0.7f, 0.3f, 1.00f);

    // Main loop
    while (window.update()) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Menu bar sticked on top
        {
            ShowMenuBar();
        }

        // Movable menu on scene
        {
            ImGui::Begin("Hello, world!");
            ImGui::Text("This is some useful text.");  

            ImGui::Checkbox("Enable light", &lightEnabled);
            ImGui::SliderFloat("alpha", &color.w, 0.0f, 1.0f);
            ImGui::ColorEdit3("color", (float*)&color);   

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        window.scene()->clear();

        UShader& shader = sphere_model.get(Cookable::CookType::Solid);
        shader->use();
        shader->set("color", glm::vec4(color.x, color.y, color.z, color.w));

        if (lightEnabled)
            sphere_model.draw(window.scene()->camera(), {}, {}, window.scene()->lights());
        else
            sphere_model.draw(window.scene()->camera());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

// Impl: copied from imgui_demo
static void ShowMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void ShowMenuFile()
{
    ImGui::MenuItem("(demo menu)", NULL, false, false);
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
