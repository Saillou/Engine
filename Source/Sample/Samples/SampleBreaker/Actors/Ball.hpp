#pragma once

#include "Actor.hpp"

struct Ball : public Actor {
    // Constantes
    inline static const std::string Entity_Name = "ball";

    // Members
    glm::vec3 speed = { 0, 0, 0 };

    // Methods
    Ball():
        Actor(Model::Load(Model::SimpleShape::Sphere))
    {
        _entity.color() = glm::vec4(1.0f, 0.4f, 0.2f, 1.0f);
        _entity.local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
        _entity.collidable(true);
    }
};
