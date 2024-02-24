#include "SamplePong.hpp"

#include <algorithm>

SamplePong::SamplePong() :
    m_scene(Service<Window>::get().scene())
{
    _init_game_elements();
    _create_entities();

    // Enable events
    _subscribe(&SamplePong::_on_key_pressed);
    _subscribe(&SamplePong::_update);
    _subscribe([=](const SceneEvents::RenderFinished&) { 
        m_ui.show(); 
    });

    // Go
    m_timer.tic();
}

void SamplePong::_init_game_elements() 
{
    // Scene
    m_scene.lights = { {glm::vec3(-1.0f, -2.0f, 2.50f), glm::vec4(1.0f) } };

    // Camera
    m_scene.camera.position  = glm::vec3(0, m_distance, -6.0f);
    m_scene.camera.direction = glm::vec3(0, 0, 0);

    // Game elements
    m_player_human.pos = glm::vec3(0.0f, 0.0f, -2.0f);
    m_player_ia.pos    = glm::vec3(0.0f, 0.0f, +2.0f);

    m_ball.pos   = glm::vec3(0.0f, 0.0f, 0.0f);
    m_ball.speed = glm::vec3(+0.003f, 0.0f, -0.001f);

    m_wall_1.pos = glm::vec3(-2.0f, 0.0f, 0.0f);
    m_wall_2.pos = glm::vec3(+2.0f, 0.0f, 0.0f);
}

void SamplePong::_create_entities() 
{
    // ---- Field -----
    auto& field_e = _create_entity("field", Model::Load(Model::SimpleShape::Quad));
    field_e.color() = glm::vec4(1, 1, 1, 0.2f);

    glm::mat4& local(field_e.local());
    local = glm::translate(local, glm::vec3(0, 0.1f, 0));
    local = glm::scale(local, glm::vec3(2.0f));
    local = glm::rotate(local, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    // ---- Walls -----
    for (_Wall wall : {m_wall_1, m_wall_2})
    {
        auto& wall_e = _create_entity(_Wall::Entity_Name, Model::Load(Model::SimpleShape::Quad));
        wall_e.color() = glm::vec4(1, 1, 1, 0.5f);
        wall_e.draw().type = DrawComponent::Geometry;

        glm::mat4& local(wall_e.local());
        local = glm::translate(local, glm::vec3(0, -0.1f, 0));
        local = glm::scale(local, glm::vec3(1.0f, 0.2f, 2.0f));
        local = glm::rotate(local, glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0));

        wall_e.world() = glm::translate(glm::mat4(1.0f), wall.pos);
        wall_e.collidable(true);
    }

    // ---- Players -----
    for (_Player player : {m_player_human, m_player_ia})
    {
        auto& player_e = _create_entity(_Player::Entity_Name, Model::Load(Model::SimpleShape::Cube));
        player_e.color() = player.pos == m_player_ia.pos ? glm::vec4(0.7f, 0, 0, 1.0f) : glm::vec4(0, 0, 0.7f, 1.0f);

        player_e.local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f));
        player_e.world() = glm::translate(glm::mat4(1.0f), player.pos);
        player_e.collidable(true);
    }

    // ---- Ball -----
    auto& ball_e = _create_entity(_Ball::Entity_Name, Model::Load(Model::SimpleShape::Sphere));
    ball_e.color() = glm::vec4(1.0f);
    ball_e.local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    ball_e.world() = glm::translate(glm::mat4(1.0f), m_ball.pos);
    ball_e.collidable(true);
}

ManagedEntity& SamplePong::_create_entity(const std::string& category, Model::Ref model)
{
    m_entities[category].push_back(
        ManagedEntity::Create(model)
    );
    return *m_entities[category].back();
}

// - Loop -
void SamplePong::_ia() {
    if (m_player_ia.pos.x < m_ball.pos.x - 0.1f)
        m_player_ia.next_action = _Player::Action::Right;

    if (m_player_ia.pos.x > m_ball.pos.x + 0.1f)
        m_player_ia.next_action = _Player::Action::Left;
}

