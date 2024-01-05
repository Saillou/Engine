#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Objects/ShadowRender.hpp>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>
#include <Engine/Graphic/Model/Skybox.hpp>
#include <Engine/Graphic/Model/ObjectModel.hpp>

struct View : public BaseScene {
    View(int widthHint = 0, int heightHint = 0);

    void draw() override;

    void drawObject(const glm::vec3& pos, int type);

private:
    void _initObjects();

    std::shared_ptr<Box> _modelCharacter;
    std::shared_ptr<Sphere> _modelBall;

    std::map<int, std::vector<glm::vec3>> _models;

    // Other members
    Timer::Chronometre m_timer;
};
