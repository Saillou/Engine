#include "SampleCanvas.hpp"

#include <algorithm>

SampleCanvas::SampleCanvas() :
    m_scene(Service<Window>::get().scene())
{
    m_scene.camera.direction = glm::vec3(0, 2, 0);
    m_scene.camera.position  = glm::vec3(0, -5, 3);

    // Enable events
    _subscribe(&SampleCanvas::_on_key_pressed);
    _subscribe(&SampleCanvas::_update);

    m_timer.tic();
}

void SampleCanvas::_generateFood()
{
    m_foods.push_back(std::make_shared<Food>());
}

void SampleCanvas::_consumeFood(Entity foodId)
{
    m_monster.earn_xp(10);

    m_foods.erase(std::remove_if(m_foods.begin(), m_foods.end(), [foodId](std::shared_ptr<Food> food) {
        return food->id() == foodId;
    }), m_foods.end());
}

// - Events -
void SampleCanvas::_update(const CommonEvents::StateUpdated&)
{
    // Tic
    float dt_s = m_timer.elapsed<Timer::millisecond>() / 1000.0f;
    m_timer.tic();

    // Make food fall
    for (std::shared_ptr<Food> food : m_foods) {
        food->fall(dt_s);
    }

    // Check collisions
    std::vector<Entity> food_to_consume;

    for (std::shared_ptr<Food> food : m_foods) {
        m_scene.collider().check(food->id());

        if (!food->is_colliding())
            continue;

        food_to_consume.push_back(food->id());
    }

    // Consume
    for (Entity foodId : food_to_consume) {
        _consumeFood(foodId);
    }
}

void SampleCanvas::_on_key_pressed(const CommonEvents::KeyPressed& evt) 
{
    // -------------------- Repeated ---------------------
    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    const float MIN_CAM_X = -5.0f;
    const float MAX_CAM_X = +5.0f;

    float move_x  = 0.0f;
    float speed_x = 0.01f;

    switch (evt.key)
    {
        case KeyCode::ArrowLeft:  move_x = -1.0f; break;
        case KeyCode::ArrowRight: move_x = +1.0f; break;
    }

    float next_x = m_scene.camera.direction.x + speed_x * move_x;
    if (next_x < MAX_CAM_X && next_x > MIN_CAM_X)
    {
        m_scene.camera.direction.x = next_x;
        m_scene.camera.position.x  = next_x;
    }

    // -------------------- Pressed ---------------------
    if (evt.action != InputAction::Pressed)
        return;

    switch (evt.key)
    {
        case KeyCode::Space: _generateFood(); break;
        case KeyCode::Escape: wantQuit = true; break;
    }
}
