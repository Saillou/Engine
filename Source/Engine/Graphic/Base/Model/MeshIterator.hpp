#pragma once

#include <functional>

#include "Model.hpp"

struct MeshIterator 
{
    struct Accumulator {
        const std::unique_ptr<Model::Node>& node;
        const glm::mat4 transform;
    };

    typedef std::function<void(const std::unique_ptr<Mesh>& mesh, const Accumulator& node_acc)> mesh_cbk;

    // Methods
    static void forEachMesh(const Model& model, mesh_cbk cbk);
};
