#pragma once

#include <vector>
#include <unordered_map>

#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Events/CommonEvents.hpp>

struct ViewCube : Event::Subscriber {
    ViewCube();
    ~ViewCube();

protected:
    // Events
    void _update(const CommonEvents::StateUpdated&);

private:
    std::unordered_map<std::string, std::vector<Entity>> m_entities;
};