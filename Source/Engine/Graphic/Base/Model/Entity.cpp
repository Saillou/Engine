#include "Entity.hpp"

#include "Primitive/Quad.hpp"
#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

// Instances
Entity::Entity(const std::string& path) :
    _model(std::make_shared<Model>(path))
{
    // ..
}

Entity::Entity(SimpleShape shape):
    _model(std::make_shared<Model>())
{
    _model->_root = std::make_unique<Model::Node>();

    switch (shape) {
    case Entity::Quad:
        _model->_root->meshes.push_back(Quad::CreateMesh());
        break;

    case Entity::Cube: 
        _model->_root->meshes.push_back(Cube::CreateMesh());
        break;

    case Entity::Sphere: 
        _model->_root->meshes.push_back(Sphere::CreateMesh());
        break;
    }
}

// Accessors
void Entity::setPosesWithMaterials(const std::vector<Pose>& poses, const std::vector<Material>& materials) {
    _poses = poses;
    _materials = materials;
}

Pose& Entity::localPose() {
    return (Pose&)_model->_localPose;
}
Material& Entity::localMaterial() {
    return _model->_localMaterial;
}

const Pose& Entity::localPose() const {
    return _model->_localPose;
}
const Material& Entity::localMaterial() const {
    return _model->_localMaterial;
}

std::vector<Pose>& Entity::poses() {
    return _poses;
}

std::vector<Material>& Entity::materials() {
    return _materials;
}

const std::vector<Pose>& Entity::poses() const {
    return _poses;
}
const std::vector<Material>& Entity::materials() const {
    return _materials;
}

const Model& Entity::model() const {
    return *_model;
}

Model& Entity::model() {
    return *_model;
}

// Methods
void Entity::_update_model_buffer() {
    _model->_setBatch(std::vector<glm::mat4>(_poses.cbegin(), _poses.cend()), Material::ExtractColors(_materials));
}
