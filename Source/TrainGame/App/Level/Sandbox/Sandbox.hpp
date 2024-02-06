#pragma once
#include "TrainGame/App/Level/Level.hpp"

namespace Thomas
{
    class Sandbox : public Level
    {
    public:
        void load() override;
        void unload() override;
        void update(float dt) override;

    private:

    };
}