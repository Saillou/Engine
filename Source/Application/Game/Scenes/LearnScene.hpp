#pragma once

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Quad.hpp>

// -- Scene --
struct LearnScene : public BaseScene {
    LearnScene();

    void draw() override;
    void _onResize() override;

private:
    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Quad> m_quad;
};
