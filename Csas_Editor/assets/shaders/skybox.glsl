// Basic ADS Light Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

#include Camera.glsl
layout(location = 0) uniform  mat4 model;
layout(location = 0)  out vec3 v_tex_coords;
void main()
{
    v_tex_coords=a_Position;

    mat4 view = mat4(mat3(Camera.View));
    mat4 ViewProj= Camera.Projection*view;
    gl_Position = ViewProj * model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0)  out vec4 FragColor;
layout(location = 0)  in vec3 v_tex_coords;
layout(binding = 0) uniform samplerCube skybox;
vec3 Reinhard(vec3 irradiance)
{
    return irradiance / (1.0 + irradiance);
}
void main()
{
    vec3 hdrColor = texture(skybox, v_tex_coords).rgb;

    vec3 mapped=Reinhard(hdrColor);
    // Gamma校正
    const float gamma = 2.2;
    mapped = pow(mapped, vec3(1.0 / gamma));

    //FragColor = vec4(hdrColor,1.0);
    FragColor = vec4(mapped,1.0);


}