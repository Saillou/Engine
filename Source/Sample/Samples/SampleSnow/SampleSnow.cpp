#include "SampleSnow.hpp"

#include <Engine/Graphic/Base/Model/MeshIterator.hpp>
#include <Engine/Graphic/Base/Model/Primitive/PrimitiveHelper.hpp>
#include <random>

// Particles
SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights = { };

    m_scene.camera.up        = glm::vec3(0, 0, 1);
    m_scene.camera.position  = glm::vec3(0.0f, m_cam_distance, 1.5f);
    m_scene.camera.direction = glm::vec3(0, 0, 0);

    // Create custom model (just one triangle)
    m_model_flake = Model::Create(0);
    m_flake_local = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    std::shared_ptr<Mesh> meshTriangle = std::make_shared<Mesh>();
    PrimitiveHelper::createTriangle(*meshTriangle, {
        glm::vec3(-0.2f, 0.0f, +0.1f),
        glm::vec3(0,     0.0f, -0.1f),
        glm::vec3(+0.2f, 0.0f, +0.1f)
    });
    m_model_flake->addMesh(meshTriangle, m_model_flake->root);

    // Entities
    auto& socle = _create_entity("socle", Model::Load(Model::SimpleShape::Cube));
    socle.local() = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.05f));
    socle.color() = glm::vec4(0.8f, 0.9f, 1.0f, 0.7f);

    auto& tree = _create_entity("tree", Model::Load("Resources/objects/tree/tree.glb"));
    tree.local() = glm::translate(tree.local(), glm::vec3(0, 0.50f, +0.20f));
    tree.local() = glm::scale(tree.local(), glm::vec3(2.0f));
    tree.local() = glm::rotate(tree.local(), glm::half_pi<float>(), glm::vec3(1, 0, 0));

    // Enable events
    _subscribe(&SampleSnow::_update);
    _subscribe(&SampleSnow::_on_key_pressed);
    _subscribe([=](const SceneEvents::RenderFinished&) { 
        m_ui.show(); 
    });

    // Go
    m_timer.tic();
}

SampleSnow::~SampleSnow()
{    
}

// Events
void SampleSnow::_update(const CommonEvents::StateUpdated&)
{
    float dt_s = m_timer.elapsed<Timer::millisecond>()/1000.0f;

    _generate_flakes();
    _compute_physics(dt_s);

    m_timer.tic();
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

    m_scene.camera.position.x = m_cam_distance * sin(m_cam_theta);
    m_scene.camera.position.y = m_cam_distance * cos(m_cam_theta);
}

// Entities management
ManagedEntity& SampleSnow::_create_entity(const std::string& category, Model::Ref model) {
    m_entities[category].push_back(
        ManagedEntity::Create(model)
    );
    return *m_entities[category].back();
}

void SampleSnow::_destroy_entity(const std::string& category, Entity entity)
{
    std::vector<SharedEntity>& entities = m_entities[category];
    entities.erase(std::remove_if(entities.begin(), entities.end(), [entity](SharedEntity se) {
        return se->entity() == entity;
    }), entities.end());
}

// Methods
void SampleSnow::_generate_flakes()
{
    static const glm::mat4 Identity(1.0f);

    static std::default_random_engine gen;
    static std::uniform_real_distribution<float> dstr(-0.5f, 0.5f);

    static constexpr float Spread = 1.2f;
    static size_t id_flake = 0;

    if (!m_ui.loop) {
        if (m_flakes.size() >= (size_t)m_ui.flakes_number) {
            return;
        }
    }

    // Generate one
    auto& flake = _create_entity("flake", m_model_flake);
    flake.local() = m_flake_local;
    flake.world() = glm::translate(Identity, Spread * glm::vec3(dstr(gen), dstr(gen), 1.f));
    flake.material().color = glm::vec4(0.7f, 1.0f, 0.9f, 0.7f);
    flake.material().cast_shadow = false;

    m_flakes.push_back(_Flake { 
        /* .id       = */ flake.entity(),
        /* .pose     = */ flake.world(),
        /* .accurate = */ false,
        /* .moving   = */ true
    });

    // Limits
    while (m_flakes.size() > (size_t)m_ui.flakes_number) {
        _destroy_entity("flake", m_flakes.front().id);
        m_flakes.pop_front();
    }
}

void SampleSnow::_compute_physics(float delta_time_seconds) 
{
    static const glm::vec3 Gravity = glm::vec3(0, 0, -0.5f);

    for (_Flake& flake : m_flakes) 
    {
        if (!flake.moving)
            continue;

        if (!_is_flake_colliding(flake.pose * m_flake_local, flake.accurate)) {
            flake.pose = glm::translate(flake.pose, delta_time_seconds * Gravity);
        }
        else {
            if (flake.accurate)
                flake.moving = false;
            else
                flake.accurate = true;
        }
    }
}

bool SampleSnow::_is_flake_colliding(const glm::mat4& flake_pose, bool accurate) const
{
    const ManagedEntity& socle = *m_entities.at("socle").front();
    const ManagedEntity& tree  = *m_entities.at("tree").front();

    const glm::mat4& socle_pose = socle.transform();
    const glm::mat4& tree_pose  = tree.transform();

    if (Collider::CheckHitboxes(m_model_flake, flake_pose, socle.model(), socle_pose).has_value()) 
        return true;

    if (!accurate)
        return Collider::CheckHitboxes(m_model_flake, flake_pose, tree.model(), tree_pose).has_value();

    return Collider::CheckAccurate(m_model_flake, flake_pose,  tree.model(), tree_pose).has_value();
}

// UI
void SampleSnow::Ui::show() {
#ifdef _DEBUG
    #define MAX_FLAKES 500
#else
    #define MAX_FLAKES 2500
#endif

    ImGui::Begin("Actions");
    ImGui::Separator();

    ImGui::Checkbox("Loop", &loop);
    ImGui::SliderInt("Flakes", &flakes_number, 1, MAX_FLAKES);

    ImGui::End();
}
