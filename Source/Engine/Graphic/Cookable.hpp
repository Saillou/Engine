#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "Utils/Shader.hpp"

// Objects
struct Cookable {
    // Basic possibilities
    enum class CookType {       
        Basic,
        Shadow,
        Geometry,
    };

    // Shader helper
    static void Set(CookType type, Shader& shader);

    // Instance
    virtual ~Cookable() = default;

    virtual Cookable* addRecipe(CookType type);
    virtual Cookable* addRecipe(CookType type, const glm::vec4& color);

    UShader& get(CookType type);

protected:
    static ShaderSource _init_vertex();
    static ShaderSource _init_fragment();

    static void _set_shader_basic(Shader& shader);
    static void _set_shader_shadow(Shader& shader);
    static void _set_shader_geometry(Shader& shader);

    // Members
    std::unordered_map<CookType, UShader> m_shaders;
};
