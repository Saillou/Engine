#pragma once

#include <memory>
#include <queue>

#include "../Engine/Graphic/Window.hpp"
#include "../Engine/Events/CommonEvents.hpp"
#include "../Engine/Graphic/Base/BaseCommander.hpp"

// Data
enum SceneId {
    None = 0,
    App, Fire, Bloom, Learn
};

// Singleton
class ApplicationManager : public Event::Subscriber {
    static ApplicationManager* s_appManager;

    // -- No copy --
    ApplicationManager();

    ApplicationManager& operator=(const ApplicationManager&) = delete;
    ApplicationManager(const ApplicationManager&) = delete;
    ApplicationManager(ApplicationManager&&) = delete;

    // Instance members
    SceneId                 _currSceneId = None;
    std::queue<SceneId>     _sceneId    = {};
    std::queue<glm::vec2>   _mousePos   = {};
    std::queue<int>         _keyPressed = {};
    uBaseCommander          _commander  = nullptr;

    // Instance Methods
    void _on_scene_refresh(const CommonEvents::SceneRefresh& evt);
    void _on_scene_ended(const CommonEvents::SceneEnded& evt);

public:
    enum class ActionCode {
        Close = 0, 
        Ok, Refresh
    };

    ~ApplicationManager();

    static void Create();
    static void Destroy();
    static void Refresh(Window&);

    static void SetScene(SceneId);
    static void KeyPressed(unsigned int);
    static void MousePos(const glm::vec2&);

    static ActionCode UpdateState();
};