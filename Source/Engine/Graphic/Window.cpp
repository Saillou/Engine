#include "Window.hpp"

#include <vector>
#include <memory>

Window::Window(int width, int height, const char* title, bool start_fs) : 
    m_width(width), 
    m_height(height),
    m_is_fullscreen(start_fs),
    m_title(title)
{
    _init(title);
}

Window::~Window() {
    glfwTerminate();
}

// Public
bool Window::update() {
    // To be called in the same thread as the m_window constructor
    glfwMakeContextCurrent(m_window);

    // Can't continue states
    if (!m_window)
        return false;

    if (glfwWindowShouldClose(m_window))
        return false;

    // Ok
    if (m_scene)
        m_scene->draw();

    glfwSwapBuffers(m_window);
    glfwPollEvents();

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
    keys.reserve(GLFW_KEY_LAST + 1);

    for (unsigned int key = 0; key <= GLFW_KEY_LAST; key++) {
        if (glfwGetKey(m_window, key) == GLFW_PRESS) {
            keys.push_back(key);
        }
    }

    return keys;
}

glm::vec2 Window::mousePos() const {
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return glm::vec2(x, y);
}

std::shared_ptr<BaseScene> Window::scene() const {
    return m_scene;
}

GLFWwindow* Window::backend() {
    return m_window;
}

// Setters
std::shared_ptr<BaseScene> Window::scene(std::shared_ptr<BaseScene> scene) {
    m_scene.swap(scene);
    if (!m_scene)
        return nullptr;

    // Adjust viewport
    if (height() <= 0)
        return nullptr;

    _resize(width(), height());

    // Get if needed
    return m_scene;
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

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // Load gl entry points
    glfwSwapInterval(1);                                // Enable v-sync

    // Events
    glfwSetWindowUserPointer(m_window, this); // store `this` in m_window's userdata

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* this_window, int new_width, int new_height) {
        ((Window*)(glfwGetWindowUserPointer(this_window)))->_resize(new_width, new_height);
    });

    // Basic scene
    m_scene = std::make_shared<BaseScene>();
    m_scene->resize(m_width, m_height);
}

// Private
void Window::_resize(int width, int height) {
    BaseScene::Viewport(width, height);
    TextEngine::SetViewport(0, 0, width, height);

    if (m_scene)
        m_scene->resize(width, height);

    m_width  = width;
    m_height = height;

    update();
}