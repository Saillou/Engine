#include "WorldEditor.h"
#include "WorldEditorHelper.hpp"

using namespace WorldEditorHelper;

// -------- Editor --------
WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter() {
    // Change app draw style to be able to reorder entities and compute shadows
    m_scene.directDraw(false);

    // Set menu
    m_menu.reset();

    // Set Scene
    m_scene.lights() = { Light(glm::vec3{ 0, 0, 5 }, glm::vec4{ 1, 0.7, 0.3, 1 }) };

    m_scene.camera().position.z = 0.25f;

    // Create entities
    m_entities["train"] = train();
    m_entities["grass"] = tile_with_texture("Resources/textures/grass.png");
    m_entities["earth"] = tile_with_rgba(glm::vec4(185, 122, 87, 255));

    // - Setup world -
    const int world_size = 200;
    const int n_grasses  = 500;

    // Grid of earth
    for (float x = -world_size/2.0f; x < world_size/2.0f; x++) {
        for (float y = -world_size/2.0f; y < world_size/2.0f; y++) {
            m_entities["earth"].poses().push_back(pose(glm::vec2(x, y)));
        }
    }

    // Random grass
    for (int i = 0; i < n_grasses; i++) {
        float x = (rand() % (100*world_size))/100.0f - world_size/2.0f;
        float y = (rand() % (100*world_size))/100.0f - world_size/2.0f;
        float s = (rand() % 1000) / 3000.0f + 0.2f;
        m_entities["grass"].poses().push_back(pose_scale(glm::vec2(x, y), s));
    }


    // Only one train
    m_entities["train"].poses() = { 
        pose(m_player_data.position)
    };

    // Enable events
    _subscribe(&WorldEditor::_on_key_pressed);

    // Start
    m_timer.tic();
}

void WorldEditor::onExit() { 
    // Put back original app state
    m_scene.directDraw(true);

    m_entities.clear();
    _unsubscribeAll();
}

void WorldEditor::onUpdate() {
    _compute_physics();
    _drawScene();

    m_menu.show();
}

// - Private -
void WorldEditor::_compute_physics() {
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;
    
    // integrate speed
    m_player_data.angle  += m_player_data.angle_speed  * dt_ms;

    // project to postion
    m_player_data.position.x += m_player_data.linear_speed * cos(m_player_data.angle) * dt_ms;
    m_player_data.position.y += m_player_data.linear_speed * sin(m_player_data.angle) * dt_ms;

    // friction
    m_player_data.linear_speed *= 0.99f;
    m_player_data.angle_speed  *= 0.99f;

    // Update model
    m_entities["train"].poses().front() = pose_rot(m_player_data.position, m_player_data.angle);

    m_timer.tic();
}

void WorldEditor::_drawScene() {
    // Set camera
    glm::vec3 train_pos = glm::vec3(pose(m_player_data.position)[3]);
    m_scene.camera().direction = train_pos;
    m_scene.camera().position.x = train_pos.x + m_camera_data.distance * cos(m_player_data.angle);
    m_scene.camera().position.y = train_pos.y + m_camera_data.distance * sin(m_player_data.angle);

    // Draw items
    Renderer& render = m_scene.renderer();

    for (auto& entity : m_entities) {
        render.draw(Render::DrawType::Shadows, entity.second);
    }
}

// Events
void WorldEditor::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    if (evt.action == InputAction::Pressed || evt.action == InputAction::Repeated)
    {
        // Linear speed
        {
            float val = 0.0f;

            switch (evt.key)
            {
                case KeyCode::ArrowUp:   val = -1.0f; break;
                case KeyCode::ArrowDown: val = +1.0f; break;
            }

            if (m_player_data.linear_speed < 1e-2f) {
                m_player_data.linear_speed += 1e-5f * val;
            }
        }

        // Angular speed
        {
            float val = 0.0f;

            switch (evt.key)
            {
                case KeyCode::ArrowLeft:  val = -1.0f; break;
                case KeyCode::ArrowRight: val = +1.0f; break;
            }

            m_player_data.angle_speed += m_player_data.linear_speed * 0.01f * val;
        }

        // Camera
        switch (evt.key)
        {
            case 'Q': m_scene.camera().position.z += 0.001f; break;
            case 'W': m_scene.camera().position.z -= 0.001f; break;
        }
    }
}