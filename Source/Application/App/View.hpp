#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>
#include <Engine/Graphic/Model/Skybox.hpp>
#include <Engine/Graphic/Utils/Framebuffer.hpp>

#include "Models/ObjectModel.hpp"

struct View : public BaseScene {
    View();

    void draw() override;

private:
    enum class _ObjecId {
        Character, Tree
    };

    struct _Object {
        _ObjecId id;
        glm::mat4 quat;
    };

    void _initObjects();
    void _onResize() override;

    // Object specifics
    std::unordered_map<_ObjecId, std::unique_ptr<ObjectModel>> m_models; // Object models (vertices, textures..)
    std::vector<_Object> m_objects;                                      // All scene objects

    std::unique_ptr<Box> m_model_box;
    std::unique_ptr<Skybox> m_skybox;

    // Other members
    Timer::Chronometre m_timer;
};
