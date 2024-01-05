#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>

View::View(int widthHint, int heightHint) :
    BaseScene(widthHint, heightHint)
{
    // Camera
    m_camera.position = glm::vec3(0.0f, -6.0f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Lightnings
    m_lights = {
        Light(glm::vec3{  0,  -1 , 0 }, glm::vec4{ 1,1,1,1 })
    };

    // Populate objects
    m_timer.tic();
    {
        _initObjects();
    }
    std::cout << "Objects initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;
}

void View::draw() {
    float dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();

    BaseScene::_update_camera();
    BaseScene::clear();

    /*glm::mat4 quat_play1 = glm::translate(glm::mat4(1.0f), _player1_pos);
    glm::mat4 quat_play2 = glm::translate(glm::mat4(1.0f), _player2_pos);
    glm::mat4 quat_ball = glm::translate(glm::mat4(1.0f), _ball_pos);*/

    for (auto& model : _models)
    {
        std::shared_ptr<BaseShape> shape = nullptr;
        for (auto& pos : model.second) {
            glm::mat4 quat = glm::translate(glm::mat4(1.0f), pos);

            switch (model.first)
            {
            case 0: _modelCharacter->draw(m_camera, quat, m_lights); break;
            case 1: _modelBall->draw(m_camera, quat, m_lights); break;
            }
        }
        
        
    }
    /*_modelCharacter->draw(m_camera, quat_play1, m_lights);
    _modelCharacter->draw(m_camera, quat_play2, m_lights);
    _modelBall->draw(m_camera, quat_ball, m_lights);*/
    
    // Some static texts
    TextEngine::Write("Score 0 - 0", 50, 50, 1.0f, glm::vec3(1, 1, 1));

    _models.clear();
    // Prepare next
    float dt_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();
}

void View::drawObject(const glm::vec3& pos, int type)
{
    // do it better in the future please
    _models[type].push_back(pos);
}

void View::_initObjects() {
    _modelCharacter  = std::make_shared<Box>(0.1f*glm::vec3(2.0f, 0.5f, 0.5f));
    _modelCharacter->addRecipe(Cookable::CookType::Solid, glm::vec4(1, 1, 1, 1));

    _modelBall       = std::make_shared<Sphere>(0.1f);
    _modelBall->addRecipe(Cookable::CookType::Solid, glm::vec4(1, 1, 1, 1));
}
