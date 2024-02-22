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

// Cache magic
std::unordered_map<std::string, Model::Ref> Model::_s_model_cache;

void Model::ClearCache()
{
    _s_model_cache.clear();
}

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
Model::Ref Model::Create(int customId)
{
    const std::string uuid = "#CUSTOM#" + std::to_string(customId);

    // Check cache
    if (_s_model_cache.find(uuid) != _s_model_cache.cend()) {
        std::cerr << "Already created object with this ID - Previous one will be invalidated." << std::endl;
    }

    // Create empty
    struct _Model_ :Model {
        _Model_() : Model() { }
    };

    // Caching
    _s_model_cache[uuid] = std::make_shared<_Model_>();
    _s_model_cache[uuid]->_uuid = uuid;

    return _s_model_cache[uuid];
}
Model::Ref Model::Load(SimpleShape shape)
{
    const std::string uuid = "#SHAPE#" + std::to_string((int)shape);

    // Check cache
    if (_s_model_cache.find(uuid) != _s_model_cache.cend()) {
        return _s_model_cache[uuid];
    }

    // Create
    struct _Model_:Model {
        _Model_(SimpleShape s) : Model(s) { }
    };

    // Caching
    _s_model_cache[uuid] = std::make_shared<_Model_>(shape);
    _s_model_cache[uuid]->_uuid = uuid;

    return _s_model_cache[uuid];
}
Model::Ref Model::Load(const std::string& path)
{
    const std::string uuid = "#FILE#" + path;

    // Check cache
    if (_s_model_cache.find(uuid) != _s_model_cache.cend()) {
        return _s_model_cache[uuid];
    }

    // Create
    struct _Model_: Model {
        _Model_(const std::string& p) : Model(p) { }
    };

    // Caching
    _s_model_cache[uuid] = std::make_shared<_Model_>(path);
    _s_model_cache[uuid]->_uuid = uuid;

    return _s_model_cache[uuid];
}

Model::Ref Model::Clone()
{
    // Create a new one
    const std::string uuid = "#CLONE#" + _uuid;

    // Create empty
    struct _Model_ :Model {
        _Model_() : Model() { }
    };

    // Caching
    Model::Ref model_copied = std::make_shared<_Model_>();
    model_copied->_uuid     = uuid;

    // Populate
    {
        std::stack<const std::unique_ptr<Node>*> st;
        std::stack<std::unique_ptr<Node>*> st_copied;

        st.push(&root);
        st_copied.push(&(model_copied->root));

        while (!st.empty()) 
        {
            const auto currNode = st.top();
            auto currNodeCopied = st_copied.top();

            st.pop();
            st_copied.pop();

            // Clone meshes
            (*currNodeCopied)->transform = (*currNode)->transform;
            for (const auto& mesh : (*currNode)->_meshes) 
            {
                (*currNodeCopied)->_meshes.push_back(std::make_unique<Mesh>());
                Mesh::Clone(mesh, (*currNodeCopied)->_meshes.back());

                model_copied->_setMeshVao(*(*currNodeCopied)->_meshes.back());
            }

            // Create nodes
            for (size_t i = 0; i < (*currNode)->children.size(); i++) 
            {
                (*currNodeCopied)->children.push_back(std::make_unique<Node>());

                st.push(&(*currNode)->children[i]);
                st_copied.push(&(*currNodeCopied)->children[i]);
            }
        }
    }

    // Return new model
    _s_model_cache[uuid] = model_copied;
    return model_copied;
}

// Instance
Model::Model():
    root(std::make_unique<Model::Node>()),
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
    m_colors.bindData(sizeof(vec4));
    m_instances.bindData(sizeof(mat4));

    // Let user do what he wants
}
Model::Model(SimpleShape shape):
    root(std::make_unique<Model::Node>()),
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
    using MSS = Model::SimpleShape;

    m_colors.bindData(sizeof(vec4));
    m_instances.bindData(sizeof(mat4));

    std::unique_ptr<Mesh> mesh = nullptr;
    switch (shape)
    {
        case MSS::Quad: mesh  = Quad::CreateMesh();   break;
        case MSS::Cube: mesh  = Cube::CreateMesh();   break;
        case MSS::Sphere:mesh = Sphere::CreateMesh(); break;
    }
    if (!mesh)
        return;

    addMesh(*mesh.release(), root);
}
Model::Model(const std::string& path):
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
    m_colors.bindData(sizeof(vec4));
    m_instances.bindData(sizeof(mat4));

    _loadModel(path);
}

// Draws
void Model::draw(Shader& shader) const
{
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc) 
    {
        shader.use();
        if (shader.has("LocalModel"))
            shader.set("LocalModel", node_acc.transform);

        mesh->bindTextures(shader);
        mesh->drawElements((GLsizei)m_instances.size() / sizeof(mat4));
        mesh->unbindTextures();
    });
}
void Model::drawElements(Shader& shader) const
{
    MeshIterator::forEachMesh(*this, [&](const std::unique_ptr<Mesh>& mesh, const MeshIterator::Accumulator& node_acc)
    {
        shader.use();
        if (shader.has("LocalModel"))
            shader.set("LocalModel", node_acc.transform);

        mesh->drawElements((GLsizei)m_instances.size() / sizeof(mat4));
    });
}
void Model::addMesh(Mesh& mesh, std::unique_ptr<Node>& parent_node) {
    _setMeshVao(mesh);
    parent_node->_meshes.emplace_back(std::move(std::unique_ptr<Mesh>(&mesh)));
}

const std::string& Model::uuid() const {
    return _uuid;
}

// Model loading
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
        currentNode->_meshes.push_back(std::make_unique<Mesh>());
        _processMesh(scene->mMeshes[inNnode->mMeshes[i]], scene, currentNode->_meshes.back());
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
    _setMeshVao(outMesh);
}

// Vao stuff
void Model::_setMeshVao(Mesh& mesh) const {
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
