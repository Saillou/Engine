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
    root(std::make_unique<Model::Node>()),
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
    m_colors.bindData(sizeof(vec4));
    m_instances.bindData(sizeof(mat4));

    std::unique_ptr<Mesh> mesh = nullptr;
    switch (shape)
    {
    case Quad:
        mesh = Quad::CreateMesh();
        break;

    case Cube:
        mesh = Cube::CreateMesh();
        break;

    case Sphere:
        mesh = Sphere::CreateMesh();
        break;

    default:
        return;
    }

    if (!mesh)
        return;

    setMeshVao(*mesh);
    root->meshes.emplace_back(std::move(mesh));
}

Model::Model(const std::string& path):
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
    m_colors.bindData(sizeof(vec4));
    m_instances.bindData(sizeof(mat4));

    _loadModel(path);
}

void Model::setMeshVao(Mesh& mesh) const {
    mesh.setupVao();

    m_colors.bind();
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glVertexAttribDivisor(3, 1);

    m_instances.bind();
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(4 + i);
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(i * sizeof(vec4)));
        glVertexAttribDivisor(4 + i, 1);
    }
}

void Model::draw(Shader& shader, const glm::mat4& localTransform) const
{
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc) 
    {
        shader.use();
        if (shader.has("LocalModel"))
            shader.set("LocalModel", localTransform * node_acc.transform);

        mesh->bindTextures(shader);
        mesh->drawElements((GLsizei)m_instances.size() / sizeof(mat4));
        mesh->unbindTextures();
    });
}

void Model::drawElements(Shader& shader, const glm::mat4& localTransform) const
{
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc)
    {
        shader.use();
        if (shader.has("LocalModel"))
            shader.set("LocalModel", localTransform * node_acc.transform);

        mesh->drawElements((GLsizei)m_instances.size() / sizeof(mat4));
    });
}

void Model::_setBatch(const std::vector<mat4>& models, const std::vector<vec4>& colors) {
    m_instances.bindData(models);

    if (colors.size() >= models.size()) {
        m_colors.bindData(colors);
    }
    else {
        std::vector<vec4> res_colors = colors;
        res_colors.resize(models.size());
        m_colors.bindData(res_colors);
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

    outMesh.compute_obb();
    setMeshVao(outMesh);
}
