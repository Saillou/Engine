#pragma once

#include <string>
#include <vector>

#include "../Render/RenderType.hpp"
#include "Pose.hpp"
#include "Model.hpp"

struct Entity
{
    friend struct Renderer;

    enum SimpleShape {
        Custom, 
        Quad, Cube, Sphere
    };

    Entity(SimpleShape shape = SimpleShape::Custom);
    Entity(const std::string& path);

    virtual ~Entity() = default;

    // Accessors
    void setPosesWithMaterials(const std::vector<Pose>&, const std::vector<Material>&);

    Pose& localPose();
    Material& localMaterial();

    const Pose& localPose() const;
    const Material& localMaterial() const;

    std::vector<Pose>& poses();
    std::vector<Material>& materials();

    const std::vector<Pose>& poses() const;
    const std::vector<Material>& materials() const;

    const Model& model() const;
    Model& model();

protected:
    void _update_model_buffer();

    std::shared_ptr<Model> _model;

    std::vector<Pose>     _poses;
    std::vector<Material> _materials;
};
