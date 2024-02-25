#include "_ShaderHelper.hpp"

// - Source helpers
ShaderSource ShaderHelper::_init_vertex() {
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

ShaderSource ShaderHelper::_init_geometry()
{
    return ShaderSource{}
        .add_var("in", "VS_OUT", R"_struct_({
            vec3 FragPos;
            vec3 Normal;
            vec2 TexCoords;
            vec4 Color;
        } gs_in[])_struct_")
    ;
}

ShaderSource ShaderHelper::_init_fragment() {
    return ShaderSource{}
        .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec3 Normal;
                vec2 TexCoords;
                vec4 Color;
            } fs_in)_struct_")

        .add_var("uniform", "sampler2D", "texture_diffuse")

        .add_var("out", "vec4", "FragColor")
    ;
}