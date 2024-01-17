#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>

struct Model
{
    friend struct Entity;

         Model() = default;
         Model(const std::string& path);

    // Methods
    void draw        (Shader& shader) const;
    void drawElements(Shader& shader) const;

    // Data tree for storing organized meshes
    struct Node {
        glm::mat4 transform = glm::mat4(1.0f);
        std::vector<std::unique_ptr<Mesh>> meshes = {};
        std::vector<std::unique_ptr<Node>> children = {};
    };

    const std::unique_ptr<Node>& root() const;

private:
    void _loadModel     (const std::string& path);
    void _processNode   (const aiNode* inNode, const aiScene* scene, std::unique_ptr<Node>& parent);
    void _processMesh   (const aiMesh* inMesh, const aiScene* scene, std::unique_ptr<Mesh>& mesh);

    std::vector<TextureData> _textures_loaded;
    std::unique_ptr<Node>    _root;
};
