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

namespace Thomas
{

    // Random engine
    static std::default_random_engine gen;

    static std::uniform_real_distribution<float> dstr_pi(-glm::pi<float>(), +glm::pi<float>());
    static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);
    static std::uniform_real_distribution<float> dstr_half(-0.5f, +0.5f);

    View::View(int widthHint, int heightHint) :
        BaseScene(widthHint, heightHint)
    {
        _subscribe(&View::_on_mouse_moved);

        // Camera
        m_camera.position = glm::vec3(0.f, -3.5f, 8.0f);
        m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

        // Lightnings
        m_lights = {
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

    void View::draw() {
        float dt_since_last_draw = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;
        m_timer.tic();

        // Main scene
        BaseScene::Viewport(width(), height());
        BaseScene::clear();
        {
            // Lights
            for (auto& light : m_lights) {
                m_model_sphere->get(Cookable::CookType::Basic)->use().set("diffuse_color", light.color);
                m_model_sphere->drawOne(Cookable::CookType::Basic, m_camera, glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(0.1f)));
            }

            // Draw game objects
            for (auto& model : m_modelsToDraw)
            {
                const GameModel& gameModel = GameModelTable::getModelById(model.first);

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

                    if (model.first != ModelId::CubeGeometry && model.first != ModelId::CubeBasic)
                    {
                        m_gameModels[model.first]->drawOne(Cookable::CookType::Basic, m_camera, worldTransform * localTransform, m_lights);
                    }
                    else
                    {
                        if (model.first == ModelId::CubeGeometry)
                        {
                            m_gameModels[model.first]->get(Cookable::CookType::Geometry)->use().set("diffuse_color", { 1,1,1,1 });
                            m_gameModels[model.first]->drawOne(Cookable::CookType::Geometry, m_camera, worldTransform * localTransform);
                        }
                        else
                        {
                            m_gameModels[model.first]->get(Cookable::CookType::Basic)->use().set("diffuse_color", { 0.3,0.6,0.3,1 });
                            m_gameModels[model.first]->drawOne(Cookable::CookType::Basic, m_camera, worldTransform * localTransform);
                        }
                    }

                    // draw debug center
                    m_model_sphere->drawOne(Cookable::CookType::Basic, m_camera, glm::scale(glm::translate(glm::mat4(1.0f), t.position), glm::vec3(0.05f)));
                    m_model_sphere->drawOne(Cookable::CookType::Geometry, m_camera, glm::scale(glm::translate(glm::mat4(1.0f), t.position), glm::vec3(0.05f)));
                }
            }
            
            auto cast_res = RayCaster::Intersect(m_mousePos, m_camera, *m_groundEntity.entity, m_groundEntity.transform.getMat4());

            if (cast_res.has_value())
            {
                Event::Emit(CommonEvents::MouseHit(cast_res.value().x, cast_res.value().y, 0.05f));

                m_target->get(Cookable::CookType::Basic)->use().set("diffuse_color", glm::vec4(1, 1, 1, 1));
                m_target->drawOne(Cookable::CookType::Basic, m_camera, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(cast_res.value())), glm::vec3(0.1f, 0.1f, 0.01f)));
            }
            m_groundEntity.entity->drawBasic(m_camera, m_lights);
            

            drawGrid();

            // Skybox
            m_skybox->draw(m_camera);
        }

        // Some static texts
        std::ostringstream ss;
        ss << "x: " << m_lights[0].position.x << ", z: " << m_lights[0].position.z;
        std::string s(ss.str());

        TextEngine::Write(s, 50, 50, 1.0f, glm::vec3(1, 1, 1));

        m_modelsToDraw.clear();

        Event::Emit(CommonEvents::SceneFinishedRender());

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
            m_gameModels[ModelId::Locomotive] = std::make_unique<Entity>("Resources/objects/train/locomotive.glb");
            m_gameModels[ModelId::Wagon] = std::make_unique<Entity>("Resources/objects/train/wagon_no_wheels.glb");
            m_gameModels[ModelId::Track] = std::make_unique<Entity>("Resources/objects/train/track_forward.glb");
            m_gameModels[ModelId::TrackLeft] = std::make_unique<Entity>("Resources/objects/train/track_left.glb");
            m_gameModels[ModelId::TrackRight] = std::make_unique<Entity>("Resources/objects/train/track_right.glb");
            m_gameModels[ModelId::Building1] = std::make_unique<Entity>("Resources/objects/train/building_1.glb");
            m_gameModels[ModelId::CubeBasic] = std::make_unique<Entity>(Entity::SimpleShape::Cube);
            m_gameModels[ModelId::CubeGeometry] = std::make_unique<Entity>(Entity::SimpleShape::Cube);
        }
        std::cout << "Game models loaded in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;
    }

    void View::draw(ModelId id, const Transform& transform)
    {
        m_modelsToDraw[id].push_back(transform);
    }

    void View::drawGrid(const std::map<std::pair<int, int>, GridCell>& cells)
    {
        // TODO: optimize this part please (veri bad performance)
        // TODO: add Geometry as a second batch

        m_gridCells.clear();
        m_gridCells = cells;

        m_entityGridCells.cells = cells;

        std::vector<glm::mat4> mats;
        std::vector<glm::vec4> colors;

        mats.reserve(cells.size());
        colors.reserve(cells.size());

        for (auto& c : m_entityGridCells.cells)
        {
            glm::vec4 color = {0,0,0,0};
            switch (c.second.type)
            {
            case GridCell::CellType::Visible:
                color = {0.7f,0.6f,0.3f,0.5f};
                break;
            case GridCell::CellType::ConstructOk:
                color = { 0.3f,0.6f,0.3f,0.5f };
                break;
            case GridCell::CellType::ConstructBad:
                color = { 0.7f,0.3f,0.3f,0.5f };
                break;
            }

            colors.push_back(color);
            mats.push_back(c.second.transform.getMat4());
        }

        m_entityGridCells.entity->model.setBatch(mats, colors);
    }

    void View::drawGrid()
    {
        m_entityGridCells.entity->drawBasic(m_camera);
    }

    void View::_on_mouse_moved(const CommonEvents::MouseMoved& evt)
    {
        m_mousePos.x = (float)evt.x / m_width;
        m_mousePos.y = (float)evt.y / m_height;
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
        m_groundEntity.entity->model.setBatch({ m_groundEntity.transform.getMat4() }, { {1,1,1,1} });

        // Lanterns
        m_model_sphere = std::make_unique<Entity>(Entity::Sphere);

        m_model = std::make_unique<Entity>("Resources/objects/train/wagon_no_wheels.glb");

        // GameObjects:
        m_gameObjects.push_back(std::make_pair<ModelId, glm::vec3>(ModelId::Locomotive, { 0,0,0 }));
        m_gameObjects.push_back(std::make_pair<ModelId, glm::vec3>(ModelId::Locomotive, { 1,1,1 }));
        m_gameObjects.push_back(std::make_pair<ModelId, glm::vec3>(ModelId::Wagon, { 2,2,2 }));
    }

    void View::_onResize() {
    }

} // namespace Thomas