#include "SampleSnow.hpp"

#include <Engine/Utils/Physic/Collider.hpp>
#include <Engine/Graphic/Base/Model/MeshIterator.hpp>
#include <Engine/Graphic/Base/Model/Primitive/PrimitiveHelper.hpp>
#include <random>

static Shader& _create_triangle_shader(Shader&);
static void _push_triangle_mesh(std::unique_ptr<Model::Node>&, const PrimitiveHelper::Triangle&);

#define DISABLE_REAL
#ifdef DISABLE_REAL
SampleSnow::SampleSnow() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.camera().up = glm::vec3(0, 0, 1);
    m_scene.camera().position  = glm::vec3(0.0f, -3.0f, 2.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Enable events
    _subscribe(&SampleSnow::_update);
    _subscribe(&SampleSnow::_draw);
    _subscribe(&SampleSnow::_on_key_pressed);

    // Load tree
    m_models["tree"] = Model::Create("Resources/objects/tree/tree.glb");
    m_models["tree"]->materials = { Material{ glm::vec4(1, 0, 0, 1), false } };
    m_models["tree"]->poses = { glm::mat4(1.0f) };
    {
        glm::mat4& pose = m_models["tree"]->localPose;
        pose = glm::translate(pose, glm::vec3(0, 0.50f, +0.20f));
        pose = glm::scale(pose, glm::vec3(2.0f));
        pose = glm::rotate(pose, glm::half_pi<float>(), glm::vec3(1, 0, 0));
    }

    // Create debug model
    m_models["triangle"] = Model::Create();
    m_models["triangle"]->materials = { Material{ glm::vec4(1, 0, 0, 1), false } };
    m_models["triangle"]->poses     = { glm::mat4(1.0f) };

    MeshIterator::forEachMesh(*m_models["tree"], [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& acc) {
        const glm::mat4& q = glm::mat4(1.0f) * m_models["tree"]->localPose * acc.transform;

        for (size_t i1 = 0; i1 < mesh->indices.size(); i1 += 3) {
            _push_triangle_mesh(m_models["triangle"]->root,
            {
                glm::vec3(q * glm::vec4(mesh->vertices[mesh->indices[i1 + 0]].Position, +1.0f)),
                glm::vec3(q * glm::vec4(mesh->vertices[mesh->indices[i1 + 1]].Position, +1.0f)),
                glm::vec3(q * glm::vec4(mesh->vertices[mesh->indices[i1 + 2]].Position, +1.0f))
            });
        }
    });

    // Create shader
    _create_triangle_shader(m_shaders["triangle"]).link();

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

// ---
void _push_triangle_mesh(std::unique_ptr<Model::Node>& node, const PrimitiveHelper::Triangle& triangle) 
{
    std::unique_ptr<Mesh> meshTriangle = std::make_unique<Mesh>();
    PrimitiveHelper::createTriangle(*meshTriangle, triangle);
    meshTriangle->sendToGpu();
    meshTriangle->compute_obb();

    node->meshes.push_back(std::move(meshTriangle));
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