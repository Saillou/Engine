#pragma once

namespace Thomas
{
    struct Level
    {
        virtual void loadLevel()    = 0;
        virtual void unloadLevel()  = 0;
    };
}