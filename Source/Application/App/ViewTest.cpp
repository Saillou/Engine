#include "ViewTest.hpp"

ViewTest::ViewTest(int widthHint, int heightHint) :
    BaseScene(widthHint, heightHint)
{
    // build and compile shaders
    createPointShadowShader();
    createSimpleDepthShader();

    m_camera.position = glm::vec3(5.0f, -4.5f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0f, 0.0f);

    textureWood = std::make_unique<Texture>("Resources/textures/wood.png");
    lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

    // configure depth map FBO
    framebufferDepth = std::make_unique<Framebuffer>(Framebuffer::Cubemap, 1024, 1024, GL_DEPTH_ATTACHMENT);

    // shader configuration
    shaderPoint.use();
    shaderPoint.set("diffuseTexture", 0);
    shaderPoint.set("depthMap", 1);
}

void ViewTest::draw() {
    static float time = 0.0f;

    // World is moving
    lightPos.z = std::sin(time * 0.5f) * 3.0f;
    _update_camera();

    // Create depth cubemap transformation matrices
    constexpr float near_plane = 1.0f;
    constexpr float far_plane  = 25.0f;
    const glm::mat4 shadowProj = glm::perspective(1.57f, 1.0f, near_plane, far_plane);
    const std::array<glm::mat4, 6> shadowTransforms = 
    {
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(+1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, +1.0f,  0.0f), glm::vec3(0.0f,  0.0f, +1.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, +1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // Render scene to depth cubemap
    BaseScene::Viewport(framebufferDepth->width(), framebufferDepth->height());
    framebufferDepth->bind();
    framebufferDepth->clear();
    {
        simpleDepthShader.use();
        for (unsigned int i = 0; i < 6; i++) {
            simpleDepthShader.set("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        }
        simpleDepthShader.set("far_plane", far_plane);
        simpleDepthShader.set("lightPos", lightPos);
        renderScene(simpleDepthShader);
    }
    framebufferDepth->unbind();

    // Render scene as normal using depth map
    BaseScene::Viewport(m_width, m_height);
    BaseScene::clear();
    {
        shaderPoint.use();
        glm::mat4 projection = m_camera.projection;
        glm::mat4 view = m_camera.modelview;

        shaderPoint.set("projection", projection);
        shaderPoint.set("view", view);

        // set lighting uniforms
        shaderPoint.set("lightPos", lightPos);
        shaderPoint.set("viewPos", m_camera.position);
        shaderPoint.set("shadows", true);
        shaderPoint.set("far_plane", far_plane);

        Texture::activate(GL_TEXTURE0);
        textureWood->bind();

        Texture::activate(GL_TEXTURE1);
        framebufferDepth->texture().bind();

        renderScene(shaderPoint);
    }

    time += 1.0f / 60.0f;
}

void ViewTest::renderScene(Shader& shader)
{
    // room cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f));
    shader.use();
    shader.set("model", model);

    // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    glDisable(GL_CULL_FACE);
    {
        if (shader.has("reverse_normals"))
            shader.set("reverse_normals", true); // A small little hack to invert normals when drawing cube from the inside so lighting still works.

        renderCube();

        if (shader.has("reverse_normals"))
            shader.set("reverse_normals", false); // and of course disable it
    }
    glEnable(GL_CULL_FACE);
    
    // cube
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.set("model", model);
    renderCube();
}

void ViewTest::renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void ViewTest::createPointShadowShader() {
    shaderPoint.
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")

            .add_var("out", "vec2", "TexCoords")
            .add_var("out", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec3 Normal;
                vec2 TexCoords;
            } vs_out)_struct_")

            .add_var("uniform", "mat4", "projection")
            .add_var("uniform", "mat4", "view")
            .add_var("uniform", "mat4", "model")
            .add_var("uniform", "bool", "reverse_normals")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
                if(reverse_normals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
                    vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
                else
                    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
                vs_out.TexCoords = aTexCoords;
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            )_main_").str()
        ).
        attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec3 Normal;
                vec2 TexCoords;
            } fs_in)_struct_")

            .add_var("uniform", "sampler2D", "diffuseTexture")
            .add_var("uniform", "samplerCube", "depthMap")
            .add_var("uniform", "vec3", "lightPos")
            .add_var("uniform", "vec3", "viewPos")
            .add_var("uniform", "float", "far_plane")
            .add_var("uniform", "bool", "shadows")
            .add_var("",        "vec3", "gridSamplingDisk[20]", R"_var_(vec3[] (
                   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
                   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
                   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
                   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
                   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
            ))_var_")

            .add_var("out", "vec4", "FragColor")

            .add_func("float", "ShadowCalculation", "vec3 fragPos", R"_fun_(
                vec3 fragToLight   = fragPos - lightPos;
                float currentDepth = length(fragToLight);

                float shadow = 0.0;
                float bias   = 0.15;
                int samples  = 20;
                float viewDistance = length(viewPos - fragPos);
                float diskRadius   = (1.0 + (viewDistance / far_plane)) / 25.0;

                for(int i = 0; i < samples; i++) {
                    float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
                    closestDepth *= far_plane;   // undo mapping [0;1]
                    if(currentDepth - bias > closestDepth)
                        shadow += 1.0;
                }
                return shadow / float(samples);
            )_fun_")
            .add_func("void", "main", "", R"_main_(
                vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
                vec3 normal = normalize(fs_in.Normal);
                vec3 lightColor = vec3(0.3);

                // ambient
                vec3 ambient = 0.3 * lightColor;

                // diffuse
                vec3 lightDir = normalize(lightPos - fs_in.FragPos);
                float diff = max(dot(lightDir, normal), 0.0);
                vec3 diffuse = diff * lightColor;

                // specular
                vec3 viewDir = normalize(viewPos - fs_in.FragPos);
                vec3 reflectDir = reflect(-lightDir, normal);
                float spec = 0.0;
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
                vec3 specular = spec * lightColor;  
  
                // calculate shadow
                float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;                      
                vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
                FragColor = vec4(lighting, 1.0);
            )_main_")
            .str()
        ).
        link();
}
void ViewTest::createSimpleDepthShader() {
    simpleDepthShader.
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("uniform", "mat4", "model")
            .add_func("void", "main", "", R"_main_(
                gl_Position = model * vec4(aPos, 1.0);
            )_main_")
            .str()
        ).
        attachSource(GL_GEOMETRY_SHADER, ShaderSource{}
            .add_var("in", "layout", "(triangles)")
            .add_var("uniform", "mat4", "shadowMatrices[6]")
            .add_var("out", "layout", "(triangle_strip, max_vertices = 18)")
            .add_var("out", "vec4", "FragPos")
            .add_func("void", "main", "", R"_main_(
                for(int face = 0; face < 6; face++) {
                    gl_Layer = face;                    // built-in variable: face rendered
                    for(int i = 0; i < 3; ++i) {
                        FragPos = gl_in[i].gl_Position;
                        gl_Position = shadowMatrices[face] * FragPos;
                        EmitVertex();
                    }    
                    EndPrimitive();
                }
            )_main_")
            .str()
        ).
        attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec4", "FragPos")
            .add_var("uniform", "vec3", "lightPos")
            .add_var("uniform", "float", "far_plane")
            .add_func("void", "main", "", R"_main_(
                // map to [0;1] range by dividing by far_plane as modified depth
                gl_FragDepth = length(FragPos.xyz - lightPos) / far_plane; 
            )_main_")
            .str()
        ).
        link();
}