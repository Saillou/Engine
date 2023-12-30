#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Graphic/Base/Shapes/Sphere.hpp>
#include <Engine/Graphic/Base/Shapes/Box.hpp>

#include <Engine/Utils/Timer.hpp>

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

    std::unordered_map<_ObjecId, std::unique_ptr<ObjectModel>> m_models; // Object models (vertices, textures..)
    std::vector<_Object> m_objects;                                      // All scene objects

    Timer::Chronometre m_timer;
};
