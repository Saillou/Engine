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

class Model
{
public:
    Model(const std::string& path);

    void draw(Shader& shader);
    void drawElements(Shader& shader);

private:
    // Data tree for storing organized meshes
    struct _Node {
        glm::mat4 transform = glm::mat4(1.0f);
        std::vector<std::unique_ptr<Mesh>> meshes = {};
        std::vector<std::unique_ptr<_Node>> children = {};
    };

    void _loadModel(const std::string& path);
    void _processNode(const aiNode* inNode, const aiScene* scene, std::unique_ptr<_Node>& parent);
    void _processMesh(const aiMesh* inMesh, const aiScene* scene, std::unique_ptr<Mesh>& mesh);

    static unsigned int _TextureFromRawData(const aiTexture* rawTextureData);

    std::vector<TextureData> _textures_loaded;
    std::unique_ptr<_Node> _root;
};
