// Basic Cube Shader

#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_UV;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection;
}Camera;
layout(location = 0)  uniform  mat4 model;

layout(location = 0)  out vec4 v_Color;
layout(location = 1)  out vec3 v_Normal;
layout(location = 2)  out vec2 v_UV;
void main()
{
    v_Color  = a_Color;
    v_Normal = a_Normal;
    v_UV     = a_UV;
    gl_Position = Camera.u_ViewProjection * model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;
layout (std140, binding = 1)uniform BlobSettings
{
    vec4 InnerColor;      //16                  0        {1.0f, 1.0f, 0.75f, 1.0f}
    vec4 OuterColor;      //16                  16           {0.0f, 0.0f, 0.0f, 0.0f}
    float RadiusInner;    //4                   20              0.25f
    float RadiusOuter;    //4                   24              0.45
};
layout(location = 0)  in vec4 v_Color;
layout(location = 1)  in vec3 v_Normal;
layout(location = 2)  in vec2 v_UV;
void main()
{
    float dx = v_UV.x - 0.5;//-0.5 0.5
    float dy = v_UV.y - 0.5;
    float dist = sqrt(dx * dx + dy * dy);//0 1
    color = mix( InnerColor, OuterColor,
    smoothstep( RadiusInner, RadiusOuter, dist )
    );


}