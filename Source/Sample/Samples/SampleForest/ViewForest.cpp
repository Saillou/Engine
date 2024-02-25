#include "ViewForest.hpp"

#include <random>
#include <algorithm>

// Random engine
static std::default_random_engine gen;
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

ViewForest::ViewForest() :
    m_fireGrid({
        Model::Load(Model::SimpleShape::Cube),
        size_t(500)
    }),
    m_mousePos(0.0f, 0.0f)
{
    // Populate objects
    m_timer.tic();
    {
        _initObjects();

        m_skybox = std::make_unique<Skybox>(std::array<std::string, 6> {
            "Resources/textures/skybox/right.jpg",
            "Resources/textures/skybox/left.jpg",
            "Resources/textures/skybox/top.jpg",
            "Resources/textures/skybox/bottom.jpg",
            "Resources/textures/skybox/front.jpg",
            "Resources/textures/skybox/back.jpg"
        });
    }
    std::cout << "Objects initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Create particules
    m_timer.tic();
    {
        _initParticles();
    }
    std::cout << "Particules initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Enable events
    _subscribe(&ViewForest::_update);
    _subscribe(&ViewForest::_post_draw);
}

void ViewForest::_update(const CommonEvents::StateUpdated&)
{
    Scene& scene = Service<Window>::get().scene();

    scene.raycaster().update(m_mousePos);
    for (Entity e : scene.raycaster().retrieve()) 
    {
        if (ECS::getComponent<CastComponent>(e).is_hit)
            ECS::getComponent<DrawComponent>(e).type = DrawComponent::DrawType(DrawComponent::Geometry | DrawComponent::Solid);
        else
            ECS::getComponent<DrawComponent>(e).type = DrawComponent::Solid;
    }

    _setParticles(0.16f);
}

void ViewForest::_post_draw(const SceneEvents::PostDraw&) {
    Scene& scene = Service<Window>::get().scene();

    m_skybox->draw(scene.camera);
}

// Allocate static memory
void ViewForest::_initObjects() {
    constexpr float hpi = glm::half_pi<float>();

    // Locomotive
    {
        auto& locomotive = _create_entity(_ObjectId::Locomotive, Model::Load("Resources/objects/train/locomotive.glb"));
        locomotive.castable(true);
        locomotive.local() = glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    hpi, glm::vec3(1, 0, 0)),   // Rotation
                glm::vec3(1.0f, .30f, .0f)),    // Translation
            glm::vec3(2.0f)                     // Scale
        );
    }

    // Trees
    for (int i = 0; i < 100; i++)
    {
        float x = (float)(i % 10 - 5);
        float y = (float)(i / 10 - 5);

        if (abs(x) < 2 && abs(y) < 2) {
            x = (x > 0 ? 1 : -1) * std::max(3.0f, abs(x)) + x / 10.0f;
            y = (y > 0 ? 1 : -1) * std::max(3.0f, abs(y)) + y / 10.0f;
        }

        glm::vec3 tr(-0.90f + x, 0.10f, -0.95f + y);  // Translation

        auto& tree = _create_entity(_ObjectId::Tree, Model::Load("Resources/objects/tree/tree.glb"));
        tree.castable(true);
        tree.local() = glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    hpi, glm::vec3(1, 0, 0)),   // Rotation
                tr),                            // Translation
            glm::vec3(dstr_one(gen) * 3.0f)     // Scale
        );
    }

    // Character
    {
        auto& tortle = _create_entity(_ObjectId::Character, Model::Load("Resources/objects/character/character.glb"));
        tortle.castable(true);
        tortle.local() = glm::translate(
            glm::rotate(glm::mat4(1.0f),  // Identity
                hpi, glm::vec3(1, 0, 0)), // Rotation
            glm::vec3(-1, 0.2f, -1.2f)    // Translation
        );
    }

    // Ground
    for (int i = 0; i < 1024; i++) // 1024 = 32*32
    {
        const glm::vec2 cell_pos = glm::vec2(i % 32 - 16.0f, i / 32 - 16.0f);
        const glm::vec3 T_pos = 0.3f * glm::vec3(cell_pos, 0);
        const glm::vec3 T_scale = 0.3f * glm::vec3(0.5f);

        auto& cell = _create_entity(_ObjectId::Grid, Model::Load(Model::SimpleShape::Quad));
        cell.castable(true);
        cell.local() = glm::scale(glm::translate(glm::mat4(1.0f), T_pos), T_scale);
    }

    // Cube
    {
        auto& cube = _create_entity(_ObjectId::Cube, Model::Load(Model::SimpleShape::Cube));
        cube.castable(true);
        cube.color() = glm::vec4(1.0f, 0.7f, 0.3f, 1);
        cube.local() = glm::scale(
            glm::translate(
                glm::mat4(1.0f), glm::vec3(0.3f, 0, 0.10f)),
            glm::vec3(0.1f)
        );
    }
}

void ViewForest::_initParticles() {
    // Create entities
    const glm::vec3 color(1.0f, 0.7f, 0.3f);

    for (size_t i = 0; i < m_fireGrid.amount; i++) 
    {
        auto spark = ManagedEntity::Create(m_fireGrid.model);

        spark->color() = glm::min(glm::vec4(1.5f * i * color / (float)m_fireGrid.amount, 0) + glm::vec4(1,0,0,1), glm::vec4(1));
        spark->local() = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
        spark->world() = glm::mat4(0.0f);

        m_fireGrid.particles.push_back({spark, glm::vec4(0.0f) });
    }
}

ManagedEntity& ViewForest::_create_entity(_ObjectId id, Model::Ref model)
{
    m_entities[id].push_back(
        ManagedEntity::Create(model)
    );
    return *m_entities[id].back();
}

// Update dynamic memory
void ViewForest::_setParticles(float dt) 
{
    for (int particules_id = 0; particules_id < (int)m_fireGrid.amount; particules_id++)
    {
        glm::vec4& speed = m_fireGrid.particles[particules_id].speed;
        glm::mat4& model = m_fireGrid.particles[particules_id].entity->world();

        const bool hasEnded = model[0][0] < 1e-4f || model[1][1] < 1e-4f || model[2][2] < 1e-4f; // also true for first draw

        if (hasEnded) {
            const int SIZE = (int)sqrt(m_fireGrid.amount);
            int x = particules_id % SIZE - SIZE / 2;
            int y = particules_id / SIZE - SIZE / 2;

            model = glm::translate(glm::mat4(1.0f), 0.01f * glm::vec3(x * 0.7f, 50.0f, 25.0f + y * 0.5f));
            speed = glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen), 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
        }
        else {
            model = glm::scale(glm::translate(model, dt * glm::vec3(speed)), glm::vec3(speed.a));
        }
    }
}

// Callbacks
void ViewForest::mouse_on(int x, int y) {
    Scene& scene = Service<Window>::get().scene();

    m_mousePos.x = (float)x / scene.width();
    m_mousePos.y = (float)y / scene.height();
}

