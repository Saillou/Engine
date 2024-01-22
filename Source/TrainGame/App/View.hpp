#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Objects/ShadowRender.hpp>

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
    struct View : public Scene, private Event::Subscriber {
        View(int widthHint = 0, int heightHint = 0);

        void draw() override;

        void loadModels(size_t size);

        void draw(ModelId id, const Transform& transform);
        void drawGrid(const std::map<std::pair<int,int>, GridCell>& cells);

    private:
        void drawGrid();

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
        void _onResize() override;

        // Object models (vertices, textures..)
        std::unordered_map<ModelId,  std::unique_ptr<Entity>> m_gameModels;
        std::unordered_map<ModelId,  std::vector<Transform>>  m_modelsToDraw;

        std::map<std::pair<int, int>, GridCell> m_gridCells;

        std::unique_ptr<Entity> m_model_sphere;
        std::unique_ptr<Entity> m_model_box;
        
        std::unique_ptr<Entity> m_model_box_shadow;
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