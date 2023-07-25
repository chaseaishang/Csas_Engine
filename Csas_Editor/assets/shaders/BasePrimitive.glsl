// Basic Cube Shader

#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_UV;
layout (std140, binding = 0) uniform Matrices
{
    mat4 View;
    mat4 Projection;
}Camera;
layout(location = 0) uniform  mat4 model;

layout(location = 0)  out vec4 v_Color;
layout(location = 1)  out vec3 v_Normal;
layout(location = 2)  out vec2 v_UV;
void main()
{
    v_Color  = a_Color;
    v_Normal = a_Normal;
    v_UV     = a_UV;
    mat4 ViewProj= Camera.Projection*Camera.View;
    gl_Position = ViewProj * model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;

layout(location = 0)  in vec4 v_Color;
layout(location = 1)  in vec3 v_Normal;
layout(location = 2)  in vec2 v_UV;
void main()
{
    //    vec3 Normal=(v_Normal+1)*0.5f;
    //    color =vec4(Normal,1.0);

    color = v_Color;


    //
}