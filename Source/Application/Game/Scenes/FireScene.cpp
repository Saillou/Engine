#include "FireScene.hpp"

#include <algorithm>
#include <ctime>
#include <random>
#include <array>

// Random engine
const size_t WIDTH = 150;
const size_t HEIGHT = 100;
const float PIX_SIZE = 0.007f;

static std::default_random_engine gen;

static std::uniform_real_distribution<float> dstr_pi(-glm::pi<float>(), +glm::pi<float>());
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

// -- perlin noise --
static float _noise(float x, float y) {
    // Helpers
    static const auto scaled_cosine = [=](float val) {
        return 0.5f * (1.0f - std::cos(val * glm::pi<float>()));
    };

    // Parameters
    static bool init = false;
    static const size_t PERLIN_SIZE = 4095;
    static std::array<float, PERLIN_SIZE+1> perlin;
    if (!init) {
        std::generate(perlin.begin(), perlin.end(), [&]() {  return dstr_one(gen); });
        init = true;
    }
    const int PERLIN_YWRAPB = 4;
    const int PERLIN_YWRAP  = 1 << PERLIN_YWRAPB;
    const int PERLIN_ZWRAPB = 8;
    const int PERLIN_ZWRAP  = 1 << PERLIN_ZWRAPB;

    const int perlin_octaves = 4;
    const float perlin_amp_falloff = 0.5f;

    // Args
    int xi   = (int)std::floor(x), yi = (int)std::floor(y);
    float xf = x - xi,             yf = y - yi;
    float rxf, ryf;

    float r     = 0.0f;
    float ampl  = 0.5f;
    float n1    = 0.0f, n2 = 0.0f, n3 = 0.0f;

    // Get noise
    for (int o = 0; o < perlin_octaves; o++) {
        int of = xi + (yi << PERLIN_YWRAPB);

        rxf = scaled_cosine(xf);
        ryf = scaled_cosine(yf);

        n1 = perlin[of & PERLIN_SIZE];
        n1 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n1);

        n2 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
        n2 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n2);

        n1 += ryf * (n2 - n1);

        of += PERLIN_ZWRAP;

        n2 = perlin[of & PERLIN_SIZE];
        n2 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n2);
        n3 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
        n3 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n3);
        n2 += ryf * (n3 - n2);

        r += n1 * ampl;
        ampl *= perlin_amp_falloff;
        xi <<= 1;
        xf *= 2;
        yi <<= 1;
        yf *= 2;

        if (xf >= 1.0) {
            xi++;
            xf--;
        }
        if (yf >= 1.0) {
            yi++;
            yf--;
        }
    }
    return r;
};

// -- Scene instance --
FireScene::FireScene() :
    BaseScene(),
    m_fireBall({
        glm::vec3(0, 0, 0),
        { 
            size_t(WIDTH * HEIGHT),
            std::make_unique<Box>(PIX_SIZE* glm::vec3(1.0f, 1.0f, 1.0f))
        }
    })
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -3.0f, 0.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // - Generate batch parameters
    const glm::vec3 color(1.0f, 0.7f, 0.3f);

    // Define particles
    m_fireBall.particles.models.resize(m_fireBall.particles.amount);
    m_fireBall.particles.speeds.resize(m_fireBall.particles.amount);
    m_fireBall.particles.colors.resize(m_fireBall.particles.amount);

    // positions
    std::generate(m_fireBall.particles.models.begin(), m_fireBall.particles.models.end(), [&, particules_id = 0]() mutable -> glm::mat4
    {
        return glm::translate(glm::mat4(1.0f), _getParticulePos(++particules_id));
    });

    // colors
    std::generate(m_fireBall.particles.colors.begin(), m_fireBall.particles.colors.end(), [&, particules_id = 0]() mutable -> glm::vec4
    {
        particules_id++;
        float power = particules_id / float(m_fireBall.particles.amount);
        return _getBrightness(power);
    });

    // speeds
    std::generate(m_fireBall.particles.speeds.begin(), m_fireBall.particles.speeds.end(), [&]() -> glm::vec4
    {
        return _getRandomSpeed();
    });


    // Create batch
    m_fireBall.particles.object->createBatch
    (
        m_fireBall.particles.colors,
        m_fireBall.particles.models
    );

    // Cook
    m_fireBall.particles.object->addRecipe(Cookable::CookType::Batch);
}

void FireScene::draw() {
    // Cleanup previous draws
    BaseScene::clear();

    // Update time
    static float t = 0.0f;
    const float dt = 0.016f;

    // Particles
    {
        float x_off = 0.0f;
        
        for (int x = 0; x < WIDTH; x++) {
            x_off += dt;
            float y_off = t;

            for (int y = 0; y < HEIGHT; y++) {
                y_off += dt;

                size_t particules_id = x + y * WIDTH;

                glm::mat4& model = m_fireBall.particles.models[particules_id];
                glm::vec4& color = m_fireBall.particles.colors[particules_id];
                glm::vec4& speed = m_fireBall.particles.speeds[particules_id];

                color = _getBrightness((float)std::pow(_noise(x_off, y_off), 3));

                if (model[0][0] < 1e-2f || model[1][1] < 1e-2f || model[2][2] < 1e-2f)
                {
                    model = glm::translate(
                        glm::mat4(1.0f),
                        m_fireBall.pos + _getParticulePos(particules_id)
                    );

                    speed = _getRandomSpeed();
                }
                else {
                    model = glm::scale(
                        glm::translate(
                            model,
                            dt*glm::vec3(speed)), 
                        speed.a * glm::vec3(1, 1, 1)
                    );
                }
            }
        }

        // Update
        m_fireBall.particles.object->updateBatch
        (
            m_fireBall.particles.colors,
            m_fireBall.particles.models
        );

        // Draw
        m_fireBall.particles.object->drawBatch(m_fireBall.particles.amount, m_camera);
    }

    // Update
    t += dt;
}

void FireScene::move_ball(const glm::vec3& delta_position) {
    m_fireBall.pos += delta_position;
}

glm::vec4 FireScene::_getBrightness(float ratio) const {
    return glm::min (
        glm::vec4(1.5f * ratio * _getColor()) + glm::vec4(1,0,0,0),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
}

glm::vec4 FireScene::_getColor() const {
    return glm::vec4(1.0f, 0.7f, 0.3f, 0.7f);
}

glm::vec3 FireScene::_getParticulePos(size_t particule_id) const {
    int s = int(particule_id % WIDTH - WIDTH / 2);
    int t = int(particule_id / WIDTH - HEIGHT / 2);
    float r = 6.0f;

    double x = r * cos(s) * sin(t);
    double y = r * sin(s) * sin(t);
    double z = r * cos(t);

    return 1.5f * PIX_SIZE * glm::vec3(x, y, z);
}

glm::vec4 FireScene::_getRandomSpeed() const {
    return glm::vec4(dstr_half(gen) / 2.0f, 0.0f, dstr_one(gen) / 4.0f, 1.0f - dstr_one(gen) / 10.0f - 1e-2f);
}
