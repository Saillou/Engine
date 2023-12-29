#pragma once

#include <vector>
#include <memory>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>

#include <Engine/Utils/Timer.hpp>

struct View : public BaseScene {
    View();

    void draw() override;

private:
    Timer::Chronometre m_timer;
};
