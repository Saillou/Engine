#pragma once

#include "Actor.hpp"

struct Brick : public Actor {
    // Constantes
    inline static const std::string Entity_Name = "brick";

    // Methods
    Brick(const glm::vec3& pos) :
        Actor(Model::Load(Model::SimpleShape::Cube))
    {
        setPos(pos);

        _entity.color() = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        _entity.local() = glm::scale(glm::mat4(1.0f), 0.3f * glm::vec3(0.5f, 0.1f, 0.1f));
        _entity.world() = glm::translate(glm::mat4(1.0f), pos);
        _entity.collidable(true);
    }
};

