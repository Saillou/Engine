#pragma once

#include <string>
#include <vector>

#include "../Render/RenderType.hpp"
#include "Pose.hpp"
#include "Model.hpp"
#include "Material.hpp"

struct Entity
{
    friend struct Renderer;

    enum SimpleShape {
        Custom, Cube, Sphere
    };

    Entity(SimpleShape shape = SimpleShape::Custom);
    Entity(const std::string& path);

    virtual ~Entity() = default;

    // Setters
    void setLocalPose(const Pose&);
    void setLocalMaterial(const Material&);

    void setPoses(const std::vector<Pose>&);
    void setPosesWithMaterials(const std::vector<Pose>&, const std::vector<Material>&);

    // Getters
    Pose& localPose();
    Material& localMaterial();

    const Pose& localPose() const;
    const Material& localMaterial() const;

    const Model& model() const;
    const std::vector<Pose>& poses() const;
    const std::vector<Material>& materials() const;

protected:
    void _update_model_buffer();

    std::shared_ptr<Model> _model;

    Pose     _localPose;
    Material _localMaterial;

    std::vector<Pose>     _poses;
    std::vector<Material> _materials;

};
