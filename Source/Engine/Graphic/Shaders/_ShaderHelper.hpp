#pragma once

#include "../Wrapper/Shader.hpp"
#include "../Wrapper/ShaderSource.hpp"

#include <memory>

struct ShaderHelper 
{
    virtual ~ShaderHelper() = default;

protected:
    static ShaderSource _init_vertex();
    static ShaderSource _init_geometry();
    static ShaderSource _init_fragment();
};