void SamplePong::_physics(float dt_ms) {
    // Nope
    if (m_ui.stop_time)
        return;

    // Get ball
    auto& ball = *m_entities[_Ball::Entity_Name].front();

    // Integrate speed without hindrances
    glm::vec3 new_ball_pos = m_ball.pos + m_ball.speed * dt_ms;

    // Try to move the ball and check collision
    ball.world() = glm::translate(glm::mat4(1.0f), new_ball_pos);
    m_scene.collider()->check(ball.entity());

    // Solve
    if (ball.is_colliding())
    {
        const auto& collide = ECS::getComponent<CollideComponent>(ball.entity());

        // Assuming only one interaction
        Entity entity_colliding = collide.hit_entities.front();
        glm::vec3 pos_colliding = collide.hit_positions.front();

        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity_colliding);

        // Change speed direction
        const auto& ps = m_entities[_Player::Entity_Name];

        bool is_player = std::find_if(ps.cbegin(), ps.cend(), [=](const SharedEntity& se) {
            return se->entity() == entity_colliding; 
        }) != ps.cend();

        if (is_player)
        {
            glm::vec3 entity_pos_colliding = glm::vec3(body.transform.world[3]);
            m_ball.speed = glm::length(m_ball.speed) * glm::normalize(new_ball_pos - entity_pos_colliding);
        }
        else 
        {
            m_ball.speed.x *= -1.0f;
        }

        // New position because of hindrances
        new_ball_pos = m_ball.pos + m_ball.speed * dt_ms;
    }

    // Apply
    m_ball.pos = new_ball_pos;
    ball.world() = glm::translate(glm::mat4(1.0f), m_ball.pos);
}

void SamplePong::_update_players()
{
    const std::vector<_Player>& players = { m_player_human, m_player_ia };
    for (size_t iPlayer = 0; iPlayer < m_entities[_Player::Entity_Name].size(); iPlayer++)
    {
        auto& player = *m_entities[_Player::Entity_Name][iPlayer];
        player.world() = glm::translate(glm::mat4(1.0f), players[iPlayer].pos);
    }
}

void SamplePong::_apply_actions(_Player& player)
{
    switch (player.next_action) {
        case _Player::Action::Left:  player.pos.x -= _Player::MaxSpeed; break;
        case _Player::Action::Right: player.pos.x += _Player::MaxSpeed; break;
    }

    player.next_action = _Player::Action::None;
}


// - Events -
void SamplePong::_update(const CommonEvents::StateUpdated&)
{
    if (m_ui.want_restart) {
        _init_game_elements();
        m_ui.want_restart = false;
    }

    _ia();
    _apply_actions(m_player_ia);
    _apply_actions(m_player_human);

    _physics(m_timer.elapsed<Timer::microsecond>() / 1000.0f);
    _update_players();

    m_timer.tic();
}

void SamplePong::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    // - Move player -
    {
        switch (evt.key) {
        case KeyCode::ArrowLeft:  m_player_human.next_action = _Player::Action::Left; break;
        case KeyCode::ArrowRight: m_player_human.next_action = _Player::Action::Right; break;
        }
    }

    // - Move camera -
    {
        glm::vec3 dir(0, 0, 0);

        switch (evt.key) {
        case 'D': dir.x = -1.0f; break;
        case 'A': dir.x = +1.0f; break;

        case 'W': dir.y = -1.0f; break;
        case 'S': dir.y = +1.0f; break;

        case 'Q': dir.z = +1.0f; break;
        case 'E': dir.z = -1.0f; break;
        }

        if (dir != glm::vec3(0, 0, 0)) {
            m_theta += 0.01f * dir.x;
            m_distance += 0.05f * dir.y;

            scene.camera.position.x = m_distance * sin(m_theta);
            scene.camera.position.y = m_distance * cos(m_theta);
            scene.camera.position.z += 0.05f * dir.z;
        }
    }

    // - State -
    {
        switch (evt.key) {
        case KeyCode::Escape: wantQuit = true;
        }
    }
}