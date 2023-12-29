#pragma once

#include <vector>
#include <memory>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>

#include <Engine/Utils/Timer.hpp>

// -- Scene --
struct View : public BaseScene {
    View();

    void draw() override;

    bool redraw = false;

private:
    Timer::Chronometre m_timer;

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
