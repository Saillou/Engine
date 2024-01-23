#pragma once

#include "../../Graphic/Wrapper/Shader.hpp"
#include "../../Graphic/Wrapper/Framebuffer.hpp"
#include "../../Graphic/Base/Model/Primitive/Quad.hpp"

struct BaseFilter {
    BaseFilter();
    explicit BaseFilter(Shader& sh);
    virtual ~BaseFilter() = default;

    BaseFilter& operator=(const BaseFilter&) = delete;
    BaseFilter(const BaseFilter&) = delete;
    BaseFilter(BaseFilter&&) = delete;

    void apply  (Framebuffer& fInOut);
    void apply  (const Framebuffer& fIn, Framebuffer& fOut);
    void compute(Framebuffer* = nullptr); // When nullptr, apply to _framebuffer
    void resize (int width, int height);

    // Members
    Shader& shader();
    const Framebuffer& frame() const;
    const Quad& surface() const;

protected:
    Shader      _shader;
    Quad        _surface;
    Framebuffer _framebuffer;
};