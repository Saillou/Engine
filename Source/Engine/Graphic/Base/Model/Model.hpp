#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Pose.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

struct Model
{
    friend struct Renderer;

    enum SimpleShape {
        Custom,
        Quad, Cube, Sphere
    };

    typedef std::shared_ptr<Model> Ref;
public:
    // Constructors
    static Ref Create(SimpleShape shape = SimpleShape::Custom);
    static Ref Create(const std::string& path);

    virtual ~Model() = default;

    // Methods
    void draw(Shader& shader) const;
    void drawElements(Shader& shader) const;

    // Helpers
    Ref Clone() const;
    std::vector<glm::mat4> GetMeshesPoses() const;

    // Data tree for storing organized meshes
    struct Node {
        glm::mat4 transform = glm::mat4(1.0f);
        std::vector<std::unique_ptr<Mesh>> meshes = {};
        std::vector<std::unique_ptr<Node>> children = {};
    };

    std::unique_ptr<Node> root = nullptr;

    // Local info
    glm::mat4 localPose = glm::mat4(1.0f);
    Material localMaterial = {};

    // World info
    std::vector<Pose>     poses = {};
    std::vector<Material> materials = {};

protected:
    Model(SimpleShape shape = SimpleShape::Custom);
    Model(const std::string& path);

    void _loadModel(const std::string& path);
    void _processNode(const aiNode* inNode, const aiScene* scene, std::unique_ptr<Node>& parent);
    void _processMesh(const aiMesh* inMesh, const aiScene* scene, std::unique_ptr<Mesh>& mesh);
    static void _cloneMesh(const std::unique_ptr<Mesh>& src, std::unique_ptr<Mesh>& dst);

    void _setBatch(const std::vector<glm::mat4>& models, const std::vector<glm::vec4>& colors = {});
    void _updateInternalBatch(); // _setBatch with poses and materials members

public:
    //// Helper (because it's a pain to do the stack trick instead of just a for loop over meshes)
    //struct MeshIterator {
    //    using iterator_category = std::forward_iterator_tag;
    //    using difference_type = std::ptrdiff_t;
    //    using value_type = int;
    //    using pointer = int*;  // or also value_type*
    //    using reference = int&;  // or also value_type&

    //    MeshIterator(pointer ptr) : m_ptr(ptr) {}

    //    reference operator*() const { return *m_ptr; }
    //    pointer operator->() { return m_ptr; }

    //    // Prefix increment
    //    MeshIterator& operator++() { m_ptr++; return *this; }

    //    // Postfix increment
    //    MeshIterator operator++(int) { MeshIterator tmp = *this; ++(*this); return tmp; }

    //    friend bool operator== (const MeshIterator& a, const MeshIterator& b) { return a.m_ptr == b.m_ptr; };
    //    friend bool operator!= (const MeshIterator& a, const MeshIterator& b) { return a.m_ptr != b.m_ptr; };

    //private:
    //    pointer m_ptr;
    //};

    //static MeshIterator Begin(Model::Ref) {
    //    return MeshIterator();
    //};
    //static MeshIterator End(Model::Ref) {
    //    return MeshIterator();
    //};
};
