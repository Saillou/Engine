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
    m_entities["particle"] = Entity(Entity::SimpleShape::Quad);
    m_entities["particle"].localMaterial().diffuse_color = glm::vec4(0.2f, 1.0f, 0.9f, 0.9f);

    m_entities["particle_custom"] = Entity(Entity::SimpleShape::Quad);
    m_entities["particle_custom"].localPose() = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
    m_entities["particle_custom"].localMaterial().diffuse_color = glm::vec4(0.7f, 1.0f, 0.9f, 0.5f);

    // Custom shader for particules
    _add_custom_shader("custom_particule_shader");

    // Enable events
    _subscribe(&SampleParticles::_draw);

    // Go
    m_timer.tic();
}

SampleParticles::~SampleParticles() {
    m_scene.camera().up = glm::vec3(0, 0, 1);
}

// Events
void SampleParticles::_draw(const SceneEvents::Draw&) 
{
    // Update
    _update_particle(_particles,        m_entities["particle"].poses());
    _update_particle(_particles_custom, m_entities["particle_custom"].poses());

    // Draw items
    m_scene.renderer().draw(Render::DrawType::Particle, m_entities["particle"]);
    m_scene.renderer().draw("custom_particule_shader",  m_entities["particle_custom"]);

    // Prepare next
    _generate_particle(_particles,        10'000);
    _generate_particle(_particles_custom, 1'000);

    m_timer.tic();
}

// Particules methods
void SampleParticles::_generate_particle(std::deque<_particle>& container, size_t limit) 
{
    _particle particle;

    // Positions
    constexpr float x_range = 300.0f;
    constexpr float y_range = 10.0f;
    constexpr float z_start = -900.0f;

    particle.pose = glm::translate(glm::mat4(1.0f), glm::vec3(
        x_range * (dstr_one(gen) - 0.5f),
        y_range * (dstr_one(gen) - 0.5f),
        z_start
    ));

    // Speed
    constexpr float v_max = 10.0f;

    particle.speed = glm::vec3(
        0.0f, 
        0.0f, 
        v_max * dstr_one(gen)
    );

    // Boum
    container.push_back(particle);

    while (_particles.size() > limit)
        _particles.pop_front();
}

void SampleParticles::_update_particle(std::deque<_particle>& container, std::vector<Pose>& entity_poses)
{
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;

    entity_poses.clear();

    for (_particle& p : _particles) {
        p.pose = glm::translate(p.pose, p.speed * dt_ms);
        entity_poses.push_back(p.pose);
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

    Renderer::ShaderSetter setter = [=](const Entity& entity) -> void {
        getter().use()
            .set("Projection",    m_scene.camera().projection)
            .set("View",          m_scene.camera().modelview)
            .set("diffuse_color", entity.localMaterial().diffuse_color);
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
                float smooth_fact = 1.0;

                // None
                if(dist > Radius) {
                    discard;
                }

                // Smooth
                else if(dist > Radius - smooth_dist)
                    smooth_fact = (Radius - dist) / smooth_dist;

                // Solid
                else
                    smooth_fact = 1.0;
                
                FragColor = vec4(Color.rgb, Color.a * smooth_fact);
            )_main_")
        )
        .link();

}