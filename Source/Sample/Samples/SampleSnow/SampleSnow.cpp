#include "SampleSnow.hpp"

#include <Engine/Utils/Physic/Collider.hpp>
#include <Engine/Graphic/Base/Model/MeshIterator.hpp>
#include <random>

#define DISABLE_REAL
#ifdef DISABLE_REAL
SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    // Enable events
    _subscribe(&SampleSnow::_update);
    _subscribe(&SampleSnow::_draw);
    _subscribe(&SampleSnow::_on_key_pressed);

    // Create model
    Model::Ref modelTriangle = Model::Create();
    std::unique_ptr<Mesh> meshTriangle = std::make_unique<Mesh>();

    modelTriangle->root->meshes.push_back(std::move(meshTriangle));
    m_models["triangle"] = modelTriangle;

    // Shader
    m_shaders["triangle"]
        .attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")
            .add_var("layout (location = 3) in", "vec4", "aColor")
            .add_var("layout (location = 4) in", "mat4", "aModel")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "LocalModel")
            .add_var("uniform", "vec4", "diffuse_color")

            .add_var("out", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec4 Color;
            } vs_out)_struct_")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color   = max(aColor, diffuse_color);

                gl_Position    = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        )
        .attachSource(Shader::Geometry, ShaderSource{}
            .add_var("in", "layout", "(triangles)")
            .add_var("out", "layout", "(triangle_strip, max_vertices = 3)")

            .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec4 Color;
            } gs_in[])_struct_")

            .add_var("out", "vec3", "Center")
            .add_var("out", "vec3", "FragPos")
            .add_var("out", "vec4", "Color")
            .add_var("out", "float", "Radius")

            .add_func("void", "main", "", R"_main_(
                // Compute circle dimensions (by our current quad definition: gs_in[1] is the right angle apex)
                Center = 0.5 * (gs_in[0].FragPos + gs_in[2].FragPos);
                Radius = 0.5 * distance(gs_in[0].FragPos, gs_in[1].FragPos);

                // Emit triangle
                for(int i = 0; i < 3; i++) {
                    FragPos = gs_in[i].FragPos;
                    Color   = gs_in[i].Color;

                    gl_Position = gl_in[i].gl_Position;
                    EmitVertex();
                }
    
                EndPrimitive();
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec3", "Center")
            .add_var("in", "vec3", "FragPos")
            .add_var("in", "vec4", "Color")
            .add_var("in", "float", "Radius")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                float dist = distance(Center, FragPos);
                float smooth_dist = Radius * 4.0 / 100.0;
                float smooth_fact = 1.0 - abs(Radius - dist) / smooth_dist;
                
                FragColor = vec4(Color.rgb, Color.a * smooth_fact);
            )_main_")
        )
        .link()
    ;

    // Render callbacks
    Renderer::ShaderGetter getter = [=]() -> Shader& {
        return m_shaders["triangle"];
    };

    Renderer::ShaderSetter setter = [=](const Model::Ref& model) -> void {
        getter().use()
            .set("Projection",    m_scene.camera().projection)
            .set("View",          m_scene.camera().modelview)
            .set("diffuse_color", model->localMaterial.diffuse_color);
    };

    // Add to list
    m_scene.renderer().add_shader("triangle", getter, setter);
}

// Events
void SampleSnow::_update(const CommonEvents::StateUpdated&)
{

}

void SampleSnow::_draw(const SceneEvents::Draw&)
{
    m_scene.renderer().draw("triangle", m_models["triangle"]);
}

void SampleSnow::_on_key_pressed(const CommonEvents::KeyPressed& evt)
{

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

    //_draw_hitbox("socle");
    //_draw_hitbox("tree");
    //_draw_hitbox("flake");
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

    static constexpr size_t Max_Flakes = 10000;
    static constexpr float Spread      = 1.5f;

    static std::default_random_engine gen;
    static std::uniform_real_distribution<float> dstr(-0.5f, 0.5f);

    static size_t id_flake = 0;

    //if (m_flakes.size() > 0)
    //    return;

    // Generate one
    m_flakes.push_back(_Flake { 
        /* .pose   = */ glm::translate(Identity, Spread * glm::vec3(dstr(gen), dstr(gen), 0.75f)),
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
            m_models.at("socle"), socle_pose,
            true
        ).has_value() 
        ||
        Collider::Check(
            m_models.at("flake"), flake_pose, 
            m_models.at("tree"),  tree_pose,
            true
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

#endif