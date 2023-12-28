#include "LearnScene.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

// -- Scene instance --
LearnScene::LearnScene() :
    BaseScene()
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -10.0f, 0.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // Lights
    m_lights = {  };

    // Objects
    m_texture = std::make_unique<Texture>("C:\\Dev\\Cpp\\Project_008-Ball\\Resources\\textures\\container.jpg");
    m_quad = std::make_unique<Quad>(0.25f, 0.25f, 0.50f, 0.50f);
}

void LearnScene::draw() {
    BaseScene::clear();

    m_texture->bind();
    m_quad->draw();
}

void LearnScene::_onResize() {
    // ..
}
