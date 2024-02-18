#pragma once

#include <Engine/Framework/Core/System.hpp>

struct CollideSystem : public System 
{
    void init();
    void update(float dt);
};