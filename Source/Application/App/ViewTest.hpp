#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>
#include <Engine/Graphic/Model/Skybox.hpp>
#include <Engine/Graphic/Model/ObjectModel.hpp>
#include <Engine/Graphic/Utils/Framebuffer.hpp>

struct ViewTest : public BaseScene {
    ViewTest();

    void draw() override;

private:
    void _onResize() override;

    // ----------
    unsigned int loadTexture(const char* path);
    void createPointShadowShader();
    void createSimpleDepthShader();

    void renderScene(Shader& shader);
    void renderCube();

    // shader
    Shader shaderPoint;
    Shader simpleDepthShader;

    unsigned int depthCubemap = 0;
    unsigned int depthMapFBO = 0;
    unsigned int woodTexture = 0;

    // settings
    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;

    // object
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    // lights
    glm::vec3 lightPos;
};
