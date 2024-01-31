#pragma once
#include "Level.hpp"

namespace Thomas
{
    class Sandbox : public Level
    {
    public:
        void loadLevel() override;
        void unloadLevel() override;

    private:

    };
}