#include "SamplePong.hpp"

#include <Engine/Utils/Physic/Collider.hpp>

SamplePong::SamplePong() :
    m_scene(Service<Window>::get().scene())
{
    _init_game_elements();
    _create_entities();

    // Enable events
    _subscribe(&SamplePong::_update);
    _subscribe(&SamplePong::_on_key_pressed);

    // Go
    m_timer.tic();
}

void SamplePong::_init_game_elements() {
    // Scene
    m_scene.lights() = { /*{glm::vec3(-1.0f, -2.0f, 2.50f), glm::vec4(1.0f) }*/ };

    // Camera
    m_scene.camera().position = glm::vec3(0, m_distance, -6.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Game elements
    m_player_human.pos = glm::vec3(0.0f, 0.0f, -2.0f);
    m_player_ia.pos = glm::vec3(0.0f, 0.0f, +2.0f);

    m_ball.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_ball.speed = glm::vec3(+0.003f, 0.0f, -0.001f);

    m_wall_1.pos = glm::vec3(-2.0f, 0.0f, 0.0f);
    m_wall_2.pos = glm::vec3(+2.0f, 0.0f, 0.0f);
}

void SamplePong::_create_entities() {
    // Create entities shape
    m_models["field"] = Model::Create(Model::Quad);
    {
        m_models["field"]->localMaterial = glm::vec4(1, 1, 1, 0.2f);
        //m_models["field"]->localMaterial.cast_shadow = false;

        glm::mat4& pose(m_models["field"]->localTransform);
        pose = glm::translate(pose, glm::vec3(0, 0.1f, 0));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    }

    m_models[_Wall::Entity_Name] = Model::Create(Model::Quad);
    {
        m_models[_Wall::Entity_Name]->localMaterial = glm::vec4(1, 1, 1, 1);
        //m_models[_Wall::Entity_Name]->localMaterial.cast_shadow = false;

        glm::mat4& pose(m_models[_Wall::Entity_Name]->localTransform);
        pose = glm::translate(pose, glm::vec3(0, -0.1f, 0));
        pose = glm::scale(pose, glm::vec3(1.0f, 0.2f, 2.0f));
        pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0));
    }

    m_models[_Player::Entity_Name] = Model::Create(Model::Cube);
    {
        m_models[_Player::Entity_Name]->localMaterial = glm::vec4(1.0f);
        m_models[_Player::Entity_Name]->localTransform = { glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f)) };
    }

    m_models[_Ball::Entity_Name] = Model::Create(Model::Sphere);
    {
        m_models[_Ball::Entity_Name]->localMaterial = glm::vec4(1.0f);
        m_models[_Ball::Entity_Name]->localTransform = { glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)) };
    }

    // Populate entities
    m_models["field"]->transforms = {
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    };

    m_models[_Wall::Entity_Name]->transforms = {
        glm::translate(glm::mat4(1.0f), m_wall_1.pos),
        glm::translate(glm::mat4(1.0f), m_wall_2.pos)
    };

    _update_entities();
}

void SamplePong::_ia() {
    if (m_player_ia.pos.x < m_ball.pos.x - 0.1f)
        m_player_ia.next_action = _Player::Action::Right;

    if (m_player_ia.pos.x > m_ball.pos.x + 0.1f)
        m_player_ia.next_action = _Player::Action::Left;
}

void SamplePong::_physics(float dt_ms) {
    // Integrate pos
    glm::vec3 new_ball_pos = m_ball.pos + m_ball.speed * dt_ms;

    // Define potential colliders with the ball
    struct _collide_body_ {
        const std::string& name;
        const glm::vec3& pos;
    };
    std::vector<_collide_body_> colliders =
    {
        { _Player::Entity_Name, m_player_human.pos},
        { _Player::Entity_Name, m_player_ia.pos},
        { _Wall::Entity_Name,   m_wall_1.pos},
        { _Wall::Entity_Name,   m_wall_2.pos}
    };

    struct _collision_ {
        glm::vec3 contact_point;
        const _collide_body_& collider;
    };
    std::optional<_collision_> collision = {};

    // Check
    for (const auto& collider : colliders)
    {
        const glm::mat4 body_quat = glm::translate(glm::mat4(1.0f), collider.pos);
        const glm::mat4 ball_quat = glm::translate(glm::mat4(1.0f), new_ball_pos);

        auto collision_point = Collider::CheckHitboxes(
            m_models[collider.name], body_quat,
            m_models[_Ball::Entity_Name], ball_quat
        );

        if (!collision_point.has_value())
            continue;

        collision.emplace(_collision_{ collision_point.value(), collider });
        break; // we expect only one collision
    }

    // Solve
    if (collision.has_value())
    {
        if (collision.value().collider.name == _Player::Entity_Name) {
            m_ball.speed = glm::length(m_ball.speed) * glm::normalize(new_ball_pos - collision.value().collider.pos);
        }
        else {
            m_ball.speed.x *= -1.0f;
        }
        new_ball_pos = m_ball.pos + m_ball.speed * dt_ms;
    }

    // Apply
    if (m_ui.stop_time)
        return;

    m_ball.pos = new_ball_pos;
}

void SamplePong::_update_entities() {
    m_models[_Player::Entity_Name]->transforms = {
        glm::translate(glm::mat4(1.0f), m_player_human.pos),
        glm::translate(glm::mat4(1.0f), m_player_ia.pos),
    };

    m_models[_Ball::Entity_Name]->transforms = {
        glm::translate(glm::mat4(1.0f), m_ball.pos)
    };
}

void SamplePong::_draw() {
    for (auto& entity : m_models) {
        if (entity.first.find("debug") != std::string::npos)
            continue;

        m_scene.renderer().draw(Render::Basic, entity.second);
    }

    if (m_ui.show_debug) {
        _draw_hitbox();
    }

    m_ui.show();
}

void SamplePong::_draw_hitbox() {
    // Create debug box
    if (m_models.find("debug_cube") == m_models.cend()) {
        m_models["debug_cube"] = Model::Create(Model::Cube);
        m_models["debug_cube"]->localMaterial = glm::vec4(1, 0, 0, 1);
        //m_models["debug_cube"]->localMaterial = Material{ glm::vec4(1, 0, 0, 1), false };
    }

    // Get first mesh's obb of an entity
    auto __get_hitbox = [=](const std::string& entity_name, const glm::vec3& pos) {
        return
            glm::translate(glm::mat4(1.0f), pos) *
            m_models[entity_name]->localTransform *
            m_models[entity_name]->root->transform *
            m_models[entity_name]->root->meshes.front()->obb();
        };

    // Draw hitboxes
    m_models["debug_cube"]->transforms =
    {
        __get_hitbox(_Player::Entity_Name, m_player_ia.pos),
        __get_hitbox(_Player::Entity_Name, m_player_human.pos),
        __get_hitbox(_Wall::Entity_Name, m_wall_1.pos),
        __get_hitbox(_Wall::Entity_Name, m_wall_2.pos),
        __get_hitbox(_Ball::Entity_Name, m_ball.pos)
    };

    m_scene.renderer().draw(Render::Geometry, m_models["debug_cube"]);
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
void SamplePong::_update(const SceneEvents::Draw&)
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

    _draw();

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

            scene.camera().position.x = m_distance * sin(m_theta);
            scene.camera().position.y = m_distance * cos(m_theta);
            scene.camera().position.z += 0.05f * dir.z;
        }
    }

    // - State -
    {
        switch (evt.key) {
        case KeyCode::Escape: wantQuit = true;
        }
    }
}