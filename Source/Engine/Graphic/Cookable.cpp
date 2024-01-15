#include "Cookable.hpp"

#include <glad/glad.h>

// - Recipes
Cookable* Cookable::addRecipe(CookType type) {
    // Cached
    if (m_shaders.find(type) != m_shaders.cend())
        return this;

    // Create
    m_shaders[type] = std::make_unique<Shader>();
    UShader& recipe = m_shaders[type];

    switch (type) {
    case CookType::Solid:
        _set_shader_solid(recipe);
        break;

    case CookType::Model:
        _set_shader_model(recipe);
        break;

    case CookType::Shadow:
        _set_shader_shadow(recipe);
        break;

    case CookType::BatchShadow:
        _set_shader_shadow_batch(recipe);
        break;

    case CookType::Geometry:
        _set_shader_geometry(recipe);
        break;

    case CookType::ModelGeometry:
        _set_shader_model_geometry(recipe);
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
    m_shaders[type]->use().set("diffuse_color", color);

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
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")
            .add_var("layout (location = 3) in", "vec4", "aColor")
            .add_var("layout (location = 4) in", "mat4", "aModel")

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

void Cookable::_set_shader_shadow_batch(UShader& shader) {
    shader->
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

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "Model")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
                vs_out.Normal = transpose(inverse(mat3(Model))) * aNormal;
                vs_out.TexCoords = aTexCoords;
                gl_Position = Projection * View * Model * vec4(aPos, 1.0);
            )_main_").str()
        ).
        attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec3 Normal;
                vec2 TexCoords;
            } fs_in)_struct_")

            .add_var("uniform", "vec4", "diffuse_color")
            .add_var("uniform", "samplerCube", "depthMap")
            .add_var("uniform", "vec3", "lightPos")
            .add_var("uniform", "vec4", "lightColor")
            .add_var("uniform", "vec3", "viewPos")
            .add_var("uniform", "float", "far_plane")
            .add_var("", "vec3", "gridSamplingDisk[20]", R"_var_(vec3[] (
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
                vec3 color = diffuse_color.rgb;
                vec3 normal = normalize(fs_in.Normal);

                // ambient
                vec3 ambient = 0.3 * lightColor.rgb;

                // diffuse
                vec3 lightDir = normalize(lightPos - fs_in.FragPos);
                float diff = max(dot(lightDir, normal), 0.0);
                vec3 diffuse = diff * lightColor.rgb;

                // specular
                vec3 viewDir = normalize(viewPos - fs_in.FragPos);
                vec3 reflectDir = reflect(-lightDir, normal);
                float spec = 0.0;
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
                vec3 specular = spec * lightColor.rgb;  
  
                // calculate shadow
                float shadow = ShadowCalculation(fs_in.FragPos);                      
                vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
                FragColor = vec4(lighting, 1.0);
            )_main_")
            .str()
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
            )_main_")
            .str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec3", "Normal")
            .add_var("in", "vec3", "FragPos")

            .add_var("uniform", "vec4", "diffuse_color")
            .add_var("uniform", "vec3", "CameraPos")

            .add_var("uniform", "highp int", "LightNumber")

            .add_var("uniform", "vec3", "LightPos_0")
            .add_var("uniform", "vec3", "LightPos_1")
            .add_var("uniform", "vec3", "LightPos_2")
            .add_var("uniform", "vec3", "LightPos_3")
            .add_var("uniform", "vec3", "LightPos_4")

            .add_var("uniform", "vec4", "LightColor_0")
            .add_var("uniform", "vec4", "LightColor_1")
            .add_var("uniform", "vec4", "LightColor_2")
            .add_var("uniform", "vec4", "LightColor_3")
            .add_var("uniform", "vec4", "LightColor_4")

            .add_var("out", "vec4", "FragColor")

            .add_func("vec3", "compute_light", "vec3 light_pos, vec3 light_color", R"_light_(
                // ambient
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * light_color;

                // diffuse 
                vec3 normDir = normalize(Normal);
                vec3 lightDir = normalize(light_pos - FragPos);
                float diff = max(dot(normDir, lightDir), 0.0);
                vec3 diffuse = 0.7 * diff * light_color;

                // specular
                float specularStrength = 0.5;
                vec3 cameraDir = normalize(CameraPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, normDir);  
                float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * light_color;

                return (ambient + diffuse + specular) * diffuse_color.rgb;
            )_light_")

            .add_func("void", "main", "", R"_main_(
                // Shall use light ?
                if(LightNumber == 0) {
                    FragColor = diffuse_color;
                    return;
                }

                float ratio = 1.0 / float(LightNumber);

                vec3 light_result = vec3(0,0,0);
                light_result += ratio * compute_light(LightPos_0, LightColor_0.rgb);
                light_result += ratio * compute_light(LightPos_1, LightColor_1.rgb);
                light_result += ratio * compute_light(LightPos_2, LightColor_2.rgb);
                light_result += ratio * compute_light(LightPos_3, LightColor_3.rgb);
                light_result += ratio * compute_light(LightPos_4, LightColor_4.rgb);

                FragColor = vec4(light_result, diffuse_color.a);
            )_main_")
            .str()
        );
}

