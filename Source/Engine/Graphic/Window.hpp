#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Base/BaseScene.hpp"

struct Window {
    Window(int width, int height, const char* title, bool start_fs = false);
    ~Window();

    bool update();
    void close();

    // Getters
    int width()  const;
    int height() const;

    std::vector<unsigned int> keyPressed()    const;
    std::vector<unsigned int> buttonPressed() const;
    glm::ivec2                mousePos()      const;

    std::shared_ptr<BaseScene> scene() const;
    GLFWwindow* backend();

    // Setters
    std::shared_ptr<BaseScene> scene(std::shared_ptr<BaseScene> scene);
    void toggleFullScreen();

private:
    void _init(const char* title);
    void _resize(int width, int height);
    void _manage_inputs();

    // Members
    std::string m_title;
    int         m_width;
    int         m_height;
    bool        m_is_fullscreen;

    glm::ivec2                             m_mouse_pos       = {};
    std::unordered_map<unsigned int, bool> m_keys_pressed    = {};
    std::unordered_map<unsigned int, bool> m_buttons_pressed = {};

    GLFWwindow*                m_window = nullptr;
    std::shared_ptr<BaseScene> m_scene  = nullptr;
};
