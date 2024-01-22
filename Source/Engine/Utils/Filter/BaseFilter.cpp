#include "BaseFilter.hpp"

BaseFilter::BaseFilter() : _framebuffer(Framebuffer::Unique)
{
    // ..
}
BaseFilter::BaseFilter(Shader& sh) : 
    _framebuffer(Framebuffer::Unique),
    _shader(sh)
{
    // ..
}

void BaseFilter::apply(Framebuffer& fIn) {
    apply(fIn, fIn);
}

void BaseFilter::apply(const Framebuffer& fIn, Framebuffer& fOut) {
    // Multisample -> Monosample
    Framebuffer::Blit(fIn, _framebuffer);

    // Draw
    _framebuffer.bind();
    glDisable(GL_DEPTH_TEST);
    {
        _shader.use();
        _framebuffer.texture().bind();
        _surface.drawElements();
    }
    glEnable(GL_DEPTH_TEST);
    _framebuffer.unbind();

    // Monosample -> Multisample
    Framebuffer::Blit(_framebuffer, fOut);
}

void BaseFilter::resize(int width, int height) {
    _framebuffer.resize(width, height);
}

Shader& BaseFilter::shader() {
    return _shader;
}

const Framebuffer& BaseFilter::frame() const {
    return _framebuffer;
}
