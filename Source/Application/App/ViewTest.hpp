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
    ViewTest(int widthHint = 0, int heightHint = 0);

    void draw() override;

private:
    void createPointShadowShader();
    void createSimpleDepthShader();

    void renderScene(Shader& shader);
    void renderCube();

    // shader
    Shader shaderPoint;
    Shader simpleDepthShader;

    //unsigned int depthMapFBO = 0;
    std::unique_ptr<Framebuffer> framebufferDepth;
    std::unique_ptr<Texture> textureWood;

    // object
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    // lights
    glm::vec3 lightPos;
};
