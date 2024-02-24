#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "Wrapper/Shader.hpp"

// Shaders compiled by default
enum class CookType {
    Geometry,
    Shadow,
    Shape,
    Solid,
    Skybox,
    Particle,
    Text,
};

struct ShaderManager 
{
    static void Init(); // Called by Window

    static void Provide(const std::string& identifier, std::shared_ptr<Shader> shader);
    static void Release(const std::string& identifier);
    static bool Has(const std::string& identifier);

    static void Edit(CookType identifier, ShaderSource::Type sourceType, const ShaderSource& new_source);
    static void Edit(const std::string& identifier, ShaderSource::Type sourceType, const ShaderSource& new_source);

    static Shader& Get(const std::string& identifier);
    static Shader& Get(CookType identifier);

protected:
    ShaderManager() = default;
    virtual ~ShaderManager() = default;

    void _create_default_shader();

    static std::string _getUUID(CookType);
    std::unordered_map<std::string, std::shared_ptr<Shader>> _shaders = {};

    static std::shared_ptr<ShaderManager> _s_manager;
};
