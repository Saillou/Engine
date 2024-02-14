#include "SampleSnow.hpp"

#include <Engine/Utils/Physic/Collider.hpp>
#include <random>

// Particles
SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = {
        Light(glm::vec3{ 0.2f,  -0.5f, 4.0f }, glm::vec4{ 0.0f, 0.85f, 1.0f, 1 }) 
    };

    m_scene.camera().up        = glm::vec3(0, 0, 1);
    m_scene.camera().position  = glm::vec3(0.0f, m_cam_distance, 3.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Models
    m_models["socle"] = Model::Create(Model::Cube);
    {
        m_models["socle"]->localPose = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.05f));
        m_models["socle"]->localMaterial = Material{ glm::vec4(0.8f, 0.9f, 1.0f, 0.7f), false };
    }
    
    m_models["tree"] = Model::Create("Resources/objects/tree/tree.glb");
    {
        glm::mat4& pose = m_models["tree"]->localPose;
        pose = glm::translate(pose, glm::vec3(0, 0.50f, +0.20f));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    }

    m_models["flake"] = Model::Create(Model::Sphere);
    {
        m_models["flake"]->localPose = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));
        m_models["flake"]->localMaterial = Material{ glm::vec4(0.7f, 1.0f, 0.9f, 0.5f), false };
    }

    // Populate
    m_models["socle"]->poses = { glm::mat4(1.0f) };
    m_models["tree"]->poses  = { glm::mat4(1.0f) };

    // Enable events
    _subscribe(&SampleSnow::_update);
    _subscribe(&SampleSnow::_draw);
    _subscribe(&SampleSnow::_on_key_pressed);

    // Go
    m_timer.tic();
}

// Events
void SampleSnow::_update(const CommonEvents::StateUpdated&)
{
    float dt_s = m_timer.elapsed<Timer::millisecond>()/1000.0f;

    _generate_flakes();
    _compute_physics(dt_s);
    _update_models();

    m_timer.tic();
}

void SampleSnow::_draw(const SceneEvents::Draw&) 
{
    //m_scene.renderer().draw(Render::DrawType::Shadows,  m_models["socle"]);
    //m_scene.renderer().draw(Render::DrawType::Lights,   m_models["tree"]);
    m_scene.renderer().draw(Render::DrawType::Particle, m_models["flake"]);

    _draw_hitbox("socle");
    _draw_hitbox("tree");
}

void SampleSnow::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    if (evt.action == InputAction::Released)
        return;

    switch (evt.key)
    {
    case KeyCode::ArrowRight: m_cam_theta += 0.01f; break;
    case KeyCode::ArrowLeft:  m_cam_theta -= 0.01f; break;
    case KeyCode::ArrowUp:    m_cam_distance -= 0.05f; break;
    case KeyCode::ArrowDown:  m_cam_distance += 0.05f; break;
    default:
        return;
    }

    m_scene.camera().position.x = m_cam_distance * sin(m_cam_theta);
    m_scene.camera().position.y = m_cam_distance * cos(m_cam_theta);
}

// Methods
void SampleSnow::_generate_flakes() 
{
    static const glm::mat4 Identity(1.0f);

    static constexpr size_t Max_Flakes = 500;
    static constexpr float Spread      = 2.0f;

    static std::default_random_engine gen;
    static std::uniform_real_distribution<float> dstr(-0.5f, 0.5f);

    static size_t id_flake = 0;

    if (!m_flakes.empty())
        return;

    // Generate one
    m_flakes.push_back(_Flake { 
        /* .pose   = */ glm::translate(Identity, Spread * glm::vec3(0*dstr(gen), 0*dstr(gen), 1.0f)),
        /* .id     = */ (id_flake + 1) % Max_Flakes,
        /* .moving = */ true 
    });

    // Limits
    while (m_flakes.size() >= Max_Flakes) {
        m_flakes.pop_front();
    }
}

void SampleSnow::_compute_physics(float delta_time_seconds) 
{
    static const glm::vec3 Gravity = glm::vec3(0, 0, -0.5f);

    for (_Flake& flake : m_flakes) {
        if (!flake.moving)
            continue;

        if (!_is_flake_colliding(flake.pose)) {
            flake.pose = glm::translate(flake.pose, delta_time_seconds * Gravity);
        }
        else {
            flake.moving = false;
        }
    }
}

void SampleSnow::_update_models() 
{
    m_models["flake"]->poses.clear();
    m_models["flake"]->poses.reserve(m_flakes.size());

    for (const _Flake& flake : m_flakes) {
        m_models["flake"]->poses.push_back(flake.pose);
    }
}

bool SampleSnow::_is_flake_colliding(const glm::mat4& flake_pose) const
{
    const glm::mat4& socle_pose = m_models.at("socle")->poses.front();
    const glm::mat4& tree_pose  = m_models.at("tree")->poses.front();

    return
        Collider::Check(
            m_models.at("flake"), flake_pose, 
            m_models.at("socle"), socle_pose
        ).has_value() 
        ||
        Collider::Check(
            m_models.at("flake"), flake_pose, 
            m_models.at("tree"),  tree_pose
        ).has_value()
     ;
}

// -- debug --
void SampleSnow::_draw_hitbox(const std::string& model_name) 
{
    // Check anti-stupidity
    if (m_models.find(model_name) == m_models.cend())
        return;

    const Model::Ref& model = m_models[model_name];
    if (!model->root || model->poses.empty())
        return;

    // Create models
    // - hitbox
    const std::string debug_box_name = "debug_box_" + model_name;

    if (m_models.find(debug_box_name) == m_models.cend()) {
        m_models[debug_box_name] = Model::Create(Model::Cube);
        m_models[debug_box_name]->localMaterial = Material{ glm::vec4(1, 1, 0, 1), false };
    }

    // - model
    const std::string debug_name = "debug_" + model_name;

    if (m_models.find(debug_name) == m_models.cend()) {
        m_models[debug_name] = model->Clone();
        m_models[debug_name]->localMaterial = Material{ glm::vec4(1, 0, 0, 1), false };
    }

    // Get all meshes' obb poses
    const glm::mat4& worldPose = model->poses.front();

    m_models[debug_name]->poses.clear();
    m_models[debug_box_name]->poses.clear();
    for (const glm::mat4& localPose : model->GetMeshesPoses()) {
        m_models[debug_name]->poses.push_back(worldPose * localPose);
        m_models[debug_box_name]->poses.push_back(worldPose * localPose);
    }

    // Draw
    m_scene.renderer().draw(Render::Geometry, m_models[debug_name]);
    m_scene.renderer().draw(Render::Geometry, m_models[debug_box_name]);
}
