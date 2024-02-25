#pragma once

#include "Actor.hpp"

#include <random>
#include <algorithm>
#include <Engine/Framework/Component/ParticleComponent.hpp>

struct FireBall {
    FireBall(
        glm::vec4 color = glm::vec4(1.0f, 0.7f, 0.3f, 0.7f),
        float pixel_size = 0.012f
    );

    glm::vec3 pos;
    SharedEntity entity;

    // Methods
    void animate(float dt_ms);

    size_t amount() const {
        return SIDE * SIDE;
    }

    Entity id() const {
        return entity->entity();
    }

    ParticleComponent& particle() {
        return ECS::getComponent<ParticleComponent>(id());
    }

private:
    const size_t SIDE = 50;

    glm::vec4 color;
    float pix_size;
    std::vector<glm::vec4> speeds;
    std::vector<int> life_cycles;

    glm::vec4 _getBrightness(float ratio) const;
    glm::vec3 _getParticulePos(size_t particule_id) const;
    glm::vec4 _getRandomSpeed() const;
    int _getRandomLife() const;
};