#include "View.hpp"

#include <algorithm>
#include <ctime>
#include <random>
#include <iostream>

#include <Engine/Graphic/Model/Model.hpp>

struct ModelObject {
    ModelObject(const std::string& path) :
        _object(path),
        model(glm::mat4(1.0f))
    {
        _shader.
            attachSource(GL_VERTEX_SHADER, ShaderSource{}
                .add_var("layout (location = 0) in", "vec3", "aPos")
                .add_var("layout (location = 1) in", "vec3", "aNormal")
                .add_var("layout (location = 2) in", "vec2", "aTexCoords")

                .add_var("uniform", "mat4", "model")
                .add_var("uniform", "mat4", "view")
                .add_var("uniform", "mat4", "projection")

                .add_var("out", "vec2", "TexCoords")

                .add_func("void", "main", "", R"_main_(
                    TexCoords = aTexCoords;    
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                )_main_").str()
            )
            .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
                .add_var("in", "vec2", "TexCoords")
                .add_var("uniform", "sampler2D", "texture_diffuse1")
                .add_var("out", "vec4", "FragColor")

                .add_func("void", "main", "", R"_main_(
                    FragColor = texture(texture_diffuse1, TexCoords);
                )_main_").str()
            )
        .link();
    }

    void draw(const Camera& camera) {
        _shader
            .use()
            .set("projection",  camera.projection)
            .set("view",        camera.modelview)
            .set("model",       model);

        _object.draw(_shader);
    }

    glm::mat4 model;

private:
    Shader _shader;
    Model _object;
};

std::unique_ptr<ModelObject> p_obj;

// -- Scene instance --
View::View() :
    BaseScene()
{
    // Camera
    m_camera.position    = glm::vec3(0.0f, -5.0f, 0.0f);
    m_camera.direction   = glm::vec3(0.0f, 0.0, 0.0f);
    m_camera.fieldOfView = 45.0f;

    // Objects
    p_obj = std::make_unique<ModelObject>("Resources/objects/paint/tree.glb");
    //p_obj = std::make_unique<ModelObject>("Resources/objects/backpack/backpack.obj");
    p_obj->model = glm::rotate(p_obj->model, 1.5f, glm::vec3(1, 0, 0));

    // Start
    m_timer.tic();
}

void View::draw() {
    BaseScene::clear();
    float dt_s = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;

    // Draw stuff
    p_obj->model = glm::rotate(p_obj->model, 0.5f * dt_s, glm::vec3(0, 1, 0));
    p_obj->draw(m_camera);
    
    // Prepare next
    m_timer.tic();
}
