#include "FireBall.hpp"

#include <glm/gtc/noise.hpp>

FireBall::FireBall(glm::vec4 color, float sphere_radius, float pixel_size) :
    color(color),
    radius(sphere_radius),
    entity(ManagedEntity::Create(Model::Load(Model::SimpleShape::Cube))),
    pix_size(pixel_size),
    pos(glm::vec3(0.0f))
{
    // Particle model
    entity->local() = glm::scale(glm::mat4(1.0f), glm::vec3(pix_size));
    entity->draw().type = DrawComponent::Hidden; // will use particle system instead

    // Define particles
    speeds.resize(amount());
    life_cycles.resize(amount());

    for (size_t i = 0; i < life_cycles.size(); i++)
        life_cycles[i] = (int)(i / 10);

    ParticleComponent particleComponent;
    particleComponent.elements.resize(amount());

    ECS::addComponent(id(), particleComponent);
}


void FireBall::animate(float dt_ms) 
{
    ParticleComponent& particles = particle();
    float x_off = 0.0f;

    for (int x = 0; x < SIDE; x++) {
        x_off += 0.02f;
        float y_off = 0.0f;

        for (int y = 0; y < SIDE; y++) {
            y_off += 0.02f;

            size_t particules_id = x + y * SIDE;

            glm::mat4& model = particles.elements[particules_id].world;
            glm::vec4& color = particles.elements[particules_id].color;
            glm::vec4& speed = speeds[particules_id];
            int& life_cycle = life_cycles[particules_id];

            color = _getBrightness(glm::simplex(glm::vec4(x_off, y_off, 0, y_off)));

            if (life_cycle <= 0)
            {
                model = glm::translate(glm::mat4(1.0f), pos + _getParticulePos(particules_id));
                speed = _getRandomSpeed();
                life_cycle = _getRandomLife();
            }
            else {
                model = glm::scale(glm::translate(model, dt_ms / 1000.0f * glm::vec3(speed)), speed.a * glm::vec3(1, 1, 1));
                life_cycle--;
            }
        }
    }
}

glm::vec4 FireBall::_getBrightness(float ratio) const 
{
    return glm::min(
        glm::vec4(1.5f * ratio * color) + glm::vec4(1, 0, 0, 0),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
}

glm::vec3 FireBall::_getParticulePos(size_t particule_id) const 
{
    int s = int(particule_id % SIDE - SIDE / 2);
    int t = int(particule_id / SIDE - SIDE / 2);

    double x = radius * cos(s) * sin(t);
    double y = radius * sin(s) * sin(t);
    double z = radius * cos(t);

    return 1.5f * pix_size * glm::vec3(x, y, z);
}

glm::vec4 FireBall::_getRandomSpeed() const 
{
    static std::default_random_engine gen;
    static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
    static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

    return glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen) / 4.0f, 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
}

int FireBall::_getRandomLife() const 
{
    return 50 + rand() % 100;
}