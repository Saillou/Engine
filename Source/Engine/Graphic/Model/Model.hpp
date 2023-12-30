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
    std::vector<TextureData> textures_loaded;
    std::vector<Mesh>        meshes;
    std::string directory;

    Model(const std::string& path);

    void draw(Shader& shader);

private:
    void _loadModel(const std::string& path);
    void _processNode(aiNode* node, const aiScene* scene);
    Mesh _processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureData> _loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    static unsigned int _TextureFromRawData(const aiTexture* rawTextureData);
    static unsigned int _TextureFromFile(const char* path, const std::string& directory);
};
