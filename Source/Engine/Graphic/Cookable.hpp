#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "Utils/Shader.hpp"

// Objects
struct Cookable {
    // Basic possibilities
    enum class CookType {
        Solid, 
        Geometry,
        Shadow,
        
        Model,
        ModelGeometry,
        
        Quad,
        
        Batch,
        BatchGeometry,
        BatchShadow
    };

    // Shader helper
    static void Set(CookType type, Shader& shader);

    // Instance
    virtual ~Cookable() = default;

    virtual Cookable* addRecipe(CookType type);
    virtual Cookable* addRecipe(CookType type, const glm::vec4& color);

    UShader& get(CookType type);

protected:
    static void _set_shader_batch(Shader& shader);
    static void _set_shader_solid(Shader& shader);
    static void _set_shader_model(Shader& shader);
    static void _set_shader_shadow(Shader& shader);
    static void _set_shader_shadow_batch(Shader& shader);
    static void _set_shader_geometry(Shader& shader);
    static void _set_shader_geometry_batch(Shader& shader);
    static void _set_shader_model_geometry(Shader& shader);
    static void _set_shader_quad(Shader& shader);

    // Members
    std::unordered_map<CookType, UShader> m_shaders;
};
