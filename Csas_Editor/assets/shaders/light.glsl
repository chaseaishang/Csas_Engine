// Basic ADS Light Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

#include Camera.glsl

layout(location = 0) uniform  mat4 model;


//layout(location = 3) uniform float bloom_factor;


void main()
{
    mat4 ViewProj= Camera.Projection*Camera.View;//ViewProj * model *
    gl_Position = ViewProj*model* vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 1) uniform vec4  light_color;
layout(location = 2) uniform float light_intensity;
layout(location = 0)  out vec4 FragColor;

void main()
{
    vec3 color = vec3(light_color) * light_intensity;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor=vec4(color,1.0);


}