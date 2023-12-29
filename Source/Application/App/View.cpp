#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>
#include <iostream>

// -- Scene instance --
View::View() :
    BaseScene()
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -10.0f, 0.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // Start
    m_timer.tic();
}

void View::draw() {
    BaseScene::clear();
    float dt_s = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;

    // Draw stuff
    
    // Prepare next
    m_timer.tic();
}
