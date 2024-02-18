#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Framework/Service.hpp>
#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Filter/BaseFilter.hpp>

#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Scene.hpp>

#include <Engine/Graphic/Base/Model/Model.hpp>
#include <Engine/Graphic/Base/Model/Skybox.hpp>

struct ViewForest : Event::Subscriber {
    ViewForest();

    void mouse_on(int x, int y);

    bool enable_filter = false;

protected:
    // Events
    void _draw(const SceneEvents::Draw&);
    void _post_draw(const SceneEvents::PostDraw&);
    void _on_resize(const SceneEvents::Resized&);

private:
    enum class _ObjectId {
        Character, Tree, Locomotive,
        Cube, Sphere, Target,
        Grid
    };

    struct _Object {
        _ObjectId id;
        glm::vec4 material_color;
        std::vector<glm::mat4> quats;
    };

    struct _Grid {
        float cell_size;
        int n_side;
        std::vector<glm::mat4> grid_cells;
    };

    void _initObjects();
    void _initParticles();
    void _initFilters();

    void _setParticles(float dt = 0.0f);
    void _setObjects();

    // Object models (vertices, textures..)
    std::unordered_map<_ObjectId, Model::Ref> m_models;

    // Scene objects
    std::vector<_Object>    m_objects;
    std::unique_ptr<_Grid>  m_grid;
    std::unique_ptr<Skybox> m_skybox;

    // Fire grid
    struct FireGrid {
        glm::vec3 pos;

        struct Particles {
            const size_t amount;
            Model::Ref              object;
            std::vector<glm::mat4>       models;
            std::vector<glm::vec4>   materials;
            std::vector<glm::vec4>  speeds;
        } particles;
    } m_fireGrid;

    // Other members
    BaseFilter m_filter;

    glm::vec2 m_mousePos;
    Timer::Chronometre m_timer;
};