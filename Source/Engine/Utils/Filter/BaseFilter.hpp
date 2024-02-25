#pragma once

#include "../../Framework/Core/ECS.hpp"
#include "../../Framework/Component/FilterComponent.hpp"

#include "../../Graphic/Wrapper/Shader.hpp"
#include "../../Graphic/Wrapper/Framebuffer.hpp"
#include "../../Graphic/Base/Model/Primitive/Quad.hpp"

struct BaseFilter {
    BaseFilter();
    virtual ~BaseFilter();

    BaseFilter& operator=(const BaseFilter&) = delete;
    BaseFilter(const BaseFilter&) = delete;
    BaseFilter(BaseFilter&&) = delete;

    void apply  (const Framebuffer& fIn, Framebuffer& fOut, int texture_location = 0);
    void compute(Framebuffer* = nullptr, int texture_location = 0); // When nullptr, apply to _framebuffer
    void resize (int width, int height);

    // Members
    Shader& shader();
    bool& enabled();
    const Framebuffer& frame() const;
    const Quad& surface() const;

protected:
    Entity      _entity;
    Shader      _shader;
    Quad        _surface;
    Framebuffer _framebuffer;
};