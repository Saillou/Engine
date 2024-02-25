#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>
#include <sstream>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Framework/System/Physic/RayCaster.hpp>
#include <Engine/Framework/Core/ECS.hpp>

#include "TrainGame/App/GameEvents.hpp"
#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/RenderComponent.h"

// Random engine
static std::default_random_engine gen;

static std::uniform_real_distribution<float> dstr_pi(-glm::pi<float>(), +glm::pi<float>());
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

View::View(Scene& scene) :
    scene(scene)
{
    // Root events
    _subscribe(&View::_draw);
    _subscribe(&View::_on_resize);
    _subscribe(&View::_post_process);
    _subscribe(&View::_on_mouse_moved);

    // Camera
    scene.camera.position = glm::vec3(0.f, -3.5f, 8.0f);
    scene.camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Lightnings
    scene.lights = {
        Light(glm::vec3{  0,  0, 0.50f }, glm::vec4{ 1, 0.7, 0.3, 1 })
    };

    // Populate objects
    m_timer.tic();
    {
        _initObjects();
    }
    std::cout << "Objects initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;
}

View::~View()
{
    ECS::destroyEntity(m_model_sphere);
    ECS::destroyEntity(m_target);
    ECS::destroyEntity(m_ground);
}

void View::_post_process(const SceneEvents::PostDraw& evt)
{
    m_skybox->draw(scene.camera);
}

void View::_on_resize(const SceneEvents::Resized& evt)
{

}

void View::_draw(const CommonEvents::StateUpdated& evt) {
    float dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
    m_timer.tic();

    // Light
    {
        const Light& light = scene.lights.front();

        BodyComponent& body = ECS::getComponent<BodyComponent>(m_model_sphere);
        body.transform.world = glm::translate(glm::mat4(1.0f), light.position);
        body.material.color  = light.color;
    }

    // Cast
    {
        BodyComponent& ground = ECS::getComponent<BodyComponent>(m_ground);
        auto cast_res = RayCaster::Intersect(m_mousePos, scene.camera, ground.model, ground.transform.get());

        if (cast_res.has_value())
        {
            Event::Emit(GameEvents::MouseHit(cast_res.value().x, cast_res.value().y, 0.05f));

            BodyComponent& body = ECS::getComponent<BodyComponent>(m_target);
            body.transform.world = glm::translate(glm::mat4(1.0f), glm::vec3(cast_res.value()));
        }
    }

    // Prepare next
    m_timer.tic();
}

void View::_on_mouse_moved(const CommonEvents::MouseMoved& evt)
{
    m_mousePos.x = (float)evt.x / scene.width();
    m_mousePos.y = (float)evt.y / scene.height();
}

void View::_initObjects() {
    // Sky
    m_skybox = std::make_unique<Skybox>(std::array<std::string, 6> {
        "Resources/textures/skybox/right.jpg",
        "Resources/textures/skybox/left.jpg",
        "Resources/textures/skybox/top.jpg",
        "Resources/textures/skybox/bottom.jpg",
        "Resources/textures/skybox/front.jpg",
        "Resources/textures/skybox/back.jpg"
    });

    // Target
    m_target = ECS::createEntity();
    {
        ParticleComponent particle;
        particle.type = ParticleComponent::Type::Solid;
        particle.elements = { ParticleComponent::Element{glm::vec4(1.0f),glm::mat4(1.0f)} };

        BodyComponent body;
        body.model = Model::Load(Model::SimpleShape::Sphere);
        body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.01f));

        ECS::addComponent(m_target, body);
        ECS::addComponent(m_target, particle);
    }

    // Ground - Grid
    m_ground = ECS::createEntity();
    {
        DrawComponent draw;
        draw.type = DrawComponent::Solid;

        BodyComponent body;
        body.model = Model::Load(Model::SimpleShape::Cube);
        body.material = { 1.f, 1.f, 1.f, 1.f };
        body.transform.world = { Thomas::Transform({ {0.5f,0.5f,-0.1f}, {3.f, 3.f, 0.12f}, {0,0,0} }).getMat4() };

        ECS::addComponent(m_ground, body);
        ECS::addComponent(m_ground, draw);
    }

    // Lanterns
    m_model_sphere = ECS::createEntity();
    {
        ParticleComponent particle;
        particle.type = ParticleComponent::Type::Solid;
        particle.elements = { ParticleComponent::Element{glm::vec4(1.0f),glm::mat4(1.0f)} };

        BodyComponent body;
        body.model = Model::Load(Model::SimpleShape::Sphere);
        body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        ECS::addComponent(m_model_sphere, body);
        ECS::addComponent(m_model_sphere, particle);
    }
}