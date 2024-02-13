#pragma once

struct PlayerController
{
    enum class Action
    {
        None = 0,
        Left,
        Right
    };

    float speed = 0.05f;
    Action action = Action::None;
};