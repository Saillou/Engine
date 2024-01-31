#pragma once

#include "../../Graphic/Wrapper/Shader.hpp"
#include "../../Graphic/Wrapper/Framebuffer.hpp"
#include "../../Graphic/Base/Model/Primitive/Quad.hpp"

// In this filter:
// - Framebuffer    fb0     : image source
// - Framebuffer    fb1     : image to paste
// - float          opacity : opacity of the image to paste
// - vec4           color   : background color of fb1 (for removal)
struct CopyFilter
{
    CopyFilter(int width = 0, int height = 0);

    void apply(const Framebuffer& fb0, const Framebuffer& fb1, float opacity, const glm::vec4& background);
    const Framebuffer& result() const;

    void resize(int width, int height);

private:
    Quad _surface;
    Shader _shader;
    Framebuffer _frame_solo_a;
    Framebuffer _frame_solo_b;
    Framebuffer _frame_solo_c;
};