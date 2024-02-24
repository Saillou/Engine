#include "Window.hpp"

#include "../Framework/Core/ECS.hpp"
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
    ECS::init();

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

    // Render | Overlay
    if (m_scene) {
        m_scene->run();
    }

    glfwSwapBuffers(m_window);

    // Inputs
    glfwPollEvents();
    _manage_inputs();

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

    // Use OpenGL 3.3 (for best ratio compatibility/features)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // AA

    m_window = glfwCreateWindow(m_width, m_height, title, m_is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!m_window)
        return; // Failure, will close

    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // Load gl entry points
    glfwSwapInterval(1);                                // Enable v-sync

    // Events
    _set_events();

    // Basic scene
    m_scene = std::make_shared<Scene>();
    _resize(m_width, m_height);
}

// Private
void Window::_resize(int width, int height) {
    if (m_scene) {
        m_scene->resize(width, height);
    }

    m_width  = width;
    m_height = height;

    update();
}

void Window::_set_events() {
    // store `this` in m_window's userdata
    glfwSetWindowUserPointer(m_window, this);

    // Resize
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* this_window, int new_width, int new_height) {
        ((Window*)(glfwGetWindowUserPointer(this_window)))->_resize(new_width, new_height);
    });

    // Mouse position
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* this_window, double mouse_x, double mouse_y) {
        Window& self = *((Window*)(glfwGetWindowUserPointer(this_window)));
        self.m_mouse_pos.x = int(mouse_x);
        self.m_mouse_pos.y = int(mouse_y);

        Event::Emit(CommonEvents::MouseMoved(self.m_mouse_pos.x, self.m_mouse_pos.y));
    });

    // Mouse buttons
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* this_window, int button_id, int action_id, int /*mods*/) {
        Window& self = *((Window*)(glfwGetWindowUserPointer(this_window)));

        // Case is handled in _manage_inputs
        if (action_id == GLFW_REPEAT)
            return;

        InputAction action = action_id == GLFW_PRESS ? InputAction::Pressed : InputAction::Released;

        self.m_buttons_pressed[button_id] = action_id != InputAction::Released;
        Event::Emit(CommonEvents::MouseButton(button_id, action, self.m_mouse_pos.x, self.m_mouse_pos.y));
    });

    // Keyboard buttons
    glfwSetKeyCallback(m_window, [](GLFWwindow* this_window, int key_code, int /*scancode*/, int action_id, int /*mods*/) {
        Window& self = *((Window*)(glfwGetWindowUserPointer(this_window)));
        
        // Case is handled in _manage_inputs
        if (action_id == GLFW_REPEAT)
            return;

        InputAction action = action_id == GLFW_PRESS ? InputAction::Pressed : InputAction::Released;

        self.m_keys_pressed[key_code] = action_id != InputAction::Released;
        Event::Emit(CommonEvents::KeyPressed(key_code, action));
    });
}

void Window::_manage_inputs() {
    // Point of this function is to avoid the delay between the events for "Pressed" and "Repeated"

    // Mouse buttons
    for (const auto& button : m_buttons_pressed) {
        if (!button.second)
            continue;

        if (glfwGetMouseButton(m_window, button.first) == GLFW_RELEASE)
            continue;

        Event::Emit(CommonEvents::MouseButton(button.first, InputAction::Repeated, m_mouse_pos.x, m_mouse_pos.y));
    }

    // Keyboard keys
    for (const auto& key : m_keys_pressed) {
        if (!key.second)
            continue;

        if (glfwGetKey(m_window, key.first) == GLFW_RELEASE)
            continue;

        Event::Emit(CommonEvents::KeyPressed(key.first, InputAction::Repeated));
    }
}