void Cookable::_set_shader_model(UShader& shader) {
    shader->
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "Model")
            .add_var("uniform", "mat4", "LocalModel")

            .add_var("out", "vec3", "Normal")
            .add_var("out", "vec3", "FragPos")
            .add_var("out", "vec2", "TexCoords")

            .add_func("void", "main", "", R"_main_(
                TexCoords = aTexCoords;
                FragPos   = vec3(Model * LocalModel * vec4(aPos, 1.0));
                Normal    = mat3(transpose(inverse(Model * LocalModel))) * aNormal;  
    
                gl_Position = Projection * View * vec4(FragPos, 1.0);
            )_main_")
            .str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec3", "Normal")
            .add_var("in", "vec3", "FragPos")
            .add_var("in", "vec2", "TexCoords")

            .add_var("uniform", "sampler2D", "texture_diffuse")
            .add_var("uniform", "vec4", "diffuse_color")
            .add_var("uniform", "vec3", "CameraPos")

            .add_var("uniform", "highp int", "LightNumber")

            .add_var("uniform", "vec3", "LightPos_0")
            .add_var("uniform", "vec3", "LightPos_1")
            .add_var("uniform", "vec3", "LightPos_2")
            .add_var("uniform", "vec3", "LightPos_3")
            .add_var("uniform", "vec3", "LightPos_4")

            .add_var("uniform", "vec4", "LightColor_0")
            .add_var("uniform", "vec4", "LightColor_1")
            .add_var("uniform", "vec4", "LightColor_2")
            .add_var("uniform", "vec4", "LightColor_3")
            .add_var("uniform", "vec4", "LightColor_4")

            .add_var("out", "vec4", "FragColor")

            .add_func("vec3", "compute_light", "vec3 obj_color, vec3 light_pos, vec3 light_color", R"_light_(
                // ambient
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * light_color;

                // diffuse 
                vec3 normDir = normalize(Normal);
                vec3 lightDir = normalize(light_pos - FragPos);
                float diff = max(dot(normDir, lightDir), 0.0);
                vec3 diffuse = 0.7 * diff * light_color;

                // specular
                float specularStrength = 0.5;
                vec3 cameraDir = normalize(CameraPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, normDir);  
                float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * light_color;

                return (ambient + diffuse + specular) * obj_color.rgb;
            )_light_")

            .add_func("void", "main", "", R"_main_(
                vec2 tex_size = textureSize(texture_diffuse, 0);
                vec4 ambiant_color = tex_size.x * tex_size.y > 1 ? texture(texture_diffuse, TexCoords) : diffuse_color;

                // Shall use light ?
                if(LightNumber == 0) {
                    FragColor = ambiant_color;
                    return;
                }

                float ratio = 1.0 / float(LightNumber);

                vec3 light_result = vec3(0,0,0);
                light_result += ratio * compute_light(ambiant_color.rgb, LightPos_0, LightColor_0.rgb);
                light_result += ratio * compute_light(ambiant_color.rgb, LightPos_1, LightColor_1.rgb);
                light_result += ratio * compute_light(ambiant_color.rgb, LightPos_2, LightColor_2.rgb);
                light_result += ratio * compute_light(ambiant_color.rgb, LightPos_3, LightColor_3.rgb);
                light_result += ratio * compute_light(ambiant_color.rgb, LightPos_4, LightColor_4.rgb);

                FragColor = vec4(light_result, ambiant_color.a);
            )_main_")
            .str()
        );
}

void Cookable::_set_shader_shadow(UShader& shader) {
    shader->
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

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "Model")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
                vs_out.Normal = transpose(inverse(mat3(Model))) * aNormal;
                vs_out.TexCoords = aTexCoords;
                gl_Position = Projection * View * Model * vec4(aPos, 1.0);
            )_main_").str()
        ).
        attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec3 Normal;
                vec2 TexCoords;
            } fs_in)_struct_")

            .add_var("uniform", "vec4", "diffuse_color")
            .add_var("uniform", "samplerCube", "depthMap")
            .add_var("uniform", "vec3", "lightPos")
            .add_var("uniform", "vec4", "lightColor")
            .add_var("uniform", "vec3", "viewPos")
            .add_var("uniform", "float", "far_plane")
            .add_var("", "vec3", "gridSamplingDisk[20]", R"_var_(vec3[] (
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
                vec3 color = diffuse_color.rgb;
                vec3 normal = normalize(fs_in.Normal);

                // ambient
                vec3 ambient = 0.3 * lightColor.rgb;

                // diffuse
                vec3 lightDir = normalize(lightPos - fs_in.FragPos);
                float diff = max(dot(lightDir, normal), 0.0);
                vec3 diffuse = diff * lightColor.rgb;

                // specular
                vec3 viewDir = normalize(viewPos - fs_in.FragPos);
                vec3 reflectDir = reflect(-lightDir, normal);
                float spec = 0.0;
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
                vec3 specular = spec * lightColor.rgb;  
  
                // calculate shadow
                float shadow = ShadowCalculation(fs_in.FragPos);                      
                vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
                FragColor = vec4(lighting, 1.0);
            )_main_")
            .str()
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

            .add_var("uniform", "vec4", "diffuse_color")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = diffuse_color;
            )_main_").str()
        );
}

void Cookable::_set_shader_model_geometry(UShader& shader) {
    shader->
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "Model")
            .add_var("uniform", "mat4", "LocalModel")

            .add_var("out", "vec3", "Normal")
            .add_var("out", "vec3", "FragPos")

            .add_func("void", "main", "", R"_main_(
                FragPos   = vec3(Model * LocalModel * vec4(aPos, 1.0));
                Normal    = mat3(transpose(inverse(Model * LocalModel))) * aNormal; 
    
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

            .add_var("uniform", "vec4", "diffuse_color")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = diffuse_color;
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
