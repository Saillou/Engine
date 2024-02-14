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

    void _setBatch(const std::vector<glm::mat4>& models, const std::vector<glm::vec4>& colors = {});
    void _updateInternalBatch(); // _setBatch with poses and materials members
};
