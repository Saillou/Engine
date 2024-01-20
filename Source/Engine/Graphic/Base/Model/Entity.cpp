#include "Entity.hpp"

#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

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
