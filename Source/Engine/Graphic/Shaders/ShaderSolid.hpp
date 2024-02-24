#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderSolid : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderSolid::Create()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

    (*shader)
        .attachSource(Shader::Vertex,
            _init_vertex()

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos   = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Normal    = mat3(transpose(inverse(aModel * LocalModel))) * aNormal;  
                vs_out.TexCoords = aTexCoords;
                vs_out.Color     = aColor;

                gl_Position = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        )
        .attachSource(Shader::Fragment,
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
                vec4 ambiant_color = tex_size.x * tex_size.y > 1 ? texture(texture_diffuse, fs_in.TexCoords) : fs_in.Color;

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
        )
    ;
	return shader;
}