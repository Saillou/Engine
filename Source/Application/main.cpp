#include <Engine/Graphic/Window.hpp>

#include "App/Controller.hpp"
#include "App/View.hpp"
#include <Engine/Utils/Timer.hpp>

#include <map>

static uint64_t gs_id = 0;

enum class ObjectType
{
    Box = 0,
    Sphere = 1
};

struct GameObject
{
    uint64_t id;
    glm::vec3 position;
    ObjectType type;
};

class Game : private Event::Subscriber
{
public:
    Game()
    {
        _subscribe(&Game::_on_state_update);
        _subscribe(&Game::_on_key_pressed);
        _subscribe(&Game::_on_mouse_moved);

        m_window = std::make_unique<Window>(1600, 900, "Sample scene");
        m_view = std::make_shared<View>((m_window->width(), m_window->height()));
        m_window->scene(m_view);

        m_ballSpeed = glm::vec3(0, 0, -1.f);
    }

    ~Game()
    {
        // clean all objects here
    }

    Window* window() const { return m_window.get(); }

    void CreateScene()
    {
        GameObject* box1 = createBox(glm::vec3(0, 0, 1.5f));
        GameObject* box2 = createBox(glm::vec3(0, 0, -1.5f));
        GameObject* ball = createBall(glm::vec3(0, 0, 0.f));

        m_objects[box1->id] = box1;
        m_objects[box2->id] = box2;
        m_objects[ball->id] = ball;
    }

private:
    void _on_state_update(const CommonEvents::StateUpdated& evt)
    {
        float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f / 1000.f;

        // physic of ball here: m_scene->ballPos(new pos);
        //m_view->moveBallPos(dt_ms * m_ballSpeed);
        m_objects[2]->position += dt_ms * m_ballSpeed;

        if (std::abs(m_objects[2]->position.z - m_objects[1]->position.z) <= 0.1f)
        {
            auto dX = m_objects[2]->position.x - m_objects[1]->position.x;

            // hit left side
            if (dX >= -0.2f
                && dX <= -0.05f)
            {
                m_ballSpeed.x = -0.7f;
                m_ballSpeed.z *= -1;
            }
            else
            // hit right side
            if (dX <= 0.2f
                && dX >= 0.05f)
            {
                m_ballSpeed.x = 0.7f;
                m_ballSpeed.z *= -1;
            }
            else
            // hit center
            if (dX < 0.05f
                && dX > -0.05f)
            {
                m_ballSpeed.x = 0.f;
                m_ballSpeed.z *= -1;
            }

            std::cout << "dt = " << dX << "\n";
            
        }

        if (std::abs(m_objects[2]->position.z - m_objects[0]->position.z) <= 0.1f)
        {
            m_ballSpeed.x = 0;
            m_ballSpeed *= -1;
        }

        // draw my idiots
        for (auto& obj : m_objects)
        {
            m_view->drawObject(obj.second->position, (int)obj.second->type);
        }

        m_timer.tic();
    }

    void _on_key_pressed(const CommonEvents::KeyPressed& evt)
    {
        glm::vec2 dir(0, 0);

        switch (evt.key)
        {
        case Key::ArrowRight: dir.x = +0.02f; break;
        case Key::ArrowLeft:  dir.x = -0.02f; break;
        case Key::ArrowUp:    dir.y = +0.02f; break;
        case Key::ArrowDown:  dir.y = -0.02f; break;
        }

        m_objects[1]->position += glm::vec3(dir.x, 0, dir.y);
    }

    void _on_mouse_moved(const CommonEvents::MouseMoved& evt)
    {

    }

    GameObject* createBall(const glm::vec3& pos)
    {
        GameObject* obj = new GameObject();
        obj->id = gs_id++;
        obj->position = pos;
        obj->type = ObjectType::Sphere;

        return obj;
    }

    GameObject* createBox(const glm::vec3& pos)
    {
        GameObject* obj = new GameObject();
        obj->id = gs_id++;
        obj->position = pos;
        obj->type = ObjectType::Box;

        return obj;
    }

    Timer::Chronometre m_timer;
    std::unique_ptr<Window> m_window;
    std::shared_ptr<View> m_view;

    glm::vec3 m_ballSpeed;

    std::unordered_map<uint64_t, GameObject*> m_objects;
};

// -- Entry point --
int main() {
    // Setup
    Game game;  
    game.CreateScene();
    // Loop
    do {
        // Inputs
        for (auto key : game.window()->keyPressed()) {
            switch (key) 
            {
            case GLFW_KEY_ESCAPE:
                game.window()->close();
                break;

            default:
                Event::Emit(CommonEvents::KeyPressed(key));
                break;
            }
        }

        Event::Emit(CommonEvents::MouseMoved(
            (int)game.window()->mousePos().x, (int)game.window()->mousePos().y
        ));

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (game.window()->update());

    // Cleanup
    return 0;
}