#pragma once

#include <glm/glm.hpp>
#include <string>

struct _Player {
    enum class Action {
        None, Left, Right
    };

    inline static const std::string Entity_Name = "player";
    inline static const float MaxSpeed = 0.05f;

    glm::vec3 pos = { 0, 0, 0 };
    Action next_action = Action::None;
};


struct _Ball {
    inline static const std::string Entity_Name = "ball";

    glm::vec3 pos = { 0, 0, 0 };
    glm::vec3 speed = { 0, 0, 0 };
};


struct _Wall {
    inline static const std::string Entity_Name = "wall";

    glm::vec3 pos = { 0, 0, 0 };
};
