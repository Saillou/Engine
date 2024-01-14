#pragma once

#include "../../Graphic/Utils/Shader.hpp"
#include "../../Graphic/Utils/Framebuffer.hpp"
#include "../../Graphic/Base/Shapes/Quad.hpp"

struct BaseFilter {
    // Members
    Quad        surface;
    Shader      shader;
    Framebuffer framebuffer;

    // Methods
         BaseFilter ();
         BaseFilter (Shader& sh);

    void apply      (Framebuffer& fIn);
    void resize     (int width, int height);
};