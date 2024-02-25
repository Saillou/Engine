#pragma once

#include "Actor.hpp"

struct Field : public Actor {
    // Constantes
    inline static const std::string Entity_Name = "field";

    // Methods
    Field() :
        Actor(Model::Load(Model::SimpleShape::Cube))
    {
        _entity.color() = glm::vec4(1, 1, 1, 0.2f);

        glm::mat4& local(_entity.local());
        local = glm::translate(local, glm::vec3(0, -0.1f, 0));
        local = glm::scale(local, glm::vec3(2.0f, 0.01f, 2.0f));
        local = glm::rotate(local, glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0));

        _entity.world() = glm::translate(glm::mat4(1.0f), _pos);
    }
};
