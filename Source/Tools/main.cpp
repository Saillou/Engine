#include <Engine/Graphic/Window.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Main code
int main() {
    // Create surface to draw to
    Window window(1280, 720, "Model creation");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad; 

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.backend(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Our state
    bool checkMe = true;
    ImVec4 color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    do {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                

            ImGui::Text("This is some useful text.");     
            ImGui::Checkbox("Demo Window", &checkMe);     

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);  
            ImGui::ColorEdit3("color", (float*)&color);   

            if (ImGui::Button("Button"))                  
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        //Rendering
        window.scene()->clear();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    } while (window.update());

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
