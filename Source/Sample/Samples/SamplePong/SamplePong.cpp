#include "SamplePong.hpp"

#include <Engine/Utils/Collider.hpp>

// Particles
SamplePong::SamplePong() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = { {glm::vec3(-1.0f, -2.0f, 2.50f), glm::vec4(1.0f) } };

    // Camera
    m_scene.camera().position  = glm::vec3(0, m_distance, -6.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Game elements
    m_player_human.pos = glm::vec3(0.0f, 0.0f, -2.0f);
    m_player_ia.pos    = glm::vec3(0.0f, 0.0f, +2.0f);

    m_ball.pos         = glm::vec3(0.0f, 0.0f, +1.0f);
    m_ball.speed       = glm::vec3(0.0f, 0.0f, -0.0015f);

    // Create entities shape
    m_entities["field"] = Entity(Entity::SimpleShape::Quad);
    {
        m_entities["field"].localMaterial().diffuse_color = glm::vec4(1, 1, 1, 0.2f);
        m_entities["field"].localMaterial().cast_shadow = false;

        glm::mat4& pose = m_entities["field"].localPose();
        pose = glm::translate(pose, glm::vec3(0, 0.1f, 0));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));
    }

    m_entities["wall"] = Entity(Entity::SimpleShape::Quad);
    {
        m_entities["wall"].localMaterial().diffuse_color = glm::vec4(1, 1, 1, 0.2f);
        m_entities["wall"].localMaterial().cast_shadow = false;

        glm::mat4& pose = m_entities["wall"].localPose();
        pose = glm::translate(pose, glm::vec3(0, -0.1f, 0));
        pose = glm::scale(pose, glm::vec3(1.0f, 0.2f, 2.0f));
        pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0));
    }

    m_entities[_Player::Entity_Name] = Entity(Entity::SimpleShape::Cube);
    {
        m_entities[_Player::Entity_Name].localMaterial().diffuse_color = glm::vec4(1.0f);
        m_entities[_Player::Entity_Name].localPose() = { glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f)) };
    }

    m_entities[_Ball::Entity_Name] = Entity(Entity::SimpleShape::Sphere);
    {
        m_entities[_Ball::Entity_Name].localMaterial().diffuse_color = glm::vec4(1.0f);
        m_entities[_Ball::Entity_Name].localPose() = { glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)) };
    }

    // Populate entities
    m_entities["field"].poses() = {
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    };

    m_entities["wall"].poses() = {
        glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)),
        glm::translate(glm::mat4(1.0f), glm::vec3(+2.0f, 0.0f, 0.0f))
    };

    m_entities[_Player::Entity_Name].poses() = {
        glm::translate(glm::mat4(1.0f), m_player_human.pos),
        glm::translate(glm::mat4(1.0f), m_player_ia.pos),
    };

    m_entities[_Ball::Entity_Name].poses() = {
        glm::translate(glm::mat4(1.0f), m_ball.pos)
    };

    // Enable events
    _subscribe(&SamplePong::_update);
    _subscribe(&SamplePong::_on_key_pressed);

    // Go
    m_timer.tic();
}

// Methods
void SamplePong::_physics(float dt_ms) {
    // Integrate pos
    glm::vec3 new_ball_pos = m_ball.pos + m_ball.speed * dt_ms;

    // Check collisions ball / players
    std::optional<glm::vec3> collision_point = {};

    for (const glm::vec3& pos : { m_player_human.pos, m_player_ia.pos }) 
    {
        const glm::mat4 player_quat = glm::translate(glm::mat4(1.0f), m_player_human.pos);
        const glm::mat4 ball_quat   = glm::translate(glm::mat4(1.0f), new_ball_pos);

        collision_point = Collider::Check(
            m_entities[_Player::Entity_Name], player_quat,
            m_entities[_Ball::Entity_Name],   ball_quat
        );

        if (collision_point.has_value())
            break; // we expect only one collision
    }

    // Solve
    if (collision_point.has_value()) {
        // ...
        new_ball_pos = m_ball.pos - m_ball.speed * dt_ms;
    }

    // Apply
    m_ball.pos = new_ball_pos;
}

void SamplePong::_refresh_position() {
    m_entities[_Player::Entity_Name].poses() = {
        glm::translate(glm::mat4(1.0f), m_player_human.pos),
        glm::translate(glm::mat4(1.0f), m_player_ia.pos),
    };

    m_entities[_Ball::Entity_Name].poses() = {
        glm::translate(glm::mat4(1.0f), m_ball.pos)
    };
}

void SamplePong::_draw_debug() {
    // Create debug box
    if (m_entities.find("debug") == m_entities.cend()) {
        m_entities["debug"] = Entity(Entity::SimpleShape::Cube);
        m_entities["debug"].localMaterial() = Material{ glm::vec4(1, 0, 0, 1), false };
    }

    // Get first mesh's obb of an entity
    auto __get_hitbox = [=](const std::string& entity_name, const glm::vec3& pos) {
        return
            glm::translate(glm::mat4(1.0f), pos) *
            glm::mat4(m_entities[entity_name].model().localPose()) *
            m_entities[entity_name].model().root()->transform *
            m_entities[entity_name].model().root()->meshes.front()->obb();
    };

    // Draw hitboxes
    m_entities["debug"].poses() =
    {
        __get_hitbox(_Player::Entity_Name, m_player_ia.pos),
        __get_hitbox(_Player::Entity_Name, m_player_human.pos),
        __get_hitbox(_Ball::Entity_Name, m_ball.pos)
    };

    m_scene.renderer().draw(Render::Geometry, m_entities["debug"]);
}

// Events
void SamplePong::_update(const SceneEvents::Draw&)
{
    _physics(m_timer.elapsed<Timer::microsecond>()/1000.0f);
    _refresh_position();

    for (auto& entity : m_entities) {
        if (entity.first == "debug")
            continue;

        m_scene.renderer().draw(Render::Shadows, entity.second);
    }
    _draw_debug();

    m_timer.tic();
}

void SamplePong::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    // - Move player -
    {
        switch (evt.key) {
            case KeyCode::ArrowLeft:  m_player_human.pos.x -= _Player::MaxSpeed; break;
            case KeyCode::ArrowRight: m_player_human.pos.x += _Player::MaxSpeed; break;
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
            m_theta    += 0.01f * dir.x;
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