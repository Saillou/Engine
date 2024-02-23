#include "SampleTrain.hpp"
#include "HelperTrain.hpp"

static const struct _all_magic_constants_ 
{
    // - World -
    const int Ground_Size       = 50;

    const int   Grass_Number    = 500;
    const float Grass_Min_Scale = 0.1f;
    const float Grass_Max_Scale = 2.0f;

    // - Physics -
    const float Friction_Ground = 0.97f;
    const float Friction_Air    = 0.95f;

    // - Player -
    const float Accel_Linear  = 1e-4f;
    const float Accel_Angular = 1e-2f;

    const float Speed_Linear_Max = 5e-3f;
} Ctx;

// Helper functions
using namespace HelperTrain;

// -------- Editor --------
SampleTrain::SampleTrain() :
    m_scene(Service<Window>::get().scene())
{
    m_scene.lights = { 
        Light(glm::vec3{ 0, 0, 5 }, glm::vec4{ 1, 0.7, 0.3, 1 }) 
    };

    // Create entities
    m_entities["train"].push_back(train());
    ECS::getComponent<BodyComponent>(m_entities["train"].back()).transform.world = pose(m_player_data.position);

    // Earth
    m_entities["earth"].push_back(tile_with_rgba(glm::vec4(185, 122, 87, 255)));
    ECS::getComponent<BodyComponent>(m_entities["earth"].back()).transform.world = pose_scale(glm::vec2(0, 0), Ctx.Ground_Size);

    // Random grass
    for (int i = 0; i < Ctx.Grass_Number; i++) {
        float x = (rand() % (100*Ctx.Ground_Size))/100.0f - Ctx.Ground_Size /2.0f;
        float y = (rand() % (100*Ctx.Ground_Size))/100.0f - Ctx.Ground_Size /2.0f;
        float s = (Ctx.Grass_Max_Scale - Ctx.Grass_Min_Scale) * (rand() % 1000) / 1000.0f + Ctx.Grass_Min_Scale;
        m_entities["grass"].push_back(tile_with_texture("Resources/textures/grass.png"));
        ECS::getComponent<BodyComponent>(m_entities["grass"].back()).transform.world = pose_scale(glm::vec2(x, y), s);
    }

    // Enable events
    _subscribe(&SampleTrain::_on_update);
    _subscribe(&SampleTrain::_on_key_pressed);
    _subscribe(&SampleTrain::_on_scene_end);

    // Start
    m_timer.tic();
}

SampleTrain::~SampleTrain()
{
    for (auto v_entity : m_entities) {
        for (auto entity : v_entity.second) {
            ECS::destroyEntity(entity);
        }
    }
}

// Events
void SampleTrain::_on_update(const CommonEvents::StateUpdated&) {
    _compute_physics();
    _update_camera();
}

void SampleTrain::_on_scene_end(const SceneEvents::SceneFinished&)
{
    _drawInfo();
}

void SampleTrain::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    // Linear speed
    {
        float val = 0.0f;
        switch (evt.key) {
            case KeyCode::ArrowUp:   val = -1.0f; break;
            case KeyCode::ArrowDown: val = +1.0f; break;
        }
        if (std::abs(m_player_data.linear_speed) < Ctx.Speed_Linear_Max) {
            m_player_data.linear_speed += Ctx.Accel_Linear * val;
        }
    }

    // Angular speed
    {
        float val = 0.0f;
        switch (evt.key) {
            case KeyCode::ArrowLeft:  val = -1.0f; break;
            case KeyCode::ArrowRight: val = +1.0f; break;
        }
        m_player_data.angle_speed += m_player_data.linear_speed * Ctx.Accel_Angular * val;
    }
}

// - Private -
void SampleTrain::_compute_physics() {
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;
    
    // integrate speed
    m_player_data.angle  += m_player_data.angle_speed  * dt_ms;

    // project to postion
    m_player_data.position.x += m_player_data.linear_speed * cos(m_player_data.angle) * dt_ms;
    m_player_data.position.y += m_player_data.linear_speed * sin(m_player_data.angle) * dt_ms;

    // friction
    m_player_data.linear_speed *= Ctx.Friction_Ground;
    m_player_data.angle_speed  *= Ctx.Friction_Air;

    // Update model
    ECS::getComponent<BodyComponent>(m_entities["train"].back()).transform.world = pose_rot(m_player_data.position, m_player_data.angle);

    m_timer.tic();
}

void SampleTrain::_update_camera() {
    // Set camera
    m_camera_data.distance = 1.0f - m_player_data.linear_speed / Ctx.Speed_Linear_Max;

    const glm::vec3 train_pos = glm::vec3(pose(m_player_data.position)[3]);

    m_scene.camera.direction = train_pos;
    m_scene.camera.position.x = train_pos.x + m_camera_data.distance * cos(m_player_data.angle);
    m_scene.camera.position.y = train_pos.y + m_camera_data.distance * sin(m_player_data.angle);
    m_scene.camera.position.z = 0.25f * (1.0f - m_player_data.linear_speed/Ctx.Speed_Linear_Max);
}

void SampleTrain::_drawInfo() {
    m_menu.createSection("Camera");
    m_menu.addContent("Camera", "position",  m_scene.camera.position);
    m_menu.addContent("Camera", "direction", m_scene.camera.direction);

    m_menu.createSection("Player");
    m_menu.addContent("Player", "position", m_player_data.position);
    m_menu.addContent("Player", "speed",    m_player_data.linear_speed);
    m_menu.addContent("Player", "angle",    int(180.0f * m_player_data.angle / glm::pi<float>()) % 360);

    m_menu.show();
}
