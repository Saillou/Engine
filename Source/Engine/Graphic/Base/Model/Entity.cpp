#include "Entity.hpp"

#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

// Helpers
static inline std::vector<glm::vec4> _extractColors(const std::vector<Material>& materials) {
    std::vector<glm::vec4> colors;
    colors.reserve(materials.size());

    for (const Material& material : materials) {
        colors.push_back(material.diffuse_color);
    }
    return colors;
}

// Instances
Entity::Entity(const std::string& path) :
    model(path)
{
    // ..
}

Entity::Entity(SimpleShape shape)
{
    model._root = std::make_unique<Model::Node>();

    switch (shape) {
    case Entity::Cube: 
        model._root->meshes.push_back(Cube::CreateMesh());
        break;

    case Entity::Sphere: 
        model._root->meshes.push_back(Sphere::CreateMesh());
        break;
    }
}

void Entity::addPose(const Pose& newPose) {
    _poses.push_back(newPose);
    model._setBatch(std::vector<glm::mat4>(_poses.cbegin(), _poses.cend()));
}

void Entity::setLocalPose(const Pose& pose) {
    model._localPose = pose;
}

void Entity::setPoses(const std::vector<Pose>& poses) {
    _poses = poses;
    model._setBatch(std::vector<glm::mat4>(_poses.cbegin(), _poses.cend()));
}

void Entity::setPosesWithMaterials(const std::vector<Pose>& poses, const std::vector<Material>& materials) {
    _poses = poses;
    model._setBatch(std::vector<glm::mat4>(_poses.cbegin(), _poses.cend()), _extractColors(materials));
}

Pose Entity::localPose() const {
    return model._localPose;
}

const std::vector<Pose>& Entity::poses() const {
    return _poses;
}
