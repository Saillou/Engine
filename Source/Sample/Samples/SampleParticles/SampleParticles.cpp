#include "SampleParticles.hpp"

#include <memory>
#include <iostream>
#include <random>

// Random engine
static std::default_random_engine gen;
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);

// Particles
SampleParticles::SampleParticles() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = { };
    m_scene.camera().up        = glm::vec3(0, 1, 0);
    m_scene.camera().position  = glm::vec3(0.0f, 0.0f, +4.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Particles shape
    m_models["particle"]                                     = Model::Create(Model::Quad);
    m_models["particle"]->localTransform                          = glm::scale(glm::mat4(1.0f), glm::vec3(0.03f));
    m_models["particle"]->localMaterial        = glm::vec4(0.7f, 1.0f, 0.9f, 0.5f);

    m_models["particle_custom"]                              = Model::Create(Model::Quad);
    m_models["particle_custom"]->localTransform                   = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
    m_models["particle_custom"]->localMaterial = glm::vec4(1.0f, 1.0f, 1.0f, 0.4f);

    m_models["razengan_geom"]                                = Model::Create(Model::Sphere);
    m_models["razengan_geom"]->localTransform                     = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    m_models["razengan_geom"]->localMaterial   = glm::vec4(0.7f, 1.0f, 0.9f, 0.9f);
    m_models["razengan_geom"]->transforms                         = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

    m_models["razengan_shape"]                               = Model::Create(Model::Sphere);
    m_models["razengan_shape"]->localTransform                    = glm::scale(glm::mat4(1.0f), glm::vec3(0.499f));
    m_models["razengan_shape"]->localMaterial  = glm::vec4(0.2f, 0.5f, 0.6f, 1.0f);
    m_models["razengan_shape"]->transforms                        = { glm::mat4(1.0f) };

    // Custom shader for particules
    _add_custom_shader("custom_particule_shader");

    // Enable events
    _subscribe(&SampleParticles::_draw);

    // Go
    m_timer.tic();
}

SampleParticles::~SampleParticles() {
    m_scene.camera().up = glm::vec3(0, 0, 1);

    m_scene.renderer().remove_shader("custom_particule_shader");
}

// Events
void SampleParticles::_draw(const SceneEvents::Draw&) 
{
    // Update
    _update_particle(_particles,        m_models["particle"]->transforms);
    _update_particle(_particles_custom, m_models["particle_custom"]->transforms);

    float dt_s = m_timer.elapsed<Timer::millisecond>() / 1000.0f;
    glm::vec3 angular_speed = glm::vec3(2.f, 3.f, 5.f);

    glm::mat4& pose_0 = m_models["razengan_geom"]->transforms.at(0);
    pose_0 = glm::rotate(pose_0, angular_speed.x * dt_s, glm::vec3(1, 0, 0));

    glm::mat4& pose_1 = m_models["razengan_geom"]->transforms.at(1);
    pose_1 = glm::rotate(pose_1, angular_speed.y * dt_s, glm::vec3(0, 1, 0));

    glm::mat4& pose_2 = m_models["razengan_geom"]->transforms.at(2);
    pose_2 = glm::rotate(pose_2, angular_speed.z * dt_s, glm::vec3(0, 0, 1));

    // Draw items
    m_scene.renderer().draw(Render::DrawType::Particle, m_models["particle"]);
    m_scene.renderer().draw("custom_particule_shader", m_models["particle_custom"]);

    m_scene.renderer().draw(Render::DrawType::Geometry, m_models["razengan_geom"]);
    m_scene.renderer().draw(Render::DrawType::Basic,    m_models["razengan_shape"]);


    // Prepare next
    static const _conditions _particles_conditions = {
        /*.number_limit = */ 150,
        /*.p_start      = */ glm::vec3(0.0f, 0.0f, 0.0f),
        /*.p_range      = */ glm::vec3(1.0f, 1.0f, 1.0f),
        /*.v_dir        = */ glm::vec3(0.1f,0.1f,0.1f),
        /*.v_max        = */ 0.01f,
    };
    static const _conditions _custom_particles_conditions = {
        /*.number_limit = */ 150,
        /*.p_start      = */ glm::vec3(0.0f, 0.0f, 0.0f),
        /*.p_range      = */ glm::vec3(1.0f, 1.0f, 1.0f),
        /*.v_dir        = */ glm::vec3(0.1f,0.1f,0.1f),
        /*.v_max        = */ 0.1f,
    };

    _generate_particle(_particles, _particles_conditions);
    _generate_particle(_particles_custom, _custom_particles_conditions);

    m_timer.tic();
}

// Particules methods
void SampleParticles::_generate_particle(std::deque<_particle>& container, const _conditions& cnd) 
{
    container.push_back({
        /* .particle.pose  = */ glm::translate(glm::mat4(1.0f), cnd.p_start + cnd.p_range * (dstr_one(gen) - 0.5f)),
        /* .particle.speed = */ cnd.v_dir * cnd.v_max * glm::vec3(
            (dstr_one(gen) - 0.5f),
            (dstr_one(gen) - 0.5f),
            (dstr_one(gen) - 0.5f)
        )
    });

    while (container.size() > cnd.number_limit)
        container.pop_front();
}

void SampleParticles::_update_particle(std::deque<_particle>& container, std::vector<glm::mat4>& entity_transforms)
{
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;

    entity_transforms.clear();

    for (_particle& p : container) {
        p.pose = glm::translate(p.pose, p.speed * dt_ms);
        entity_transforms.push_back(p.pose);
    }
}

// Shader methods
void SampleParticles::_add_custom_shader(const std::string& name)
{
    // Add to memory
    _create_shader(name);

    // Render callbacks
    Renderer::ShaderGetter getter = [=]() -> Shader& {
        return m_shaders[name];
    };

    Renderer::ShaderSetter setter = [=](const Model::Ref& model) -> void {
        getter().use()
            .set("Projection",    m_scene.camera().projection)
            .set("View",          m_scene.camera().modelview)
            .set("diffuse_color", model->localMaterial);
    };

    // Add to list
    m_scene.renderer().add_shader("custom_particule_shader", getter, setter);
}

void SampleParticles::_create_shader(const std::string& name)
{
    m_shaders[name]
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
        .link();
}