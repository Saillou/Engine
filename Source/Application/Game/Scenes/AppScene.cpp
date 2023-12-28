#include "AppScene.hpp"

#include <algorithm>
#include <ctime>
#include <random>

// Random engine
static std::default_random_engine gen;

static std::uniform_real_distribution<float> dstr_pi(-glm::pi<float>(), +glm::pi<float>());
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

// -- Scene instance --
AppScene::AppScene() :
    BaseScene(),
    m_fireGrid({
        glm::vec3(0, 0, 0),
        { 
            size_t(2500),
            std::make_unique<Box>(0.015f * glm::vec3(1.0f, 1.0f, 1.0f))
        }
    })
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -10.0f, 0.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // - Generate batch parameters
    const glm::vec3 color(1.0f, 0.7f, 0.3f);

    // Define particles
    m_fireGrid.particles.models.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.speeds.resize(m_fireGrid.particles.amount);
    m_fireGrid.particles.colors.resize(m_fireGrid.particles.amount);

    std::generate(m_fireGrid.particles.models.begin(), m_fireGrid.particles.models.end(), [&, particules_id = 0]() mutable -> glm::mat4
    {
        const int SIZE = (int)sqrt(m_fireGrid.particles.amount);
        int x = particules_id % SIZE - SIZE / 2;
        int y = particules_id / SIZE - SIZE / 2;
        particules_id++;

        return glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(x * 0.05f, 0.0f, y * 0.05f)
        );
    });

    std::generate(m_fireGrid.particles.colors.begin(), m_fireGrid.particles.colors.end(), [&, particules_id = 0]() mutable -> glm::vec4
    {
        particules_id++;
        float ratio = particules_id / float(m_fireGrid.particles.amount);

        return glm::min(glm::vec4(1.5f * ratio * color, 0.0f) + glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    });

    std::generate(m_fireGrid.particles.speeds.begin(), m_fireGrid.particles.speeds.end(), [&]() -> glm::vec4
    {
        return glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen), 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
    });

    // Create batch
    m_fireGrid.particles.object->createBatch(
        m_fireGrid.particles.colors,
        m_fireGrid.particles.models
    );

    // Cook
    m_fireGrid.particles.object->addRecipe(Cookable::CookType::Batch);
}

void AppScene::draw() {
    // Cleanup previous draws
    BaseScene::clear();

    // Update time
    static float t = 0.0f;
    const float dt = 0.016f;
    
    // Particles
    {
        // Move
        for (int particules_id = 0; particules_id < m_fireGrid.particles.amount; particules_id++)
        {
            glm::vec4& speed = m_fireGrid.particles.speeds[particules_id];
            glm::mat4& model = m_fireGrid.particles.models[particules_id];

            if (model[0][0] < 1e-2f || model[1][1] < 1e-2f || model[2][2] < 1e-2f || redraw)
            {
                const int SIZE = (int)sqrt(m_fireGrid.particles.amount);
                int x = particules_id % SIZE - SIZE / 2;
                int y = particules_id / SIZE - SIZE / 2;

                model = glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(x * 0.05f, 0.0f, y * 0.05f)
                );

                speed = glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen), 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
            }
            else {
                model = glm::scale(
                    glm::translate(
                        model, 
                        m_fireGrid.pos + dt * glm::vec3(speed)), speed.a * glm::vec3(1, 1, 1)
                );
            }
        }

        // Update
        m_fireGrid.particles.object->updateBatch(
            m_fireGrid.particles.colors,
            m_fireGrid.particles.models
        );

        // Draw
        m_fireGrid.particles.object->drawBatch(m_fireGrid.particles.amount, m_camera);
    }

    // Update
    t += dt;
    redraw = false;
}
