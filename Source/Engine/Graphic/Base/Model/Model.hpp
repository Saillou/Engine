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

#include "Mesh.hpp"

struct Model
{
    friend struct RenderSystem;

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
    void setMeshVao(Mesh& mesh) const;

    void draw(Shader& shader) const;
    void drawElements(Shader& shader) const;

    // Helpers
    Ref Clone() const;

    // Data tree for storing organized meshes
    struct Node {
        glm::mat4 transform = glm::mat4(1.0f);
        std::vector<std::unique_ptr<Mesh>> meshes   = {};
        std::vector<std::unique_ptr<Node>> children = {};
    };

    std::unique_ptr<Node> root = nullptr;

    // Local info 
    glm::mat4 localTransform = glm::mat4(1.0f);
    glm::vec4 localMaterial  = glm::vec4(1.0f);

protected:
    Model(SimpleShape shape = SimpleShape::Custom);
    Model(const std::string& path);

    void _loadModel(const std::string& path);
    void _processNode(const aiNode* inNode, const aiScene* scene, std::unique_ptr<Node>& parent);
    void _processMesh(const aiMesh* inMesh, const aiScene* scene, std::unique_ptr<Mesh>& mesh);
    void _cloneMesh(const std::unique_ptr<Mesh>& src, std::unique_ptr<Mesh>& dst) const;

    void _setBatch(const std::vector<glm::mat4>& models, const std::vector<glm::vec4>& colors = {});

    Buffer m_colors;
    Buffer m_instances;
};
