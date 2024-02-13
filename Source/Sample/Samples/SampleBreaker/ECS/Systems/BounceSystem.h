#pragma once

#include <Engine/Framework/Core/System.hpp>

struct BounceSystem : public System 
{
    void init();
    void update(float dt);
};