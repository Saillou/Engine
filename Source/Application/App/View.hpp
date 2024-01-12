#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Objects/ShadowRender.hpp>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>
#include <Engine/Graphic/Base/Shapes/Quad.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>

#include <Engine/Graphic/Base/Model/Skybox.hpp>
#include <Engine/Graphic/Base/Model/ObjectModel.hpp>

#include "Application/TrainGame/TrainGameModels.hpp"
#include <Engine/Graphic/Utils/Shader.hpp>
#include <Engine/Graphic/Utils/Framebuffer.hpp>

struct Filter {
    Quad surface;
    Shader shader;
    Framebuffer framebuffer;

    Filter();
    void apply(Framebuffer& fIn);
    void resize(int width, int height);
};

struct View : public BaseScene {
    View(int widthHint = 0, int heightHint = 0);

    void draw() override;

    bool enable_filter = true;

private:
    enum class _ObjecId {
        Character, Tree
    };

    struct _Object {
        _ObjecId id;
        glm::mat4 quat;
    };

    struct _Grid {
        float cell_size;
        int n_side;
        std::vector<glm::mat4> m_grid_cells;
    };

    void _initObjects();
    void _initParticles();

    void _setParticles(float dt = 0.0f);
    void _onResize() override;

    // Object models (vertices, textures..)
    std::unordered_map<_ObjecId, std::unique_ptr<ObjectModel>> m_models;
    std::unordered_map<ModelId, std::unique_ptr<ObjectModel>> m_gameModels;

    std::unique_ptr<Sphere> m_model_sphere;
    std::unique_ptr<Box>    m_model_box;
    std::unique_ptr<Box>    m_model_box_shadow;
    std::unique_ptr<ObjectModel> m_model;

    // Scene objects
    std::vector<_Object>    m_objects;
    std::vector<std::pair<ModelId, glm::vec3>> m_gameObjects;
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
    Filter m_filter;
    Framebuffer framebuffer_main;

    ShadowRender m_shadowRender;
    Timer::Chronometre m_timer;
};
