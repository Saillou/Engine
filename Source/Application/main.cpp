#include <string>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Physx/Physx.hpp>
#include <Engine/Graphic/Window.hpp>

#include "Game/ApplicationManager.hpp"

// Options
#ifdef _DEBUG
    const bool FullScreen = false;
    const int Width       = 1600;
    const int Height      = 900;
    const SceneId StartId = SceneId::Learn;
#else
    const bool FullScreen = true;
    const int Width       = 1920;
    const int Height      = 1080;
    const SceneId StartId = SceneId::Learn;
#endif

// -- Entry point --
int main() {
    // Alloc
    ApplicationManager::Create();
    Window window(Width, Height, "A Ball", FullScreen);

    // Main loop
    ApplicationManager::SetScene(StartId);

    Timer::Chronometre chrono;
    do {
        // Read keyboard inputs
        for (auto key : window.keyPressed()) {
            switch (key) {
                // Window control
                case GLFW_KEY_ESCAPE: 
                    window.close(); 
                    break;

                case GLFW_KEY_1:
                    ApplicationManager::SetScene(SceneId::App);
                    break;

                case GLFW_KEY_2:
                    ApplicationManager::SetScene(SceneId::Fire);
                    break;

                case GLFW_KEY_3:
                    ApplicationManager::SetScene(SceneId::Bloom);
                    break;

                case GLFW_KEY_4:
                    ApplicationManager::SetScene(SceneId::Learn);
                    break;

                case GLFW_KEY_F11:
                    window.toggleFullScreen();
                    break;

                // App control
                default:
                    ApplicationManager::KeyPressed(key);
                    break;
            }
        }

        // Read mouse inputs
        ApplicationManager::MousePos(window.mousePos());

        // Compute world
        switch (ApplicationManager::UpdateState()) 
        {
            // Continue normally
            case ApplicationManager::ActionCode::Ok:
                Physx::Compute(chrono.elapsed<Timer::microsecond>()/1000.0f);
                chrono.tic();
                break;

            // Create or refresh scene
            case ApplicationManager::ActionCode::Refresh:
                ApplicationManager::Refresh(window);
                break;

            // Stop
            case ApplicationManager::ActionCode::Close:
                window.close();
                break;
        }
    } while (window.update());

    // Clean up
    ApplicationManager::Destroy();
    return 0;
}