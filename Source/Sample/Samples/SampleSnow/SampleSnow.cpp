#include "SampleSnow.hpp"

#include <random>
#include <Engine/Graphic/ShaderManager.hpp>
#include <Engine/Graphic/Shaders/_ShaderHelper.hpp>
#include <Engine/Graphic/Base/Model/MeshIterator.hpp>
#include <Engine/Graphic/Base/Model/Primitive/PrimitiveHelper.hpp>

static constexpr int Id_model_flake  = 0;
static constexpr int Id_shader_flake = 0;

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
    Model::Ref model_flake = Model::Create(Id_model_flake);
    m_flake_local = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    std::shared_ptr<Mesh> meshTriangle = std::make_shared<Mesh>();
    PrimitiveHelper::createTriangle(*meshTriangle, {
        glm::vec3(-0.2f, 0.0f, +0.1f),
        glm::vec3(0,     0.0f, -0.1f),
        glm::vec3(+0.2f, 0.0f, +0.1f)
    });
    model_flake->addMesh(meshTriangle, model_flake->root);

    // Create custom shader
    _add_flake_shader();

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
    _subscribe([=](const SceneEvents::PostDraw&) { 
        m_ui.show(); 
    });

    // Go
    m_timer.tic();
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
    auto& flake = _create_entity("flake", Model::Load(Id_model_flake));
    flake.local() = m_flake_local;
    flake.world() = glm::translate(Identity, Spread * glm::vec3(dstr(gen), dstr(gen), 1.f));
    flake.material().color = glm::vec4(0.7f, 1.0f, 0.9f, 0.7f);
    flake.material().cast_shadow = false;
    flake.draw().type = (DrawComponent::DrawType) (DrawComponent::Custom | (Id_shader_flake << DrawComponent::Custom));

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

    if (Collider::CheckHitboxes(Model::Load(Id_model_flake), flake_pose, socle.model(), socle_pose).has_value())
        return true;

    if (!accurate)
        return Collider::CheckHitboxes(Model::Load(Id_model_flake), flake_pose, tree.model(), tree_pose).has_value();

    return Collider::CheckAccurate(Model::Load(Id_model_flake), flake_pose,  tree.model(), tree_pose).has_value();
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

// Shader
void SampleSnow::_add_flake_shader()
{
    // Already created
    if (ShaderManager::Has((CookType::_tag_)Id_shader_flake))
        return;

    // Maybe these methods shall be public
    struct Initializator : public ShaderHelper {
        static ShaderSource vertex()   { return _init_vertex();   }
        static ShaderSource geometry() { return _init_geometry(); }
        static ShaderSource fragment() { return _init_fragment(); }
    };

    // Create the shader
    std::shared_ptr<Shader> shader = std::make_shared<Shader>();

    (*shader)
        .attachSource(Shader::Vertex,
            Initializator::vertex()

            .add_var("uniform", "vec4", "override_color")
            .add_var("uniform", "vec4", "diffuse_color")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color   = length(override_color) == 0 ? max(aColor, diffuse_color) : override_color;
                gl_Position    = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        )
        .attachSource(Shader::Geometry,
            Initializator::geometry()

            .add_var("in", "layout", "(triangles)")
            .add_var("out", "layout", "(line_strip, max_vertices = 6)")
            .add_var("out", "vec4", "Color")

            .add_func("void", "main", "", R"_main_(
                for(int i = 0; i < 3; i++) 
                {
                    Color = gs_in[i].Color;

                    gl_Position = gl_in[(i+0)%3].gl_Position; EmitVertex();
                    gl_Position = gl_in[(i+1)%3].gl_Position; EmitVertex(); 

                    EndPrimitive();
                }
            )_main_")
        )
        .attachSource(Shader::Fragment, 
            ShaderSource{}

            .add_var("in", "vec4", "Color")
            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = Color;
            )_main_")
        )
    ;

    // Callback to set uniforms
    ShaderManager::UniformSetter setter = [&]() {
        std::cerr << "plop" << std::endl;
    };

    // Populate
    ShaderManager::Provide((CookType::_tag_)Id_shader_flake, shader, setter);
}
