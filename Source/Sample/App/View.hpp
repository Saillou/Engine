#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Objects/ShadowRender.hpp>
#include <Engine/Utils/Filter/BaseFilter.hpp>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>

#include <Engine/Graphic/Base/Model/Skybox.hpp>
#include <Engine/Graphic/Base/Model/Entity.hpp>

struct View : public BaseScene {
    View(int widthHint = 0, int heightHint = 0);

    void draw() override;
    void mouse_on(int x, int y);

    bool enable_filter = false;

private:
    enum class _ObjectId {
        Character, Tree, Locomotive,
        Cube, Sphere
    };

    struct _Object {
        _ObjectId id;
        glm::mat4 quat;
    };

    struct _Grid {
        float cell_size;
        int n_side;
        std::vector<glm::mat4> m_grid_cells;
    };

    void _initObjects();
    void _initParticles();
    void _initFilters();

    void _setParticles(float dt = 0.0f);
    void _onResize() override;

    // Object models (vertices, textures..)
    std::unordered_map<_ObjectId, std::unique_ptr<Entity>> m_entities;
    std::unique_ptr<Box> m_model_box;

    // Scene objects
    std::vector<_Object>    m_objects;                                      
    std::unique_ptr<_Grid>  m_grid;
    std::unique_ptr<Skybox> m_skybox;

    // Fire grid
    struct FireGrid {
        glm::vec3 pos;

        struct Particles {
            const size_t amount;

            std::unique_ptr<Box> object;
            std::vector<glm::mat4> models;
            std::vector<glm::vec4> colors;

            std::vector<glm::vec4> speeds;
        } particles;
    } m_fireGrid;

    // Other members
    BaseFilter m_filter;
    Framebuffer framebuffer_main;

    glm::vec2 m_mousePos;
    ShadowRender m_shadowRender;
    Timer::Chronometre m_timer;
};