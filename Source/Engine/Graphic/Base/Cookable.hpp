#pragma once

#include <glm/glm.hpp>

#include "../Shaders/Shader.hpp"
#include "../../Utils/Caster.hpp"

// Objects
struct Cookable : public Object {
    // Basic possibilities
    enum class CookType {
        Solid, Quad, Batch
    };

    // Instance
    virtual ~Cookable() = default;

    virtual Cookable* addRecipe(const CookType& type);
    virtual Cookable* addRecipe(const CookType& type, const glm::vec4& color);

    UShader& get(const CookType& type);

protected:
    virtual void _set_shader_batch(UShader& shader);
    virtual void _set_shader_solid(UShader& shader);
    virtual void _set_shader_quad(UShader& shader);

    // Members
    std::unordered_map<CookType, UShader> m_shaders;
};
