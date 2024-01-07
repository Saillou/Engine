#include "Model.hpp"

Model::Model(const std::string& path)
{
    _loadModel(path);
}

void Model::draw(Shader& shader) {
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].draw(shader);
    }
}

void Model::drawElements() {
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].drawElements();
    }
}

void Model::_loadModel(const std::string& path) {
    // read file via ASSIMP
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // process ASSIMP's root node recursively
    _processNode(scene->mRootNode, scene);
}

void Model::_processNode(aiNode* node, const aiScene* scene) {
    _meshes.resize(node->mNumMeshes);
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        _processMesh(scene->mMeshes[node->mMeshes[i]], scene, _meshes[i]);
    }

    // Recurse
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        _processNode(node->mChildren[i], scene);
    }
}

void Model::_processMesh(aiMesh* inMesh, const aiScene* scene, Mesh& outMesh) {
    // Mesh's vertices
    outMesh.vertices.resize(inMesh->mNumVertices);

    for (unsigned int i = 0; i < inMesh->mNumVertices; i++) {
        outMesh.vertices[i].Position = {
            inMesh->mVertices[i].x, inMesh->mVertices[i].y, inMesh->mVertices[i].z
        };

        // normals
        if (inMesh->HasNormals()) {
            outMesh.vertices[i].Normal = {
                inMesh->mNormals[i].x, inMesh->mNormals[i].y, inMesh->mNormals[i].z
            };
        }
        // texture coordinates (contains up to 8 different texture coordinates. We'll only take the 0th.)
        if (inMesh->mTextureCoords[0] != nullptr) {
            outMesh.vertices[i].TexCoords = {
                inMesh->mTextureCoords[0][i].x, inMesh->mTextureCoords[0][i].y
            };
        }
    }

    // Mesh's faces
    for (unsigned int i = 0; i < inMesh->mNumFaces; i++) {
        const aiFace& face = inMesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            outMesh.indices.push_back(face.mIndices[j]);
        }
    }

    // Mesh's materials
    aiMaterial* material = scene->mMaterials[inMesh->mMaterialIndex];
    aiString texture_file;

    material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
    if (const aiTexture* rawTextureData = scene->GetEmbeddedTexture(texture_file.C_Str())) {
        TextureData texture;
        texture.id = _TextureFromRawData(rawTextureData);
        texture.type = "texture_diffuse";

        outMesh.textures.push_back(texture);
    }

    outMesh._setup();
}

unsigned int Model::_TextureFromRawData(const aiTexture* rawTextureData) {
    int width, height, nrComponents;
    unsigned char* data = nullptr;

    stbi_set_flip_vertically_on_load(false);
    if (rawTextureData->mHeight == 0)
        data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(rawTextureData->pcData), rawTextureData->mWidth, &width, &height, &nrComponents, 0);
    else
        data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(rawTextureData->pcData), rawTextureData->mWidth * rawTextureData->mHeight, &width, &height, &nrComponents, 0);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to read at texture." << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
