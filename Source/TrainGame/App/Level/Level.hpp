#pragma once

namespace Thomas
{
    struct Level
    {
        virtual void load()             = 0;
        virtual void unload()           = 0;
        virtual void update(float dt)   = 0;
    };
}