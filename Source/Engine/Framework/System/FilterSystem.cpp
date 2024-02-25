#include "FilterSystem.hpp"
#include "../../Graphic/ShaderManager.hpp"
#include "../../Utils/Filter/BaseFilter.hpp"

#include <iostream>
#include <algorithm>

FilterSystem::FilterSystem(const Camera& camera) :
    _camera(camera),
    _framebuffer_main(Framebuffer::Multisample, (unsigned int)camera.screenSize.x, (unsigned int)camera.screenSize.y),
    _framebuffer_filter(Framebuffer::Unique, (unsigned int)camera.screenSize.x, (unsigned int)camera.screenSize.y)
{
    _screenQuad.mesh->setupVao();
}

void FilterSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<FilterComponent>());

    ECS::setSystemSignature<FilterSystem>(signature);
}

void FilterSystem::start()
{
    bool need_framebuffer = false;

    for (Entity entity : m_entities) {
        if (ECS::getComponent<FilterComponent>(entity).enabled) {
            need_framebuffer = true;
            break;
        }
    }

    if (need_framebuffer) {
        _framebuffer_main.resize((unsigned int)_camera.screenSize.x, (unsigned int)_camera.screenSize.y);
        _framebuffer_main.bind();
    }
}

void FilterSystem::apply()
{
    if (!_framebuffer_main.isBound())
        return;

    _framebuffer_main.unbind();
    _framebuffer_filter.resize(_framebuffer_main.width(), _framebuffer_main.height());

    Framebuffer::Blit(_framebuffer_main, _framebuffer_filter);

    std::vector<Entity> ordered_entities(m_entities.cbegin(), m_entities.cend());
    std::sort(ordered_entities.begin(), ordered_entities.end(), [=](Entity e1, Entity e2) {
        return ECS::getComponent<FilterComponent>(e1).order > ECS::getComponent<FilterComponent>(e1).order;
    });

    for (Entity entity : ordered_entities) 
    {
        FilterComponent component = ECS::getComponent<FilterComponent>(entity);
        if (!component.enabled)
            continue;

        if (!component.filter)
            continue;

        component.filter->resize(_framebuffer_filter.width(), _framebuffer_filter.height());
        component.filter->apply(_framebuffer_filter, _framebuffer_filter, 0);
    }

    _drawFrame();
}

void FilterSystem::_drawFrame() {
    // Cleanup previous draws
    glClearColor(0.05f, 0.05f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    
    _framebuffer_filter.texture().bind();

    Texture::activate(GL_TEXTURE0);
    _screenQuad.texture_location = 0;
    
    ShaderManager::Get(CookType::Shape)
        .use()
        .set("LocalModel", _screenQuad.transform())
        .set("projection", glm::mat4(1.0f))
        .set("quadTexture", _screenQuad.texture_location)
        .set("background_color", glm::vec4(0.7f, 0.0f, 0.7f, 1.0f)) // fallback color when texture is empty
    ;

    _screenQuad.drawElements();

    // set back to original state.
    Texture::deactivate(GL_TEXTURE_2D, GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
}