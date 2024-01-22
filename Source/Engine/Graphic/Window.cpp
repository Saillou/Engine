#include "Window.hpp"

#include "../Events/CommonEvents.hpp"

#include <vector>
#include <memory>
#include <iostream>

Window::Window(int width, int height, const char* title, bool start_fs) : 
    m_width(width), 
    m_height(height),
    m_is_fullscreen(start_fs),
    m_title(title)
{
    // Create window
    _init(title);

    // Mouse buttons
    for (unsigned int btn = 0; btn <= GLFW_MOUSE_BUTTON_LAST; btn++) {
        m_buttons_pressed[btn] = false;
    }

    // Keyboard keys
    for (unsigned int key = 0; key <= GLFW_KEY_LAST; key++) {
        m_keys_pressed[key] = false;
    }
}

Window::~Window() {
    glfwTerminate();
}

// Public
bool Window::update() {
    // Update has to be called in the same thread as its constructor
    glfwMakeContextCurrent(m_window);

    // Can't continue
    if (!m_window)
        return false;

    if (glfwWindowShouldClose(m_window))
        return false;

    // Inputs
    glfwPollEvents();
    _manage_inputs();

    // Render
    if (m_scene) {
        m_scene->run();
    }

    glfwSwapBuffers(m_window);

    return true;
}

void Window::close() {
    if (!m_window)
        return;

    glfwSetWindowShouldClose(m_window, true);
}

int Window::width() const {
    return m_width;
}

int Window::height() const {
    return m_height;
}

// Getters
std::vector<unsigned int> Window::keyPressed() const {
    if (!m_window)
        return {};

    std::vector<unsigned int> keys;
    for (const auto& key : m_keys_pressed) {
        if (key.second) {
            keys.push_back(key.first);
        }
    }

    return keys;
}

std::vector<unsigned int> Window::buttonPressed() const {
    if (!m_window)
        return {};

    std::vector<unsigned int> buttons;
    for (const auto& button : m_buttons_pressed) {
        if (button.second) {
            buttons.push_back(button.first);
        }
    }

    return buttons;
}

glm::ivec2 Window::mousePos() const {
    return m_mouse_pos;
}

Scene& Window::scene() {
    return *m_scene;
}

GLFWwindow* Window::backend() {
    return m_window;
}

void Window::toggleFullScreen() {
    m_is_fullscreen = !m_is_fullscreen;

    if (m_is_fullscreen) {
        // recreate the window with glfwGetPrimaryMonitor() as parameter
    }
    else {
        // put back how it was before
    }

}

void Window::_init(const char* title) {
    // Init GLFW
    glfwInit();

    // OpenGL 3.3 (for best ratio compatibility/features)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16); // AA

    m_window = glfwCreateWindow(m_width, m_height, title, m_is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!m_window)
        return; // Failure, will close

    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // Load gl entry points
    glfwSwapInterval(1);                                // Enable v-sync

    // Events
    glfwSetWindowUserPointer(m_window, this); // store `this` in m_window's userdata

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* this_window, int new_width, int new_height) {
        ((Window*)(glfwGetWindowUserPointer(this_window)))->_resize(new_width, new_height);
    });

    // Basic scene
    m_scene = std::make_shared<Scene>();
    _resize(m_width, m_height);
}

// Private
void Window::_resize(int width, int height) {
    Scene::Viewport(width, height);
    TextEngine::SetViewport(0, 0, width, height);

    if (m_scene)
        m_scene->resize(width, height);

    m_width  = width;
    m_height = height;

    update();
}

void Window::_manage_inputs() {
    // Mouse position
    {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);

        if (int(x) != m_mouse_pos.x && int(y) != m_mouse_pos.y) {
            m_mouse_pos.x = int(x);
            m_mouse_pos.y = int(y);

            Event::Emit(CommonEvents::MouseMoved(mousePos().x, mousePos().y));
        }
    }

    // Mouse buttons
    for (const auto& button : m_buttons_pressed) {
        unsigned int button_id = button.first;
        bool button_state      = button.second;

        bool stateOn = glfwGetMouseButton(m_window, button_id) == GLFW_PRESS;

        bool isPressed  = stateOn  && !button_state;
        bool isRepeated = stateOn  &&  button_state;
        bool isReleased = !stateOn &&  button_state;

        if (!stateOn && !button_state)
            continue;

        Action action = isPressed  ? Action::Pressed:
                        isRepeated ? Action::Repeated:
                                     Action::Released;

        m_buttons_pressed[button_id] = stateOn;
        Event::Emit(CommonEvents::MouseButton(button_id, action));
    }


    // Keyboard keys
    for (const auto& key : m_keys_pressed) {
        unsigned int key_id = key.first;
        bool key_state      = key.second;

        bool stateOn = glfwGetKey(m_window, key_id) == GLFW_PRESS;

        bool isPressed  = stateOn  && !key_state;
        bool isRepeated = stateOn  &&  key_state;
        bool isReleased = !stateOn &&  key_state;

        if (!stateOn && !key_state)
            continue;

        Action action = isPressed  ? Action::Pressed:
                        isRepeated ? Action::Repeated:
                                     Action::Released;

        m_keys_pressed[key_id] = stateOn;
        Event::Emit(CommonEvents::KeyPressed(key_id, action));
    }
}
