#include "Cookable.hpp"

#include <glad/glad.h>

// - Recipes
Cookable* Cookable::addRecipe(CookType type) {
    m_shaders[type] = std::make_unique<Shader>();
    UShader& recipe = m_shaders[type];

    switch (type) {
    case CookType::Solid:
        _set_shader_solid(recipe);
        break;

    case CookType::Geometry:
        _set_shader_geometry(recipe);
        break;

    case CookType::Quad:
        _set_shader_quad(recipe);
        break;

    case CookType::Batch:
        _set_shader_batch(recipe);
        break;

    default:
        return this;
    }

    recipe->link();

    return this;
}

Cookable* Cookable::addRecipe(CookType type, const glm::vec4& color) {
    addRecipe(type);

    // Set uniforms
    m_shaders[type]->use().set("color", color);

    return this;
}

UShader& Cookable::get(CookType type) {
    return m_shaders[type];
}

// - Shaders
void Cookable::_set_shader_batch(UShader& shader) {
    shader->
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec4", "aColor")
            .add_var("layout (location = 3) in", "mat4", "aModel")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")

            .add_var("out", "vec3", "Normal")
            .add_var("out", "vec4", "Color")
            .add_var("out", "vec3", "FragPos")

            .add_func("void", "main", "", R"_main_(
                FragPos = vec3(aModel * vec4(aPos, 1.0));
                Normal  = mat3(transpose(inverse(aModel))) * aNormal;  
                Color   = aColor;
    
                gl_Position = Projection * View * vec4(FragPos, 1.0);
            )_main_").str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec3", "FragPos")
            .add_var("in", "vec3", "Normal")
            .add_var("in", "vec4", "Color")

            .add_var("uniform", "vec3", "CameraPos")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                CameraPos;
                FragColor = Color;
            )_main_").str()
        );
}

void Cookable::_set_shader_solid(UShader& shader) {
    shader->
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "Model")

            .add_var("out", "vec3", "Normal")
            .add_var("out", "vec3", "FragPos")

            .add_func("void", "main", "", R"_main_(
                FragPos = vec3(Model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(Model))) * aNormal;  
    
                gl_Position = Projection * View * vec4(FragPos, 1.0);
            )_main_").str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec3", "Normal")
            .add_var("in", "vec3", "FragPos")

            .add_var("uniform", "vec4", "color")
            .add_var("uniform", "vec3", "CameraPos")
            .add_var("uniform", "lowp int", "LightLength")
            .add_var("uniform", "vec3", "LightPos")
            .add_var("uniform", "vec4", "LightColor")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                // Shall use light ?
                if(LightColor == vec4(0, 0, 0, 0)) {
                    FragColor = color;
                    return;
                }

                // ambient
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * vec3(LightColor.rgb);

                // diffuse 
                vec3 normDir = normalize(Normal);
                vec3 lightDir = normalize(LightPos - FragPos);
                float diff = max(dot(normDir, lightDir), 0.0);
                vec3 diffuse = diff * vec3(LightColor.rgb);

                // specular
                float specularStrength = 0.5;
                vec3 cameraDir = normalize(CameraPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, normDir);  
                float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * vec3(LightColor.rgb);

                vec3 result = (ambient + diffuse + specular) * color.rgb;
                FragColor = vec4(result, color.a);
            )_main_").str()
        );
}

void Cookable::_set_shader_geometry(UShader& shader) {
    shader->
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "Model")

            .add_var("out", "vec3", "Normal")
            .add_var("out", "vec3", "FragPos")

            .add_func("void", "main", "", R"_main_(
                FragPos = vec3(Model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(Model))) * aNormal;  
    
                gl_Position = Projection * View * vec4(FragPos, 1.0);
            )_main_").str()
        )
        .attachSource(GL_GEOMETRY_SHADER, ShaderSource{}
            .add_var("in", "layout", "(triangles)")
            .add_var("out", "layout", "(line_strip, max_vertices = 4)")

            .add_func("void", "main", "", R"_main_(
                gl_Position     = gl_in[1].gl_Position; EmitVertex();
                gl_Position     = gl_in[2].gl_Position; EmitVertex(); 
                EndPrimitive();

                gl_Position     = gl_in[0].gl_Position; EmitVertex();
                gl_Position     = gl_in[1].gl_Position; EmitVertex(); 
                EndPrimitive();
            )_main_").str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec3", "Normal")
            .add_var("in", "vec3", "FragPos")

            .add_var("uniform", "vec4", "color")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = color;
            )_main_").str()
        );
}


void Cookable::_set_shader_quad(UShader& shader) {
    shader->
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_").str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "quadTexture")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                FragColor = texture(quadTexture, TexCoords);
            )_main_").str()
        );
}
