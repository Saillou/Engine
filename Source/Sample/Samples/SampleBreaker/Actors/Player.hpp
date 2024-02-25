#pragma once

#include "Actor.hpp"

struct Player : public Actor {
    // Constantes
    inline static const std::string Entity_Name = "player";
    inline static const float MaxSpeed = 0.05f;

    enum class Action {
        None, Left, Right
    };

    // Members
    Action next_action = Action::None;

    // Methods
    Player() :
        Actor(Model::Load(Model::SimpleShape::Cube))
    {
        _entity.color() = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        _entity.local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f));
        _entity.collidable(true);
    }

    void move()
    {
        switch (next_action) {
            case Action::Left:  _pos.x += MaxSpeed; break;
            case Action::Right: _pos.x -= MaxSpeed; break;
        }
        next_action = Action::None;

        setPos(_pos);
    }
};
