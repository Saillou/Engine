#include "BaseFilter.hpp"

BaseFilter::BaseFilter() : framebuffer(Framebuffer::Unique) 
{
    // ..
}
BaseFilter::BaseFilter(Shader& sh) : 
    framebuffer(Framebuffer::Unique),
    shader(sh) 
{
    // ..
}

void BaseFilter::apply(Framebuffer& fIn) {
    // Multisample -> Monosample
    Framebuffer::Blit(fIn, framebuffer);

    // Draw
    framebuffer.bind();
    glDisable(GL_DEPTH_TEST);
    {
        shader.use();
        framebuffer.texture().bind();
        surface.drawElements();
    }
    glEnable(GL_DEPTH_TEST);
    framebuffer.unbind();
}

void BaseFilter::resize(int width, int height) {
    framebuffer.resize(width, height);
}