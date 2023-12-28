#include "BloomScene.hpp"

#include <iostream>

// -- Scene instance --
BloomScene::BloomScene() :
    BaseScene(),
    ball_model(0.25f),
    light_model(0.07f),
    framebuffer(Framebuffer::Multisample, m_width, m_height)
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -10.0f, 0.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // Lights
    m_lights = { 
        Light(glm::vec3{ 0,0,1 }, glm::vec4{ 1,1,1,1 }),
        Light(glm::vec3{ 1,0,1 }, glm::vec4{ 1,0,0,1 }),
        Light(glm::vec3{ 0,1,1 }, glm::vec4{ 0,1,0,1 }),
        Light(glm::vec3{ 1,1,1 }, glm::vec4{ 0,0,1,1 }),
    };

    // Objects
    ball_model.addRecipe(Cookable::CookType::Solid, glm::vec4(1, 1, 1, 1));
    light_model.addRecipe(Cookable::CookType::Solid);
}

void BloomScene::draw() {
    framebuffer.bind();
    framebuffer.clear();
    {
        // Objects
        ball_model.draw(m_camera, {}, {}, m_lights);

        // Lights
        for (auto& light : m_lights) {
            light_model.get(Cookable::CookType::Solid)->use().set("color", light.color);
            light_model.draw(m_camera, light.position);
        }
    }
    framebuffer.unbind();

    BaseScene::clear();
    BaseScene::drawFrame(framebuffer);
}

void BloomScene::_onResize() {
    framebuffer.resize(m_width, m_height);
}
