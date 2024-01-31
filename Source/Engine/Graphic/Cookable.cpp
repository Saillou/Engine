#include "Cookable.hpp"

#include <iostream>
#include <glad/glad.h>

void Cookable::Set(CookType type, Shader& recipe) {
    switch (type) {
    case CookType::Basic:
        _set_shader_basic(recipe);
        break;

    case CookType::Geometry:
        _set_shader_geometry(recipe);
        break;

    case CookType::Shape:
        _set_shader_shape(recipe);
        break;

    default:
        std::cerr << "[Warning] No shader created." << std::endl;
        return;
    }

    recipe.link();
}

// - Recipes
Cookable* Cookable::addRecipe(CookType type) {
    // Cached
    if (has(type))
        return this;

    // Create
    m_shaders[type] = std::make_unique<Shader>();
    UShader& recipe = m_shaders[type];

    Set(type, *recipe);

    return this;
}

void Cookable::editRecipe(CookType shaderType, ShaderSource::Type sourceType, const ShaderSource& new_source) {
    Shader::Release();

    const auto& old_shader = get(shaderType);
    UShader new_shader = std::make_unique<Shader>();

    for (const auto& old_source: old_shader->m_sources) {
        new_shader->attachSource(old_source.first,
            old_source.first == sourceType ?
            ShaderSource::Edit(old_source.second, new_source) :
            old_source.second
        );
    }

    new_shader->link();
    m_shaders[shaderType].swap(new_shader);
}

bool Cookable::has(CookType type) const {
    return m_shaders.find(type) != m_shaders.cend();
}

UShader& Cookable::get(CookType type) {
    return m_shaders[type];
}

// - Source helpers
ShaderSource Cookable::_init_vertex() {
    return ShaderSource{}
        .add_var("layout (location = 0) in", "vec3", "aPos")
        .add_var("layout (location = 1) in", "vec3", "aNormal")
        .add_var("layout (location = 2) in", "vec2", "aTexCoords")
        .add_var("layout (location = 3) in", "vec4", "aColor")
        .add_var("layout (location = 4) in", "mat4", "aModel")

        .add_var("uniform", "mat4", "Projection")
        .add_var("uniform", "mat4", "View")
        .add_var("uniform", "mat4", "LocalModel")

        .add_var("out", "VS_OUT", R"_struct_({
            vec3 FragPos;
            vec3 Normal;
            vec2 TexCoords;
            vec4 Color;
        } vs_out)_struct_")
    ;
}

ShaderSource Cookable::_init_fragment() {
    return ShaderSource{}
        .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec3 Normal;
                vec2 TexCoords;
                vec4 Color;
            } fs_in)_struct_")

        .add_var("uniform", "vec4", "diffuse_color")
        .add_var("uniform", "sampler2D", "texture_diffuse")

        .add_var("out", "vec4", "FragColor")
    ;
}


// - Shaders
void Cookable::_set_shader_basic(Shader& shader) {
    shader
        .attachSource(GL_VERTEX_SHADER, 
            _init_vertex()

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos   = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Normal    = mat3(transpose(inverse(aModel * LocalModel))) * aNormal;  
                vs_out.TexCoords = aTexCoords;
                vs_out.Color     = aColor;

                gl_Position = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        ).
        attachSource(GL_FRAGMENT_SHADER, 
            _init_fragment()

            .add_var("uniform", "bool",        "use_shadow")
            .add_var("uniform", "highp int",   "LightNumber")
            .add_var("uniform", "vec3",        "LightPos",   1)
            .add_var("uniform", "vec4",        "LightColor", 1)
            .add_var("uniform", "samplerCube", "depthMap",   1)

            .add_var("uniform", "vec3", "CameraPos")
            .add_var("uniform", "float", "far_plane")
            .add_var("", "vec3", "gridSamplingDisk", 20, R"_var_(vec3[] (
                   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
                   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
                   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
                   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
                   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
            ))_var_")

            .add_func("float", "ShadowCalculation", "vec3 fragPos, int i", R"_fun_(
                vec3 lightPos      = LightPos[i];
                vec3 fragToLight   = fragPos - lightPos;
                float currentDepth = length(fragToLight);

                float shadow = 0.0;
                float bias   = 0.15;
                int samples  = 20;
                float viewDistance = length(CameraPos - fragPos);
                float diskRadius   = (1.0 + (viewDistance / far_plane)) / 25.0;

                for(int iSample = 0; iSample < samples; iSample++) {
                    float closestDepth = texture(depthMap[i], fragToLight + gridSamplingDisk[iSample] * diskRadius).r;
                    closestDepth *= far_plane;
                    if(currentDepth - bias > closestDepth)
                        shadow += 1.0;
                }
                return shadow / float(samples);
            )_fun_")

            .add_func("vec3", "compute_light", "vec3 obj_color, int i", R"_light_(
                vec3 light_pos   = LightPos[i];
                vec3 light_color = LightColor[i].rgb;

                // ambient
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * light_color;

                // diffuse 
                vec3 normDir = normalize(fs_in.Normal);
                vec3 lightDir = normalize(light_pos - fs_in.FragPos);
                float diff = max(dot(normDir, lightDir), 0.0);
                vec3 diffuse = 0.7 * diff * light_color;

                // specular
                float specularStrength = 0.5;
                vec3 cameraDir = normalize(CameraPos - fs_in.FragPos);
                vec3 reflectDir = reflect(-lightDir, normDir);  
                float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * light_color;

                // calculate shadow
                float shadow = use_shadow ? ShadowCalculation(fs_in.FragPos, i) : 0.0;                      
                return (ambient + (1.0 - shadow) * (diffuse + specular)) * obj_color.rgb;   
            )_light_")

            .add_func("void", "main", "", R"_main_(
                vec2 tex_size = textureSize(texture_diffuse, 0);
                vec4 ambiant_color = tex_size.x * tex_size.y > 1 ? texture(texture_diffuse, fs_in.TexCoords) : max(fs_in.Color, diffuse_color);

                // Shall use light ?
                if(LightNumber == 0) {
                    FragColor = ambiant_color;
                    return;
                }

                vec3 normal = normalize(fs_in.Normal);
                vec3 light_result = vec3(0,0,0);

                float ratio = 1.0 / float(max(1, LightNumber));

                for(int i = 0; i < LightNumber; i++) {
                    light_result += ratio * compute_light(ambiant_color.rgb, i);
                }
    
                FragColor = vec4(light_result, ambiant_color.a);
            )_main_")
        );
}

void Cookable::_set_shader_geometry(Shader& shader) {
    shader
        .attachSource(GL_VERTEX_SHADER, 
            _init_vertex()

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos  = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color    = aColor;

                gl_Position     = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
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
            )_main_")
        )
        .attachSource(GL_FRAGMENT_SHADER, 
            _init_fragment()

            .add_func("void", "main", "", R"_main_(
                FragColor = diffuse_color;
            )_main_")
        );
}

void Cookable::_set_shader_shape(Shader& shader) {
    shader
        .attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("uniform", "mat4", "projection")
            .add_var("uniform", "mat4", "LocalModel")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = projection * LocalModel * vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_")
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "quadTexture")
            .add_var("uniform", "vec4", "background_color")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                vec2 tex_size = textureSize(quadTexture, 0);
                vec4 bk_color = tex_size.x * tex_size.y > 1 ? texture(quadTexture, TexCoords) : background_color;
                FragColor = bk_color;
            )_main_")
        );
}
