#pragma once

#include <vector>
#include <memory>

#include "../../Engine/Graphic/Base/BaseScene.hpp"
#include "../../Engine/Graphic/Base/Shapes/Sphere.hpp"
#include "../../Engine/Graphic/Base/Shapes/Box.hpp"

// -- Scene --
struct FireScene : public BaseScene {
    FireScene();

    void draw() override;
    void move_ball(const glm::vec3& delta_position);

private:

    glm::vec4 _getBrightness(float ratio) const;
    glm::vec4 _getColor() const;
    glm::vec3 _getParticulePos(size_t particule_id) const;
    glm::vec4 _getRandomSpeed() const;

    struct FireBall {
        glm::vec3 pos;

        struct Particles {
            const size_t amount;

            std::unique_ptr<Box> object;
            std::vector<glm::mat4> models;
            std::vector<glm::vec4> colors;

            std::vector<glm::vec4> speeds;
        } particles;
    } m_fireBall;
};
