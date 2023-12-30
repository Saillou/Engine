#include "View.hpp"

struct Skybox {
    Skybox() 
    {
        // Vertices
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Textures
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        std::vector<std::string> faces
        {
            "Resources/textures/skybox/right.jpg",
            "Resources/textures/skybox/left.jpg",
            "Resources/textures/skybox/top.jpg",
            "Resources/textures/skybox/bottom.jpg",
            "Resources/textures/skybox/front.jpg",
            "Resources/textures/skybox/back.jpg"
        };

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Shader
        shader.
            attachSource(GL_VERTEX_SHADER, ShaderSource{}
                .add_var("layout (location = 0) in", "vec3", "aPos")

                .add_var("uniform", "mat4", "view")
                .add_var("uniform", "mat4", "projection")

                .add_var("out", "vec3", "TexCoords")

                .add_func("void", "main", "", R"_main_(
                    TexCoords = aPos;
                    vec4 pos = projection * view * vec4(aPos, 1.0);
                    gl_Position = pos.xyww;
                )_main_").str()
            )
            .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
                .add_var("in", "vec3", "TexCoords")
                .add_var("uniform", "samplerCube", "skybox")
                .add_var("out", "vec4", "FragColor")

                .add_func("void", "main", "", R"_main_(
                    FragColor = texture(skybox, TexCoords);
                )_main_").str()
            )
        .link();

        shader.use();
        shader.set("skybox", 0);
    }

    ~Skybox()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    Shader shader;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int texture = 0;

    std::vector<float> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
};

std::unique_ptr<Skybox> skybox;

View::View() :
    BaseScene()
{
    // Camera
    m_camera.position  = glm::vec3(0.0f, -6.0f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0, 0.0f);

    // Load models
    m_timer.tic();
    {
        m_models[_ObjecId::Tree]      = std::make_unique<ObjectModel>("Resources/objects/tree/tree.glb");
        m_models[_ObjecId::Character] = std::make_unique<ObjectModel>("Resources/objects/character/character.glb");
    }
    std::cout << "Models loaded in: " << m_timer.elapsed<Timer::millisecond>() << "ms." << std::endl;

    // Populate objects
    _initObjects();

    skybox = std::make_unique<Skybox>();
}

void View::draw() {
    float dt_s = m_timer.elapsed<Timer::microsecond>() / 1'000'000.0f;

    // Main scene
    BaseScene::clear();
    {
        // Draw objects
        for (const _Object& obj : m_objects) {
            m_models[obj.id]->draw(m_camera, obj.quat);
        }

        // Draw grid
        const int N = 10;
        const float s = 0.3f;
        for (int i = 0; i < N*N; i++) {
            int x = i % N - N/2;
            int y = i / N - N/2;

            m_model_box->draw(m_camera, glm::translate(glm::mat4(1.0f), s*glm::vec3(x, y, 0)));
        }
    }

    // Skybox
    glDepthFunc(GL_LEQUAL); // depth test passes when values are equal to depth buffer's content
    {
        skybox->shader.use();
        skybox->shader.set("view", glm::mat4(glm::mat3(m_camera.modelview))); // remove translation from the view matrix
        skybox->shader.set("projection", m_camera.projection);

        // skybox cube
        glBindVertexArray(skybox->VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    glDepthFunc(GL_LESS);
    
    // Prepare next
    m_timer.tic();
}

void View::_initObjects() {
    // Ground
    m_model_box = std::make_unique<Box>(0.3f);
    m_model_box->addRecipe(Cookable::CookType::Solid,    glm::vec4(0.1f, 0.12f, 0.1f, 1));
    m_model_box->addRecipe(Cookable::CookType::Geometry, glm::vec4(0.2f, 0.2f, 0.2f, 1));

    // Trees
    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(+0.95f, 0, +0.95f)),  // Translation
            glm::vec3(0.1f, 0.1f, 0.1f)         // Scale
        )
    });

    m_objects.push_back({_ObjecId::Tree,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(-0.90f, 0, -0.95f)),  // Translation
            glm::vec3(0.1f, 0.1f, 0.1f)         // Scale
        )
    });

    // Main character
    m_objects.push_back({_ObjecId::Character,
        glm::scale(
            glm::translate(
                glm::rotate(glm::mat4(1.0f),    // Identity
                    1.5f, glm::vec3(1, 0, 0)),  // Rotation
                glm::vec3(0, 0, 0)),            // Translation
            glm::vec3(0.02f, 0.02f, 0.02f)      // Scale
        )
    });
}

void View::_onResize() {
    // For example resize: framebuffers ..
}