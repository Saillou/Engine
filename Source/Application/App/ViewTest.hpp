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
#include <Engine/Utils/Objects/ShadowRender.hpp>

struct ViewTest : public BaseScene {
    ViewTest(int widthHint = 0, int heightHint = 0);

    void draw() override;

private:
    void renderScene(Shader& shader);
    void renderCube();

    // shader
    ShadowRender m_shadowRender;

    // object
    Cookable cook;
    std::unique_ptr<Texture> textureWood;

    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    // lights
    glm::vec3 lightPos;
};
