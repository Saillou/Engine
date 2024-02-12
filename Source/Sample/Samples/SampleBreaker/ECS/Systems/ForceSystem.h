#pragma once

#include "Sample/Samples/SampleBreaker/ECS/Core/Types.h"

struct ForceSystem : public Thomas::System 
{
    void init();
    void update(float dt);
};