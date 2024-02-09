//#pragma once
//
//#include <vector>
//#include <memory>
//#include <unordered_map>
//
//#include <Engine/Utils/Timer.hpp>
//#include <Engine/Utils/Filter/BaseFilter.hpp>
//
//#include <Engine/Graphic/Base/BaseScene.hpp>
//
//#include <Engine/Graphic/Base/Model/Skybox.hpp>
//#include <Engine/Graphic/Base/Model/Entity.hpp>
//
//struct ViewForest : public BaseScene {
//    ViewForest(int widthHint = 0, int heightHint = 0);
//
//    void mouse_on(int x, int y);
//
//    bool enable_filter = false;
//
//protected:
//    void _draw()      override;
//    void _post_draw() override;
//
//private:
//    enum class _ObjectId {
//        Character, Tree, Locomotive,
//        Cube, Sphere, Target,
//        Grid
//    };
//
//    struct _Object {
//        _ObjectId id;
//        glm::vec4 material_color;
//        std::vector<Pose> quats;
//    };
//
//    struct _Grid {
//        float cell_size;
//        int n_side;
//        std::vector<Pose> grid_cells;
//    };
//
//    void _initObjects();
//    void _initParticles();
//    void _initFilters();
//
//    void _setParticles(float dt = 0.0f);
//    void _setObjects();
//    void _on_resize() override;
//
//    // Object models (vertices, textures..)
//    std::unordered_map<_ObjectId, std::unique_ptr<Entity>> m_entities;
//
//    // Scene objects
//    std::vector<_Object>    m_objects;
//    std::unique_ptr<_Grid>  m_grid;
//    std::unique_ptr<Skybox> m_skybox;
//
//    // Fire grid
//    struct FireGrid {
//        glm::vec3 pos;
//
//        struct Particles {
//            const size_t amount;
//            std::unique_ptr<Entity> object;
//            std::vector<Pose>       models;
//            std::vector<Material>   materials;
//            std::vector<glm::vec4>  speeds;
//        } particles;
//    } m_fireGrid;
//
//    // Other members
//    BaseFilter m_filter;
//
//    glm::vec2 m_mousePos;
//    Timer::Chronometre m_timer;
//};