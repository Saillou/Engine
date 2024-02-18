#pragma once

class Level
{
public:
    virtual ~Level() {};

    virtual void load()     = 0;
    virtual void unload()   = 0;
};