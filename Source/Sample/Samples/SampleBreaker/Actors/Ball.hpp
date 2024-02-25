#pragma once

#include "Actor.hpp"
#include "FireBall.hpp"

#include <random>
#include <algorithm>

struct Ball : public Actor {
    // Constantes
    inline static const std::string Entity_Name = "ball";

    // Members
    glm::vec3 speed = { 0, 0, 0 };

    // Methods
    Ball():
        Actor(Model::Load(Model::SimpleShape::Sphere))
    {
        _entity.color() = glm::vec4(1.0f, 0.1f, 0.1f, 0.2f);
        _entity.local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
        _entity.collidable(true);
    }

    void setPos(const glm::vec3& pos) override
    {
        _pos = pos;
        _fireball.pos = _pos;

        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
    }

    void animate(float dt_ms) {
        _fireball.animate(dt_ms);
    }

private:
    FireBall _fireball;
};
