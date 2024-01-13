#include "Model.hpp"

#include <glm/gtx/string_cast.hpp>

// Helper
inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
    glm::mat4 to;

    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

    return to;
}

// Instance
Model::Model(const std::string& path)
{
    _loadModel(path);
}

void Model::draw(Shader& shader) {
    if (!_root)
        return;

    std::stack<std::unique_ptr<_Node>*> st;
    st.push(&_root);

    while (!st.empty()) {
        // Get next in line
        const auto currNode = st.top();
        st.pop();

        // Draw
        for (const auto& mesh : (*currNode)->meshes) {
            mesh->draw(shader, (*currNode)->transform);
        }

        // Add children
        for (size_t i = 0; i < (*currNode)->children.size(); i++) {
            st.push(&(*currNode)->children[i]);
        }
    }
}

void Model::drawElements(Shader& shader) {
    if (!_root)
        return;

    std::stack<std::unique_ptr<_Node>*> st;
    st.push(&_root);

    while (!st.empty()) {
        // Get next in line
        const auto currNode = st.top();
        st.pop();

        // Draw
        for (const auto& mesh : (*currNode)->meshes) {
            mesh->drawElements(shader, (*currNode)->transform);
        }

        // Add children
        for (size_t i = 0; i < (*currNode)->children.size(); i++) {
            st.push(&(*currNode)->children[i]);
        }
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

    // Ok, create and process from root (recursive)
    _root = std::make_unique<_Node>();
    _processNode(scene->mRootNode, scene, _root);
}

void Model::_processNode(const aiNode* inNnode, const aiScene* scene, std::unique_ptr<_Node>& currentNode) {
    // Relative position
    currentNode->transform = aiMatrix4x4ToGlm(inNnode->mTransformation);

    for (unsigned int i = 0; i < inNnode->mNumMeshes; i++) {
        // Create and process mesh
        currentNode->meshes.push_back(std::make_unique<Mesh>());
        _processMesh(scene->mMeshes[inNnode->mMeshes[i]], scene, currentNode->meshes.back());
    }

    // Continue recursively
    for (unsigned int i = 0; i < inNnode->mNumChildren; i++) {
        // Create and process next child
        currentNode->children.push_back(std::make_unique<_Node>());
        _processNode(inNnode->mChildren[i], scene, currentNode->children[i]);
    }
}

void Model::_processMesh(const aiMesh* inMesh, const aiScene* scene, std::unique_ptr<Mesh>& pOutMesh) {
    Mesh& outMesh = *pOutMesh;

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
