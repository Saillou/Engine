#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


class Model
{
public:
    Model(const std::string& path);

    void draw(Shader& shader);

private:
    void _loadModel(const std::string& path);
    void _processNode(aiNode* node, const aiScene* scene);
    void _processMesh(aiMesh* inMesh, const aiScene* scene, Mesh& outMesh);

    static unsigned int _TextureFromRawData(const aiTexture* rawTextureData);

    std::vector<TextureData> _textures_loaded;
    std::vector<Mesh>        _meshes;
};
