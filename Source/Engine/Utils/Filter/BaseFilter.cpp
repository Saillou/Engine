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
    Framebuffer::Blit(fIn, _framebuffer);
    {
        compute();
    }
    Framebuffer::Blit(_framebuffer, fOut);
}

void BaseFilter::compute(Framebuffer* pF) {
    Framebuffer* pFramebuffer = pF ? pF : &_framebuffer;

    pFramebuffer->bind();
    glDisable(GL_DEPTH_TEST);
    {
        _shader.use();
        pFramebuffer->texture().bind();
        _surface.drawElements();
    }
    glEnable(GL_DEPTH_TEST);
    pFramebuffer->unbind();
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
