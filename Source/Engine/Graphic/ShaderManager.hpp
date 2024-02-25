#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <functional>
#include <unordered_map>

#include "Wrapper/Shader.hpp"

// Shaders compiled by default
struct CookType {
    enum _tag_ { 
        Geometry    = -9,
        Shadow      = -8,
        Shape       = -7,
        Solid       = -6,
        Skybox      = -5,
        Particle    = -4,
        Text        = -3,

        // Custom tags are positives
        Custom      = 0,
    };
};

struct ShaderManager 
{
    typedef std::function<void(void)> UniformSetter;

    static void Init(); // Called by Window

    static void Provide(CookType::_tag_, std::shared_ptr<Shader> shader);
    static void Provide(CookType::_tag_, std::shared_ptr<Shader> shader, UniformSetter);

    static void Release(CookType::_tag_);
    static bool Has(CookType::_tag_);

    static void Edit(CookType::_tag_, ShaderSource::Type sourceType, const ShaderSource& new_source);
    static void Apply(CookType::_tag_);
    static Shader& Get(CookType::_tag_);

protected:
    ShaderManager() = default;
    virtual ~ShaderManager() = default;

    void _create_default_shaders();

    std::unordered_map<CookType::_tag_, std::shared_ptr<Shader>> _shaders = {};
    std::unordered_map<CookType::_tag_, UniformSetter> _shadersSetters = {};

    static std::shared_ptr<ShaderManager> _s_manager;
};
