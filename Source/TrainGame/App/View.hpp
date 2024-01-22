#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

#include <Engine/Utils/Timer.hpp>

#include <Engine/Graphic/Base/Scene.hpp>

#include <Engine/Graphic/Base/Model/Skybox.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

#include "TrainGame/TrainGame/TrainGameModels.hpp"
#include <Engine/Graphic/Utils/Shader.hpp>
#include <Engine/Graphic/Utils/Framebuffer.hpp>

#include "TrainGame/TrainGame/GridCell.hpp"

#include <Engine/Events/CommonEvents.hpp>

#include "TrainGame/App/Objects/Transform.h"

namespace Thomas
{
    struct View : private Event::Subscriber {
        View(Scene& scene);

        void loadModels(size_t size);

        void draw(ModelId id, const Transform& transform);
        void drawGrid(const std::map<std::pair<int,int>, GridCell>& cells);

        Scene& scene;

    private:
        void drawGrid();

        // Events
        void _draw(const SceneEvents::Draw&);
        void _post_process(const SceneEvents::PostDraw&);
        void _on_resize(const SceneEvents::Resized&);
        void _on_mouse_moved(const CommonEvents::MouseMoved& evt);

        struct _Grid {
            float cell_size;
            int n_side;
            std::vector<glm::mat4> m_grid_cells;
        };

        struct _GridCells {
            std::unique_ptr<Entity> entity;
            std::map<std::pair<int, int>, GridCell> cells;
        };

        struct _EntityModel {
            Transform transform;
            std::unique_ptr<Entity> entity;
        };

        void _initObjects();

        // Object models (vertices, textures..)
        std::unordered_map<ModelId,  std::unique_ptr<Entity>> m_gameModels;
        std::unordered_map<ModelId,  std::vector<Transform>>  m_modelsToDraw;

        std::map<std::pair<int, int>, GridCell> m_gridCells;

        std::unique_ptr<Entity> m_model_sphere;
        
        std::unique_ptr<Entity> m_model;
        std::unique_ptr<Entity> m_target;

        _EntityModel m_groundEntity;

        std::unique_ptr<Entity> m_entityGridLayout;
        _GridCells m_entityGridCells;

        // Scene objects
        std::vector<std::pair<ModelId, glm::vec3>> m_gameObjects;
        std::unique_ptr<_Grid>  m_grid;
        std::unique_ptr<Skybox> m_skybox;

        // Other members
        Timer::Chronometre m_timer;

        glm::vec2 m_mousePos;
    };
} // namespace Thomas