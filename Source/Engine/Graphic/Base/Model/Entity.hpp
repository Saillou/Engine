#pragma once

#include <string>
#include <vector>

#include "../Render/RenderType.hpp"
#include "Pose.hpp"
#include "Model.hpp"
#include "Material.hpp"

struct Entity
{
    enum SimpleShape {
        Custom, Cube, Sphere
    };

    Entity(SimpleShape shape = SimpleShape::Custom);
    Entity(const std::string& path);

    virtual ~Entity() = default;

    // Helpers
    void addPose(const Pose&);

    // Setters
    void setLocalPose(const Pose&);
    void setPoses(const std::vector<Pose>&);
    void setPosesWithMaterials(const std::vector<Pose>&, const std::vector<Material>&);

    // Getters
    Model model;
    Material material;

    Pose localPose() const;
    const std::vector<Pose>& poses() const;

private:
    std::vector<Pose> _poses;
};
