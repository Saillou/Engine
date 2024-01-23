#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "Wrapper/Shader.hpp"

// Objects
struct Cookable {
    // Basic possibilities
    enum class CookType {       
        Basic,
        Geometry,
    };

    // Shader helper
    static void Set(CookType type, Shader& shader);

    // Instance
    virtual ~Cookable() = default;

    Cookable* addRecipe(CookType type);
    void editRecipe(CookType shaderType, ShaderSource::Type sourceType, const ShaderSource& ss);

    bool has(CookType type) const;
    UShader& get(CookType type);

protected:
    static ShaderSource _init_vertex();
    static ShaderSource _init_fragment();

    static void _set_shader_basic(Shader& shader);
    static void _set_shader_geometry(Shader& shader);

    // Members
    std::unordered_map<CookType, UShader> m_shaders;
};
