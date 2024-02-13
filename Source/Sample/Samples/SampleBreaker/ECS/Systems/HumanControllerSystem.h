#pragma once

#include <Engine/Framework/Core/System.hpp>

struct HumanControllerSystem : public System
{
    void init();
    void update(const int key);
};