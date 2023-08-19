// Basic ADS Light Shader

#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 0) uniform  mat4 model;
layout(location = 1) uniform  mat4 lightSpaceMatrix;

void main()
{

    gl_Position = lightSpaceMatrix * model * vec4(a_Position, 1.0);
}
#type fragment
#version 450 core
void main()
{

}