#pragma once

#include "Sample/Samples/SampleBreaker/ECS/Core/Types.h"

struct HumanControllerSystem : public Thomas::System
{
    void init();
    void update(const int key);
};