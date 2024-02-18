#pragma once

#include <Engine/Framework/Core/System.hpp>

struct PlayerControllerSystem : public System 
{
    void init();
    void update(float dt);
};