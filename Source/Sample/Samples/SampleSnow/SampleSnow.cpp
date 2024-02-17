#include "SampleSnow.hpp"

#include <Engine/Utils/Physic/Collider.hpp>
#include <Engine/Graphic/Base/Model/MeshIterator.hpp>
#include <Engine/Graphic/Base/Model/Primitive/PrimitiveHelper.hpp>
#include <random>

//#define DRAW_DEBUG_COLLISION
#ifdef DRAW_DEBUG_COLLISION

static Shader& _create_triangle_shader(Shader&);
static void _push_triangle_mesh(std::unique_ptr<Model::Node>&, const PrimitiveHelper::Triangle&);

static glm::vec3 _pos = { 0, -0.2f, 0.5f };

SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    _init_shaders();

    // Scene
    m_scene.camera().up = glm::vec3(0, 0, 1);
    m_scene.camera().position  = glm::vec3(0.0f, m_cam_distance, 2.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0.5f);

    // Enable events
    _subscribe(&SampleSnow::_update);
    _subscribe(&SampleSnow::_draw);
    _subscribe(&SampleSnow::_on_key_pressed);

    // Create debug models
    m_models["model"]                = Model::Create("Resources/objects/tree/tree.glb");
    m_models["model"]->poses         = { glm::mat4(1.0f) };
    m_models["model"]->materials     = { Material{ glm::vec4(1, 0, 0, 1), false } };
    {
        glm::mat4& pose = m_models["model"]->localPose;
        pose = glm::translate(pose, glm::vec3(0, 0.50f, +0.20f));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    }

    m_models["target"]               = Model::Create();
    m_models["target"]->poses        = { glm::mat4(1.0f) };
    m_models["target"]->materials    = { Material{ glm::vec4(0, 1, 1, 1), false } };

    m_models["point"]                = Model::Create(Model::Sphere);
    m_models["point"]->localMaterial = { Material{ glm::vec4(1, 0, 1, 1), false } };
    m_models["point"]->localPose     = { glm::scale(glm::mat4(1.0f), glm::vec3(0.03f)) };

    _push_triangle_mesh(m_models["target"]->root, {
        glm::vec3(-0.2f, 0.0f, -0.1f),
        glm::vec3(0,     0.0f, -0.3f),
        glm::vec3(+0.2f, 0.0f, -0.1f)
    });
}

// Events
void SampleSnow::_update(const CommonEvents::StateUpdated&)
{
    m_models["point"]->poses.clear();
    m_models["target"]->poses.front() = glm::translate(glm::mat4(1.0f), _pos);

    auto collision_points = Collider::CheckAccurate(
        m_models["target"], m_models["target"]->poses.front(),
        m_models["model"],  m_models["model"]->poses.front()
    );

    if (!collision_points.has_value())
        return;

    for (const glm::vec3& point : collision_points.value()) {
        m_models["point"]->poses.push_back(glm::translate(glm::mat4(1.0f), point));
    }
}

void SampleSnow::_draw(const SceneEvents::Draw&)
{
    m_scene.renderer().draw(Render::Particle, m_models["point"]);
    m_scene.renderer().draw("triangle",       m_models["model"]);
    m_scene.renderer().draw("triangle",       m_models["target"]);

    _draw_hitbox("model");
}

void SampleSnow::_on_key_pressed(const CommonEvents::KeyPressed& evt)
{
    static bool shift = false;

    if (evt.key == KeyCode::ShiftLeft) {
        shift = evt.action != InputAction::Released;
    }

    if (evt.action == InputAction::Released)
        return;

    switch (evt.key)
    {
        case KeyCode::ArrowDown:  shift ? (_pos.y -= 0.02f):(_pos.z -= 0.02f); break;
        case KeyCode::ArrowUp:    shift ? (_pos.y += 0.02f):(_pos.z += 0.02f); break;

        case KeyCode::ArrowRight: _pos.x += 0.02f; break;
        case KeyCode::ArrowLeft:  _pos.x -= 0.02f; break;

        case 'A': m_cam_theta += 0.01f; break;
        case 'D': m_cam_theta -= 0.01f; break;
        case 'W': m_cam_distance -= 0.05f; break;
        case 'S': m_cam_distance += 0.05f; break;
    }

    m_scene.camera().position.x = m_cam_distance * sin(m_cam_theta);
    m_scene.camera().position.y = m_cam_distance * cos(m_cam_theta);
}

void _push_triangle_mesh(std::unique_ptr<Model::Node>& node, const PrimitiveHelper::Triangle& triangle) 
{
    std::unique_ptr<Mesh> meshTriangle = std::make_unique<Mesh>();

    PrimitiveHelper::createTriangle(*meshTriangle, triangle);
    meshTriangle->sendToGpu();
    meshTriangle->compute_obb();

    node->meshes.push_back(std::move(meshTriangle));
}

void SampleSnow::_init_shaders() {
    // Create shader
    _create_triangle_shader(m_shaders["triangle"]).link();

    // Render callbacks
    Renderer::ShaderGetter getter = [=]() -> Shader& {
        return m_shaders["triangle"];
    };

    Renderer::ShaderSetter setter = [=](const Model::Ref& model) -> void {
        getter().use()
            .set("Projection", m_scene.camera().projection)
            .set("View", m_scene.camera().modelview)
            .set("diffuse_color", model->localMaterial.diffuse_color);
    };

    // Add to list
    m_scene.renderer().add_shader("triangle", getter, setter);
}

