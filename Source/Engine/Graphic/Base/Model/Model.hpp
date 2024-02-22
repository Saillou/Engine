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

    enum class SimpleShape {
        Quad, Cube, Sphere
    };

    typedef std::shared_ptr<Model> Ref;
public:
    // Constructors
    static Ref Create(int customId);
    static Ref Load(SimpleShape shape);
    static Ref Load(const std::string& path);

    Ref Clone();

    static void ClearCache();

    virtual ~Model() = default;

    // Data tree for storing organized meshes
    struct Node {
        glm::mat4 transform = glm::mat4(1.0f);
        std::vector<std::unique_ptr<Node>> children = {};

        // Readonly to force the usage of 'addMesh' method (and not fuck with VAOs)
        const std::vector<std::unique_ptr<Mesh>>& meshes() {
            return _meshes;
        }
    private:
        friend Model;
        friend struct MeshIterator;

        std::vector<std::unique_ptr<Mesh>> _meshes = {};
    };

    std::unique_ptr<Node> root = nullptr;

    // Methods
    void draw(Shader& shader) const;
    void drawElements(Shader& shader) const;

    void addMesh(Mesh& mesh, std::unique_ptr<Node>& node);

    // Getters
    const std::string& uuid() const;

protected:
    Model();
    Model(SimpleShape shape);
    Model(const std::string& path);

    void _loadModel(const std::string& path);
    void _processNode(const aiNode* inNode, const aiScene* scene, std::unique_ptr<Node>& parent);
    void _processMesh(const aiMesh* inMesh, const aiScene* scene, std::unique_ptr<Mesh>& mesh);

    void _setMeshVao(Mesh& mesh) const;
    void _setBatch(const std::vector<glm::mat4>& models, const std::vector<glm::vec4>& colors = {});

    Buffer m_colors;
    Buffer m_instances;

private:
    static std::unordered_map<std::string, Ref> _s_model_cache;
    
    std::string _uuid = "";
};
