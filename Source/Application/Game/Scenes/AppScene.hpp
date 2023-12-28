#pragma once

#include <vector>
#include <memory>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>

// -- Scene --
struct AppScene : public BaseScene {
    AppScene();

    void draw() override;

    bool redraw = false;

private:

    struct FireGrid {
        glm::vec3 pos;

        struct Particles {
            const size_t amount;

            std::unique_ptr<Box> object;
            std::vector<glm::mat4> models;
            std::vector<glm::vec4> colors;

            std::vector<glm::vec4> speeds;
        } particles;
    } m_fireGrid;
};
