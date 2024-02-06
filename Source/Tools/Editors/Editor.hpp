#pragma once

#include <memory>
#include <vector>

#include <Engine/Graphic/Base/Model/Entity.hpp>
#include <Engine/Graphic/Base/Scene.hpp>

class Editor
{
public:
    Editor(Scene& scene) : m_scene(scene) {};
    virtual ~Editor() = default;

    virtual void onEnter()  = 0;
    virtual void onExit()   = 0;
    virtual void onUpdate() = 0;

protected:
    Scene& m_scene;
};
