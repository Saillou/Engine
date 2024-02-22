#include "MeshIterator.hpp"

// - Iterator -
void MeshIterator::forEachMesh(const Model& model, mesh_cbk cbk)
{
    if (!model.root)
        return;

    std::stack<Accumulator> s;
    s.push({ model.root, model.root->transform });

    do {
        // Get
        Accumulator elt = s.top();
        s.pop();

        // Use
        for (const auto& mesh : elt.node->_meshes) 
        {
            cbk(mesh, elt);
        }

        // Next
        for (auto rit = elt.node->children.crbegin(); rit != elt.node->children.crend(); ++rit)
        {
            const auto& child = *rit;
            s.push({ child, child->transform * elt.transform });
        }
    } while (!s.empty());
}
