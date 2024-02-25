#include "SampleBreaker.hpp"

#include <algorithm>

SampleBreaker::SampleBreaker() :
    m_scene(Service<Window>::get().scene())
{
    _init_scene();

    // Enable events
    _subscribe(&SampleBreaker::_on_key_pressed);
    _subscribe(&SampleBreaker::_update);

    m_timer.tic();
}

void SampleBreaker::_init_scene()
{
    // Scene
    m_scene.lights = { 
        { glm::vec3(0.0f, 0.0f, +0.0f), glm::vec4(1.0f, 0.7f, 0.6f, 1.0f) },
        { glm::vec3(0.0f, 1.0f, +2.0f), glm::vec4(1.0f, 0.7f, 0.0f, 1.0f) },
        { glm::vec3(0.0f, 1.0f, -2.0f), glm::vec4(0.0f, 0.7f, 1.0f, 1.0f) },
    };

    // Camera
    m_scene.camera.position  = glm::vec3(0, 10.0f, -3.0f);
    m_scene.camera.direction = glm::vec3(0, 0, 0);

    // Game elements
    m_ball.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
    m_player.setPos(glm::vec3(0.0f, 0.0f, -2.0f));
    m_wall_left.setPos(glm::vec3(-2.0f, 0.0f, 0.0f));
    m_wall_right.setPos(glm::vec3(+2.0f, 0.0f, 0.0f));

    m_ball.speed = glm::vec3(+1.0f, 0.0f, -2.0f) * 0.001f;

    // Bricks
    const int n_rows = 8;
    const int n_cols = 5;

    for (int i_rows = 0; i_rows < n_rows; i_rows++) {
        for (int i_cols = 0; i_cols < n_cols; i_cols++) {
            glm::vec3 pos(0.0f);
            pos.x = (i_rows - n_rows / 2.0f) * 0.5f + 0.25f;
            pos.z = (i_cols - n_cols / 2.0f) * 0.15f + 1.5f;

            Brick brick(pos);
            m_bricks.emplace(brick.id(), std::move(brick));
        }
    }
    
}

// - Methods -
void SampleBreaker::_physics(float dt_ms) 
{
    // Integrate speed without hindrances
    glm::vec3 new_ball_pos = m_ball.pos() + m_ball.speed * dt_ms;

    // Try to move the ball and check collision
    m_ball.setPos(new_ball_pos);
    m_scene.collider().check(m_ball.id());

    // Nothing to change
    if (!m_ball.entity().is_colliding())
        return;

    // Solve constraints
    const auto& collide = ECS::getComponent<CollideComponent>(m_ball.id());

    // Assuming only one interaction per frame
    Entity hidrance(collide.hit_entities.front());

    // Change speed direction
    if (hidrance == m_wall_left.id() || hidrance == m_wall_right.id()) // simple bounce on wall
    {
        m_ball.speed.x *= -1.0f;
    }
    else
    {
        const BodyComponent& body(ECS::getComponent<BodyComponent>(hidrance));

        glm::vec3 hindrance_pos = glm::vec3(body.transform.world[3]);
        m_ball.speed = glm::length(m_ball.speed) * glm::normalize(m_ball.pos() - hindrance_pos);

        // If not wall, not player, i'll assume it's a brick
        if (hidrance != m_player.id())
            _destroy_brick(hidrance);
    }

    // New position because of hindrances
    m_ball.setPos(m_ball.pos() + m_ball.speed * dt_ms);
}

void SampleBreaker::_destroy_brick(Entity entity) 
{
    if (m_bricks.find(entity) == m_bricks.cend())
        return;

    m_bricks.erase(entity);
}


// - Events -
void SampleBreaker::_update(const CommonEvents::StateUpdated&)
{
    m_player.move();

    _physics(m_timer.elapsed<Timer::microsecond>() / 1000.0f);

    m_scene.lights.front().position = m_ball.pos() + glm::vec3(0, 0.2f, 0);
    m_timer.tic();
}

void SampleBreaker::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    // - Move player -
    {
        switch (evt.key) {
            case KeyCode::ArrowLeft:  m_player.next_action = Player::Action::Left; break;
            case KeyCode::ArrowRight: m_player.next_action = Player::Action::Right; break;
        }
    }

    // - State -
    {
        switch (evt.key) {
            case KeyCode::Escape: wantQuit = true;
        }
    }
}