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

SamplePong::~SamplePong()
{
    for (auto v_entity : m_entities) {
        for (auto entity : v_entity.second) {
            ECS::destroyEntity(entity);
        }
    }
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
    {
        DrawComponent draw;
        draw.type = DrawComponent::Shadows;

        BodyComponent body;
        body.model     = Model::Load(Model::SimpleShape::Quad)->Clone();
        body.material  = glm::vec4(1, 1, 1, 0.2f);

        glm::mat4& pose(body.transform);
        pose = glm::translate(pose, glm::vec3(0, 0.1f, 0));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

        m_entities["field"].push_back(ECS::createEntity());
        ECS::addComponent(m_entities["field"].back(), body);
        ECS::addComponent(m_entities["field"].back(), draw);
    }

    // ---- Walls -----
    for (_Wall wall : {m_wall_1, m_wall_2}) 
    {
        DrawComponent draw;
        draw.type = DrawComponent::Shadows;

        BodyComponent body;
        body.model     = Model::Load(Model::SimpleShape::Quad)->Clone();;
        body.material  = glm::vec4(1, 1, 1, 0.5f);

        // Local transform
        glm::mat4& pose(body.transform);
        pose = glm::translate(pose, glm::vec3(0, -0.1f, 0));
        pose = glm::scale(pose, glm::vec3(1.0f, 0.2f, 2.0f));
        pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0));

        // World transform
        pose = glm::translate(glm::mat4(1.0f), wall.pos) * pose;

        CollideComponent collider;

        m_entities[_Wall::Entity_Name].push_back(ECS::createEntity());
        ECS::addComponent(m_entities[_Wall::Entity_Name].back(), body);
        ECS::addComponent(m_entities[_Wall::Entity_Name].back(), draw);
        ECS::addComponent(m_entities[_Wall::Entity_Name].back(), collider);
    }

    // ---- Players -----
    for (_Player player : {m_player_human, m_player_ia})
    {
        DrawComponent draw;
        draw.type = DrawComponent::Shadows;

        // Local transform
        BodyComponent body;
        body.model     = Model::Load(Model::SimpleShape::Cube)->Clone();
        body.material  = glm::vec4(1.0f);
        body.transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f));
        
        // World transform
        body.transform = glm::translate(glm::mat4(1.0f), player.pos) * body.transform;

        CollideComponent collider;

        m_entities[_Player::Entity_Name].push_back(ECS::createEntity());
        ECS::addComponent(m_entities[_Player::Entity_Name].back(), body);
        ECS::addComponent(m_entities[_Player::Entity_Name].back(), draw);
        ECS::addComponent(m_entities[_Player::Entity_Name].back(), collider);
    }

    // ---- Ball -----
    {
        DrawComponent draw;
        draw.type = DrawComponent::Shadows;

        // Local transform
        BodyComponent body;
        body.model     = Model::Load(Model::SimpleShape::Sphere)->Clone();
        body.material  = glm::vec4(1.0f);
        body.transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

        // World transform
        body.transform = glm::translate(glm::mat4(1.0f), m_ball.pos) * body.transform;

        CollideComponent collider;

        m_entities[_Ball::Entity_Name].push_back(ECS::createEntity());
        ECS::addComponent(m_entities[_Ball::Entity_Name].back(), body);
        ECS::addComponent(m_entities[_Ball::Entity_Name].back(), draw);
        ECS::addComponent(m_entities[_Ball::Entity_Name].back(), collider);
    }
}


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

    // Integrate speed without hindrances
    glm::vec3 new_ball_pos = m_ball.pos + m_ball.speed * dt_ms;

    // Try to move the ball and check collision
    ECS::getComponent<BodyComponent>(m_entities[_Ball::Entity_Name].front()).transform = glm::translate(glm::mat4(1.0f), new_ball_pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

    m_scene.collider()->update(m_entities[_Ball::Entity_Name].front());

    const CollideComponent& collide = ECS::getComponent<CollideComponent>(m_entities[_Ball::Entity_Name].front());

    // Solve
    if (collide.is_colliding)
    {
        // Assuming only one interaction
        Entity entity_colliding = collide.hit_entities.front();
        glm::vec3 pos_colliding = collide.hit_positions.front();

        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity_colliding);

        // Change speed direction
        const auto& players = m_entities[_Player::Entity_Name];
        if (std::find(players.cbegin(), players.cend(), entity_colliding) != players.cend()) 
        {
            glm::vec3 entity_pos_colliding = glm::vec3(body.transform[3]);
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
}

void SamplePong::_update_entities()
{
    // Update players
    {
        const std::vector<_Player>& players = { m_player_human, m_player_ia };
        for (size_t iPlayer = 0; iPlayer < m_entities[_Player::Entity_Name].size(); iPlayer++)
        {
            glm::mat4& transform = ECS::getComponent<BodyComponent>(m_entities[_Player::Entity_Name][iPlayer]).transform;
            transform = glm::translate(glm::mat4(1.0f), players[iPlayer].pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f));
        }
    }

    // Update ball
    {
        glm::mat4& transform = ECS::getComponent<BodyComponent>(m_entities[_Ball::Entity_Name].front()).transform;
        transform = glm::translate(glm::mat4(1.0f), m_ball.pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
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


// Events
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
    _update_entities();
    //_update_hitboxes(m_ui.show_debug);

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