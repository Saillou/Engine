#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>
#include <Engine/Utils/Objects/ShadowRender.hpp>

struct ViewShadow : public BaseScene {
    ViewShadow(int widthHint = 0, int heightHint = 0);

    void draw() override;

private:
    void renderScene(Shader& shader);
    void renderGround(Shader& shader);
    void renderCube(Shader& shader);

    // shader
    ShadowRender m_shadowRender;

    // object
    std::unique_ptr<Box> box;

    float m_time = 0.0f;
};