Shader& _create_triangle_shader(Shader& shader)
{
    shader
        .attachSource(GL_VERTEX_SHADER,
            Cookable::_init_vertex()

            .add_var("uniform", "vec4", "diffuse_color")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color   = max(aColor, diffuse_color);
                gl_Position    = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        )
        .attachSource(GL_GEOMETRY_SHADER, 
            Cookable::_init_geometry()

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
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec4", "Color")
            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = Color;
            )_main_")
        );

    return shader;
}

void SampleSnow::_draw_hitbox(const std::string& model_name)
{
    // Check anti-stupidity
    if (m_models.find(model_name) == m_models.cend())
        return;

    const Model::Ref& model = m_models[model_name];
    if (!model->root || model->poses.empty())
        return;

    // Create hitboxes model
    const std::string debug_box_name = "debug_box_" + model_name;

    if (m_models.find(debug_box_name) == m_models.cend()) {
        m_models[debug_box_name] = Model::Create(Model::Cube);
        m_models[debug_box_name]->localMaterial = Material{ glm::vec4(1, 1, 0, 1), false };
    }

    // Get all meshes' poses
    m_models[debug_box_name]->poses.clear();

    for (const glm::mat4& worldPose : model->poses) {
        MeshIterator::forEachMesh(*model, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc) {
            m_models[debug_box_name]->poses.push_back(worldPose * model->localPose * node_acc.transform * mesh->obb());
            });
    }

    // Draw
    m_scene.renderer().draw(Render::Geometry, m_models[debug_box_name]);
}

#else

// Particles
SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = {
        Light(glm::vec3{ 0.2f,  -0.5f, 1.0f }, glm::vec4{ 0.0f, 0.85f, 1.0f, 1 }) 
    };

    m_scene.camera().up        = glm::vec3(0, 0, 1);
    m_scene.camera().position  = glm::vec3(0.0f, m_cam_distance, 1.5f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Models
    m_models["socle"] = Model::Create(Model::Cube);
    {
        m_models["socle"]->localPose = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.05f));
        m_models["socle"]->localMaterial = Material{ glm::vec4(0.8f, 0.9f, 1.0f, 0.7f), false };
    }
    
    m_models["tree"] = Model::Create("Resources/objects/tree/tree.glb");
    {
        glm::mat4& pose = m_models["tree"]->localPose;
        pose = glm::translate(pose, glm::vec3(0, 0.50f, +0.20f));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    }

    m_models["flake"] = Model::Create(Model::Quad);
    {
        glm::mat4& pose = m_models["flake"]->localPose;
        pose = glm::scale(pose, glm::vec3(0.03f));
        pose = glm::rotate(pose, glm::pi<float>()/4.0f, glm::vec3(1, 0, 0));
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
    m_scene.renderer().draw(Render::DrawType::Shadows,  m_models["socle"]);
    m_scene.renderer().draw(Render::DrawType::Lights,   m_models["tree"]);
    m_scene.renderer().draw(Render::DrawType::Particle, m_models["flake"]);
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

#ifdef _DEBUG
    #define N_FLAKES 50
#else
    #define N_FLAKES 1500
#endif

    static constexpr size_t Max_Flakes = N_FLAKES;
    static constexpr float Spread      = 1.2f;

    static std::default_random_engine gen;
    static std::uniform_real_distribution<float> dstr(-0.5f, 0.5f);

    static size_t id_flake = 0;

    // Comment this to get snow in continue
    if (m_flakes.size() >= Max_Flakes) {
        return;
    }

    // Generate one
    m_flakes.push_back(_Flake { 
        /* .pose     = */ glm::translate(Identity, Spread * glm::vec3(dstr(gen), dstr(gen), 1.f)),
        /* .id       = */ (id_flake + 1) % Max_Flakes,
        /* .accurate = */ false,
        /* .moving   = */ true
    });

    // Limits
    while (m_flakes.size() > Max_Flakes) {
        m_flakes.pop_front();
    }
}

void SampleSnow::_compute_physics(float delta_time_seconds) 
{
    static const glm::vec3 Gravity = glm::vec3(0, 0, -0.5f);

    for (_Flake& flake : m_flakes) {
        if (!flake.moving)
            continue;

        if (!_is_flake_colliding(flake.pose, flake.accurate)) {
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

void SampleSnow::_update_models() 
{
    m_models["flake"]->poses.clear();
    m_models["flake"]->poses.reserve(m_flakes.size());

    for (const _Flake& flake : m_flakes) {
        m_models["flake"]->poses.push_back(flake.pose);
    }
}

bool SampleSnow::_is_flake_colliding(const glm::mat4& flake_pose, bool accurate) const
{
    const glm::mat4& socle_pose = m_models.at("socle")->poses.front();
    const glm::mat4& tree_pose  = m_models.at("tree")->poses.front();

    if (Collider::CheckHitboxes(
        m_models.at("flake"), flake_pose, 
        m_models.at("socle"), socle_pose).has_value()
    ) 
        return true;

    if (!accurate)
        return Collider::CheckHitboxes(
            m_models.at("flake"), flake_pose,
            m_models.at("tree"), tree_pose
        ).has_value();

    return Collider::CheckAccurate(
        m_models.at("flake"), flake_pose,
        m_models.at("tree"), tree_pose
    ).has_value();
}

#endif