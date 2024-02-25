#include "BaseFilter.hpp"
#include <iostream>

BaseFilter::BaseFilter(): 
    _entity(ECS::createEntity()),
    _framebuffer(Framebuffer::Unique)
{
    _surface.mesh->setupVao();

    FilterComponent componentFilter;
    componentFilter.filter = this; // i hate this but i've no other idea
    ECS::addComponent(_entity, componentFilter);
}

BaseFilter::~BaseFilter()
{
    ECS::destroyEntity(_entity);
}

void BaseFilter::apply(const Framebuffer& fIn, Framebuffer& fOut, int texture_location) {
    Framebuffer::Blit(fIn, _framebuffer);
    {
        compute(nullptr, texture_location);
    }
    Framebuffer::Blit(_framebuffer, fOut);
}

void BaseFilter::compute(Framebuffer* pF, int texture_location) {
    Framebuffer* pFramebuffer = pF ? pF : &_framebuffer;

    pFramebuffer->bind();
    glDisable(GL_DEPTH_TEST);
    Texture::activate(GL_TEXTURE0 + texture_location);
    {
        pFramebuffer->texture().bind();
        _shader.use();
        _surface.drawElements();
    }
    Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    pFramebuffer->unbind();
}

void BaseFilter::resize(int width, int height) {
    _framebuffer.resize(width, height);
}

Shader& BaseFilter::shader() {
    return _shader;
}

bool& BaseFilter::enabled() {
    return ECS::getComponent<FilterComponent>(_entity).enabled;
}

const Framebuffer& BaseFilter::frame() const {
    return _framebuffer;
}

const Quad& BaseFilter::surface() const {
    return _surface;
}
