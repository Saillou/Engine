#include "SampleCanvas.hpp"
#include <Engine/Framework/System/Physic/RayTracer.hpp>

#include <algorithm>

SampleCanvas::SampleCanvas() :
    m_scene(Service<Window>::get().scene())
{
    m_scene.camera.direction = glm::vec3(0, +2, 0);
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

    // Update ui
    m_level_info.set_position(RayTracer::GetScreenPosition(m_scene.camera, m_monster.get_position()));
}

void SampleCanvas::_on_key_pressed(const CommonEvents::KeyPressed& evt) 
{
    // Keyboard mode
    static bool shift_key_on = false;
    if (evt.key == KeyCode::ShiftLeft)
        shift_key_on = evt.action != InputAction::Released;

    // -------------------- Repeated ---------------------
    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    glm::vec2 arrow_move(0.0f, 0.0f);

    switch (evt.key)
    {
        case KeyCode::ArrowLeft:  arrow_move.x = -1.0f; break;
        case KeyCode::ArrowRight: arrow_move.x = +1.0f; break;

        case KeyCode::ArrowUp:   arrow_move.y = -1.0f; break;
        case KeyCode::ArrowDown: arrow_move.y = +1.0f; break;
    }

    // apply
    const float speed = 0.01f;

    glm::vec2 delta = speed * arrow_move;
    m_scene.camera.direction.x += delta.x;
    m_scene.camera.position.x  += delta.x;

    if (shift_key_on) {
        m_scene.camera.direction.y += delta.y;
        m_scene.camera.position.y  += delta.y;
    }
    else {
        m_scene.camera.direction.z += delta.y;
        m_scene.camera.position.z  += delta.y;
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
