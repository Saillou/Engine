#include "SampleSnow.hpp"

#include <Engine/Utils/Physic/Collider.hpp>
#include <Engine/Graphic/Base/Model/MeshIterator.hpp>
#include <Engine/Graphic/Base/Model/Primitive/PrimitiveHelper.hpp>
#include <random>

// Particles
SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    _add_shaders("triangle");
    m_shaders["triangle"].use().set("override_color", glm::vec4(1, 0, 0, 1));

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
        m_models["socle"]->localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 0.05f));
        m_models["socle"]->localMaterial = glm::vec4(0.8f, 0.9f, 1.0f, 0.7f);
    }
    
    m_models["tree"] = Model::Create("Resources/objects/tree/tree.glb");
    {
        glm::mat4& pose = m_models["tree"]->localTransform;
        pose = glm::translate(pose, glm::vec3(0, 0.50f, +0.20f));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    }

    m_models["flake"] = Model::Create();
    {
        // Local pose
        glm::mat4& pose = m_models["flake"]->localTransform;
        pose = glm::scale(pose, glm::vec3(0.1f));
        pose = glm::rotate(pose, glm::pi<float>()/4.0f, glm::vec3(1, 0, 0));

        // Local material
        m_models["flake"]->localMaterial = glm::vec4(0.7f, 1.0f, 0.9f, 0.5f);

        // Model meshes
        _push_triangle_mesh(m_models["flake"]->root, {
            glm::vec3(-0.2f, 0.0f, +0.1f),
            glm::vec3(0,     0.0f, -0.1f),
            glm::vec3(+0.2f, 0.0f, +0.1f)
        });
        m_models["flake"]->setMeshVao(*m_models["flake"]->root->meshes.back());
    }

    // Populate
    m_models["socle"]->transforms = { glm::mat4(1.0f) };
    m_models["tree"]->transforms  = { glm::mat4(1.0f) };

    // Enable events
    _subscribe(&SampleSnow::_update);
    _subscribe(&SampleSnow::_draw);
    _subscribe(&SampleSnow::_on_key_pressed);

    // Go
    m_timer.tic();
}

SampleSnow::~SampleSnow()
{
    m_scene.renderer().remove_shader("triangle");
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

    if(m_ui.show_debug)
        _draw_debug("tree");

    m_ui.show();
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
void SampleSnow::_push_triangle_mesh(std::unique_ptr<Model::Node>& node, const PrimitiveHelper::Triangle& triangle)
{
    std::unique_ptr<Mesh> meshTriangle = std::make_unique<Mesh>();

    PrimitiveHelper::createTriangle(*meshTriangle, triangle);
    meshTriangle->compute_obb();

    node->meshes.push_back(std::move(meshTriangle));
}

void SampleSnow::_add_shaders(const std::string& shader_name) {
    // Create shader
    _create_triangle_shader(m_shaders[shader_name]).link();

    // Render callbacks
    Renderer::ShaderGetter getter = [=]() -> Shader& {
        return m_shaders[shader_name];
    };

    Renderer::ShaderSetter setter = [=](const Model::Ref& model) -> void {
        getter().use()
            .set("Projection", m_scene.camera().projection)
            .set("View", m_scene.camera().modelview)
            .set("diffuse_color", model->localMaterial);
    };

    // Add to list
    m_scene.renderer().add_shader(shader_name, getter, setter);
}

Shader& SampleSnow::_create_triangle_shader(Shader& shader)
{
    shader
        .attachSource(GL_VERTEX_SHADER,
            Cookable::_init_vertex()

            .add_var("uniform", "vec4", "override_color")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color   = length(override_color) == 0 ? max(aColor, diffuse_color) : override_color;
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
    m_flakes.push_back(_Flake { 
        /* .pose     = */ glm::translate(Identity, Spread * glm::vec3(dstr(gen), dstr(gen), 1.f)),
        /* .id       = */ (id_flake + 1) % (size_t)m_ui.flakes_number,
        /* .accurate = */ false,
        /* .moving   = */ true
    });

    // Limits
    while (m_flakes.size() > (size_t)m_ui.flakes_number) {
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
    m_models["flake"]->transforms.clear();
    m_models["flake"]->transforms.reserve(m_flakes.size());

    for (const _Flake& flake : m_flakes) {
        m_models["flake"]->transforms.push_back(flake.pose);
    }
}

bool SampleSnow::_is_flake_colliding(const glm::mat4& flake_pose, bool accurate) const
{
    const glm::mat4& socle_pose = m_models.at("socle")->transforms.front();
    const glm::mat4& tree_pose  = m_models.at("tree")->transforms.front();

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

// - Debug -
void SampleSnow::_draw_debug(const std::string& model_name) {
    // Check anti-stupidity
    if (m_models.find(model_name) == m_models.cend())
        return;

    const Model::Ref& model = m_models[model_name];
    if (!model->root || model->transforms.empty())
        return;

    // Create hitboxes model
    const std::string debug_box_name = "debug_box_" + model_name;

    if (m_models.find(debug_box_name) == m_models.cend()) {
        m_models[debug_box_name] = Model::Create(Model::Cube);
        m_models[debug_box_name]->localMaterial = glm::vec4(1, 1, 0, 1);
    }

    // Get all meshes' transforms
    m_models[debug_box_name]->transforms.clear();

    for (const glm::mat4& worldPose : model->transforms) {
        MeshIterator::forEachMesh(*model, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc) {
            m_models[debug_box_name]->transforms.push_back(worldPose * model->localTransform * node_acc.transform * mesh->obb());
        });
    }

    // Draw
    m_scene.renderer().draw(Render::Geometry, m_models[debug_box_name]);
    m_scene.renderer().draw("triangle",       m_models[model_name]);
}

void SampleSnow::Ui::show() {
#ifdef _DEBUG
    #define MAX_FLAKES 500
#else
    #define MAX_FLAKES 25000
#endif

    ImGui::Begin("Actions");
    ImGui::Separator();

    ImGui::Checkbox("Show debug", &show_debug);
    ImGui::Checkbox("Loop", &loop);
    ImGui::SliderInt("Flakes", &flakes_number, 1, MAX_FLAKES);

    ImGui::End();
}
