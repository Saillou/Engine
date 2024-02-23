#include "HelperTrain.hpp"

// -------- Utilities --------
Entity HelperTrain::train() 
{
    BodyComponent body;
    body.model = Model::Load("Resources/objects/train/locomotive.glb");

    // Local
    glm::mat4& pose(body.transform.local);

    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    pose = glm::scale(pose, glm::vec3(0.5f));
    pose = glm::translate(pose, glm::vec3(0, 0, 0.17f));
    pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    // Draw
    DrawComponent draw;
    draw.type = DrawComponent::Solid;

    // Entity
    Entity train = ECS::createEntity();
    ECS::addComponent<DrawComponent>(train, draw);
    ECS::addComponent<BodyComponent>(train, body);

    return train;
}

Entity HelperTrain::tile_with_texture(const std::string& texturePath) 
{
    // Create model
    static std::unordered_map<std::string, Model::Ref> tiles_model;
    if (tiles_model.find(texturePath) == tiles_model.cend()) {
        // Clone to not touch other quad;
        tiles_model[texturePath] = Model::Load(Model::SimpleShape::Quad)->Clone(); 

        // Set texture on the first (and only) mesh 
        tiles_model[texturePath]->root->meshes().front()->textures.push_back(
            TextureData{ "texture_diffuse", std::make_unique<Texture>(texturePath) }
        );
    }

    // Create entity
    BodyComponent body;
    body.model = tiles_model[texturePath];

    // Transform:
    glm::mat4& pose(body.transform.local);

    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    //  - scale in half because quads are 2x2 by default
    pose = glm::scale(pose, glm::vec3(0.5f));
    pose = glm::translate(pose, glm::vec3(0, 0, 1.0f));
    pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    // Define opacity < 1: trick renderer with reordering
    body.material = glm::vec4(0, 0, 0, 0.9f);


    // Draw:
    DrawComponent draw;
    draw.type = DrawComponent::Solid;

    // Entity
    Entity tile = ECS::createEntity();
    ECS::addComponent<DrawComponent>(tile, draw);
    ECS::addComponent<BodyComponent>(tile, body);

    return tile;
}

Entity HelperTrain::tile_with_rgba(const glm::vec4& colorRGBA)
{
    BodyComponent body;
    body.model = Model::Load(Model::SimpleShape::Quad);

    // Transform:
    glm::mat4& pose(body.transform.local);

    //  - scale in half because quads are 2x2 by default
    pose = glm::scale(pose, glm::vec3(0.5f));

    // channel color 8-bits to float
    body.material = colorRGBA / 255.0f;

    // Draw:
    DrawComponent draw;
    draw.type = DrawComponent::Solid;

    // Entity
    Entity tile = ECS::createEntity();
    ECS::addComponent<DrawComponent>(tile, draw);
    ECS::addComponent<BodyComponent>(tile, body);

    return tile;
}

glm::mat4 HelperTrain::pose(const glm::vec2& position) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
}

glm::mat4 HelperTrain::pose_scale(const glm::vec2& position, float scale) {
    return glm::scale(pose(position), glm::vec3(scale));
}

glm::mat4 HelperTrain::pose_rot(const glm::vec2& position, float angle) {
    return glm::rotate(pose(position), angle, glm::vec3(0, 0, 1));
}