#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>

#include <sstream>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Engine/Events/Events.hpp"
#include "Engine/Events/CommonEvents.hpp"

#include <Engine/Utils/RayCaster.hpp>

#include "TrainGame/Engine/Core/ECS.h"
#include "TrainGame/Engine/Components/Transform2.h"
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

        m_renderSystem = Thomas::ECS::registerSystem<RenderSystem>();
        {
            Thomas::Signature signature;
            signature.set(Thomas::ECS::getComponentType<Thomas::Transform2>());
            signature.set(Thomas::ECS::getComponentType<Thomas::RenderComponent>());

            Thomas::ECS::setSystemSignature<RenderSystem>(signature);
            m_renderSystem->init();
        }

        // Camera
        scene.camera().position = glm::vec3(0.f, -3.5f, 8.0f);
        scene.camera().direction = glm::vec3(0.0f, 0.0, 0.0f);

        // Lightnings
        scene.lights() = {
            Light(glm::vec3{  0,  0, 0.50f }, glm::vec4{ 1, 0.7, 0.3, 1 })
        };

        loadModels(6);

        // Populate objects
        m_timer.tic();
        {
            _initObjects();
        }
        std::cout << "Objects initialized in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;
    }

    void View::_post_process(const SceneEvents::PostDraw& evt)
    {
        scene.framebuffer_main().bind();
        {
            m_skybox->draw(scene.camera());
        }
        scene.framebuffer_main().unbind();        
    }

    void View::_on_resize(const SceneEvents::Resized& evt)
    {

    }

    void View::_draw(const SceneEvents::Draw& evt) {
        float dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
        m_timer.tic();

        auto& renderer = scene.renderer();

        if (m_renderSystem)
            m_renderSystem->update(renderer);
        // Main scene
        {
            // Lights
            for (auto& light : scene.lights())
            {
                m_model_sphere->localMaterial().diffuse_color = light.color;
                m_model_sphere->poses() = { glm::scale(glm::translate(glm::mat4(1.0f), light.position),  glm::vec3(0.1f)) };
                renderer.draw(Render::DrawType::Basic, *m_model_sphere);
            }

            // Draw game objects
            for (auto& model : m_modelsToDraw)
            {
                const Thomas::GameModel& gameModel = Thomas::GameModelTable::getModelById(model.first);

                for (auto& t : model.second)
                {
                    glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
                    localTransform = glm::rotate(localTransform, gameModel.transform.rotation.x, glm::vec3(1, 0, 0));
                    localTransform = glm::rotate(localTransform, gameModel.transform.rotation.y, glm::vec3(0, 1, 0));
                    localTransform = glm::rotate(localTransform, gameModel.transform.rotation.z, glm::vec3(0, 0, 1));
                    localTransform = glm::translate(localTransform, gameModel.transform.position);
                    localTransform = glm::scale(localTransform, gameModel.transform.scale);

                    glm::mat4 worldTransform = glm::translate(glm::mat4(1.0f), t.position);
                    worldTransform = glm::rotate(worldTransform, t.rotation.x, glm::vec3(1, 0, 0));
                    worldTransform = glm::rotate(worldTransform, t.rotation.y, glm::vec3(0, 1, 0));
                    worldTransform = glm::rotate(worldTransform, t.rotation.z, glm::vec3(0, 0, 1));
                    worldTransform = glm::scale(worldTransform, t.scale);

                    if (model.first != Thomas::ModelId::CubeGeometry && model.first != Thomas::ModelId::CubeBasic)
                    {
                        m_gameModels[model.first]->poses() = { worldTransform * localTransform };
                        renderer.draw(Render::DrawType::Shadows, *m_gameModels[model.first]);

                        //m_gameModels[model.first]->drawOne(Cookable::CookType::Basic, m_camera, worldTransform * localTransform, m_lights);
                    }
                    else
                    {
                        if (model.first == Thomas::ModelId::CubeGeometry)
                        {
                            m_gameModels[model.first]->localMaterial().diffuse_color = {1,1,1,1};
                            m_gameModels[model.first]->poses() = { worldTransform * localTransform };
                            renderer.draw(Render::DrawType::Geometry, *m_gameModels[model.first]);
                        }
                        else
                        {
                            m_gameModels[model.first]->localMaterial().diffuse_color = { 0.3,0.6,0.3,1 };
                            m_gameModels[model.first]->poses() = { worldTransform * localTransform };
                            renderer.draw(Render::DrawType::Basic, *m_gameModels[model.first]);
                        }
                    }

                    // draw debug center
                    m_model_sphere->localMaterial().diffuse_color = {0.7f, 0.5f, 0.f, 1.f};
                    m_model_sphere->poses() = { glm::scale(glm::translate(glm::mat4(1.0f), t.position),  glm::vec3(0.05f)) };
                    renderer.draw(Render::DrawType::Basic, *m_model_sphere);
                }
            }

            auto cast_res = RayCaster::Intersect(m_mousePos, scene.camera(), *m_groundEntity.entity, m_groundEntity.transform.getMat4());

            if (cast_res.has_value())
            {
                Event::Emit(CommonEvents::MouseHit(cast_res.value().x, cast_res.value().y, 0.05f));

                m_target->localMaterial().diffuse_color = { 1.f, 1.f, 1.f, 1.f };
                m_target->poses() = { glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(cast_res.value())), glm::vec3(0.1f, 0.1f, 0.01f)) };
                renderer.draw(Render::DrawType::Basic, *m_target);
            }

            m_groundEntity.entity->localMaterial().diffuse_color = { 1.f, 1.f, 1.f, 1.f };
            m_groundEntity.entity->poses() = { m_groundEntity.transform.getMat4()};

            renderer.draw(Render::DrawType::Shadows, *m_groundEntity.entity);

            drawGrid();
        }

        m_modelsToDraw.clear();

        // Prepare next
            m_timer.tic();
        }


    void View::loadModels(size_t size)
    {
        m_gameModels.reserve(size);

        // TODO: get the list of models we want to load from somewhere
        // example: we do not want to store all game models here, only for the current level
        // 
        // Misha: can this part be async at some point ?
        m_timer.tic();
        {
            m_gameModels[Thomas::ModelId::Locomotive] = std::make_unique<Entity>("Resources/objects/train/locomotive.glb");
            m_gameModels[Thomas::ModelId::Wagon] = std::make_unique<Entity>("Resources/objects/train/wagon_no_wheels.glb");
            m_gameModels[Thomas::ModelId::Track] = std::make_unique<Entity>("Resources/objects/train/track_forward.glb");
            m_gameModels[Thomas::ModelId::TrackLeft] = std::make_unique<Entity>("Resources/objects/train/track_left.glb");
            m_gameModels[Thomas::ModelId::TrackRight] = std::make_unique<Entity>("Resources/objects/train/track_right.glb");
            m_gameModels[Thomas::ModelId::Building1] = std::make_unique<Entity>("Resources/objects/train/building_1.glb");
            m_gameModels[Thomas::ModelId::CubeBasic] = std::make_unique<Entity>(Entity::SimpleShape::Cube);
            m_gameModels[Thomas::ModelId::CubeGeometry] = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        }
        std::cout << "Game models loaded in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;
    }

    void View::draw(Thomas::ModelId id, const Thomas::Transform& transform)
    {
        m_modelsToDraw[id].push_back(transform);
    }

    void View::drawGrid(const std::map<std::pair<int, int>, Thomas::GridCell>& cells)
    {
        // TODO: optimize this part please (veri bad performance)
        // TODO: add Geometry as a second batch

        m_gridCells.clear();
        m_gridCells = cells;

        m_entityGridCells.cells = cells;

        std::vector<Pose> mats;
        std::vector<Material> colors;

        mats.reserve(cells.size());
        colors.reserve(cells.size());

        for (auto& c : m_entityGridCells.cells)
        {
            Material color = { {0,0,0,0}, false  };
            switch (c.second.type)
            {
            case Thomas::GridCell::CellType::Visible:
                color.diffuse_color = {0.7f,0.6f,0.3f,0.5f};
                break;
            case Thomas::GridCell::CellType::ConstructOk:
                color.diffuse_color = { 0.3f,0.6f,0.3f,0.5f };
                break;
            case Thomas::GridCell::CellType::ConstructBad:
                color.diffuse_color = { 0.7f,0.3f,0.3f,0.5f };
                break;
            }

            colors.push_back(color);
            mats.push_back(c.second.transform.getMat4());
        }

        m_entityGridCells.entity->setPosesWithMaterials(mats, colors);
    }

    void View::drawGrid()
    {
        scene.renderer().draw(Render::Basic, *m_entityGridCells.entity);
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

        // Grid cells
        m_entityGridCells.entity = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        m_entityGridCells.cells.clear();

        // Target
        m_target = std::make_unique<Entity>(Entity::SimpleShape::Sphere);

        // Ground - Grid
        m_groundEntity.transform = { {0.5f,0.5f,-0.1f}, {3.f, 3.f, 0.12f}, {0,0,0} };
        m_groundEntity.entity = std::make_unique<Entity>(Entity::SimpleShape::Cube);

        // Lanterns
        m_model_sphere = std::make_unique<Entity>(Entity::Sphere);

        m_model = std::make_unique<Entity>("Resources/objects/train/wagon_no_wheels.glb");

        // GameObjects:
        m_gameObjects.push_back(std::make_pair<Thomas::ModelId, glm::vec3>(Thomas::ModelId::Locomotive, { 0,0,0 }));
        m_gameObjects.push_back(std::make_pair<Thomas::ModelId, glm::vec3>(Thomas::ModelId::Locomotive, { 1,1,1 }));
        m_gameObjects.push_back(std::make_pair<Thomas::ModelId, glm::vec3>(Thomas::ModelId::Wagon, { 2,2,2 }));
    }