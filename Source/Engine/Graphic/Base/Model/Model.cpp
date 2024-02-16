#include "Model.hpp"
#include "MeshIterator.hpp"

#include "Primitive/Quad.hpp"
#include "Primitive/Cube.hpp"
#include "Primitive/Sphere.hpp"

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/string_cast.hpp>

using namespace glm;

// Helper
inline mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
    mat4 to;

    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

    return to;
}

// Creators
Model::Ref Model::Create(SimpleShape shape)
{
    struct ModelInstance : public Model {
        ModelInstance(SimpleShape s) : Model(s) { }
    };

    return std::make_shared<ModelInstance>(shape);
}
Model::Ref Model::Create(const std::string& path)
{
    struct ModelInstance : public Model {
        ModelInstance(const std::string& p) : Model(p) { }
    };

    return std::make_shared<ModelInstance>(path);
}

// Instance
Model::Model(SimpleShape shape) :
    root(std::make_unique<Model::Node>())
{
    switch (shape)
    {
    case Quad:
        root->meshes.push_back(Quad::CreateMesh());
        break;

    case Cube:
        root->meshes.push_back(Cube::CreateMesh());
        break;

    case Sphere:
        root->meshes.push_back(Sphere::CreateMesh());
        break;
    }
}

Model::Model(const std::string& path)
{
    _loadModel(path);
}

void Model::draw(Shader& shader) const 
{
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc) 
    {
        shader.use();
        if (shader.has("LocalModel"))
            shader.set("LocalModel", localPose * node_acc.transform);

        mesh->bindTextures(shader);
        mesh->drawElements();
        mesh->unbindTextures();
    });
}

void Model::drawElements(Shader& shader) const 
{
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc)
    {
        shader.use();
        if (shader.has("LocalModel"))
            shader.set("LocalModel", localPose * node_acc.transform);

        mesh->drawElements();
    });
}

Model::Ref Model::Clone() const
{
    Model::Ref model_copied = Create();

    std::stack<const std::unique_ptr<Node>*> st;
    std::stack<std::unique_ptr<Node>*> st_copied;

    st.push(&root);
    st_copied.push(&(model_copied->root));

    while (!st.empty()) {
        // Get next in line
        const auto currNode = st.top();
        st.pop();

        auto currNodeCopied = st_copied.top();
        st_copied.pop();

        // Clone meshes
        (*currNodeCopied)->transform = (*currNode)->transform;
        for (const auto& mesh : (*currNode)->meshes) {
            (*currNodeCopied)->meshes.push_back(std::make_unique<Mesh>());
            _cloneMesh(mesh, (*currNodeCopied)->meshes.back());
        }

        // Add children
        for (size_t i = 0; i < (*currNode)->children.size(); i++) {
            (*currNodeCopied)->children.push_back(std::make_unique<Node>());

            st.push(&(*currNode)->children[i]);
            st_copied.push(&(*currNodeCopied)->children[i]);
        }
    }

    return model_copied;
}

std::vector<glm::mat4> Model::GetMeshesPoses() const
{
    std::vector<glm::mat4> quats;

    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc)
    {
        quats.push_back(
            localPose * node_acc.transform * mesh->obb()
        );
    });

    return quats;
}

void Model::_setBatch(const std::vector<mat4>& models, const std::vector<vec4>& colors) {
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc)
    {
        mesh->updateBatch(models, colors);
    });
}

void Model::_updateInternalBatch() {
    _setBatch(std::vector<mat4>(poses.cbegin(), poses.cend()), Material::ExtractColors(materials));
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
    root = std::make_unique<Node>();
    _processNode(scene->mRootNode, scene, root);
}

void Model::_processNode(const aiNode* inNnode, const aiScene* scene, std::unique_ptr<Node>& currentNode) {
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
        currentNode->children.push_back(std::make_unique<Node>());
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
        outMesh.textures.push_back(TextureData{
            "texture_diffuse",
            std::make_unique<Texture>(rawTextureData)
        });
    }

    outMesh.sendToGpu();
    outMesh.compute_obb();
}

void Model::_cloneMesh(const std::unique_ptr<Mesh>& src, std::unique_ptr<Mesh>& dst)
{
    const Mesh& inMesh = *src;
    Mesh& outMesh = *dst;

    outMesh.vertices = inMesh.vertices;
    outMesh.indices  = inMesh.indices;

    for (const TextureData& inTexture : inMesh.textures) {
        outMesh.textures.push_back(TextureData{
            inTexture.type,
            std::make_unique<Texture>(*inTexture.data.get())
        });
    }

    outMesh.sendToGpu();
    outMesh.compute_obb();
}
