#pragma once

#include "Actor.hpp"
#include <random>
#include <algorithm>

struct Ball : public Actor {
    // Constantes
    inline static const std::string Entity_Name = "ball";

    // Members
    glm::vec3 speed = { 0, 0, 0 };

    // Methods
    Ball():
        _fire(ManagedEntity::Create(Model::Load(Model::SimpleShape::Cube))),
        Actor(Model::Load(Model::SimpleShape::Sphere))
    {
        // Ball
        _entity.color() = glm::vec4(1.0f, 0.4f, 0.2f, 1.0f);
        _entity.local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
        _entity.collidable(true);

        // Fire ball
        _fire->local()    = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
        _fire->world()    = _entity.world();
        _fire->material() = _entity.material();

        ParticleComponent particles;
        particles.type = ParticleComponent::Type::Solid;

        for (size_t i = 0; i < 100; i++)
        {
            ParticleComponent::Element spark;
            spark.color = glm::min(glm::vec4(1.5f * i * _fire->color() / 100.0f) + glm::vec4(1, 0, 0, 1), glm::vec4(1.0f));
            spark.world = glm::mat4(0.0f);
            particles.elements.emplace_back(std::move(spark));

            _sparksSpeed.push_back(glm::vec3(0.0f));
        }
        ECS::addComponent(_fire->entity(), particles);

        // `ManagedEntity` creates a draw component by default, which the particle system needs not 
        ECS::removeComponent<DrawComponent>(_fire->entity());
    }

    void animate(float dt_ms) 
    {
        ParticleComponent& particles = ECS::getComponent<ParticleComponent>(_fire->entity());
        auto& sparks = particles.elements;

        for (int i = 0; i < (int)particles.elements.size(); i++)
        {
            glm::vec3& speed = _sparksSpeed[i];
            glm::mat4& model = sparks[i].world;

            const bool hasEnded = model[0][0] < 1e-4f || model[1][1] < 1e-4f || model[2][2] < 1e-4f; // also true for first draw

            if (hasEnded) {
                const int SIZE = (int)sqrt(particles.elements.size());
                float x = (i % SIZE - SIZE / 2) / float(SIZE);
                float y = (i / SIZE - SIZE / 2) / float(SIZE);

                speed = glm::vec3(0, 0.001f, 0);
                model = glm::translate(glm::mat4(1.0f), 0.25f*glm::vec3(x, 0.2f, y));
            }
            else {
                model = glm::translate(model, dt_ms * speed);
            }
        }
    }

    void setPos(const glm::vec3& pos) override
    {
        _pos = pos;
        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
        _fire->world() = _entity.world();
    }

private:
    SharedEntity _fire;
    std::vector<glm::vec3> _sparksSpeed;
};
