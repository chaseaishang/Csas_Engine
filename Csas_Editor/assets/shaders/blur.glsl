// blur https://zhuanlan.zhihu.com/p/128023876
//https://blog.csdn.net/h5502637/article/details/90271986
#type vertex
#version 450 core

layout(location = 0)  out vec2 v_UV;

void main()
{
    vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    v_UV = (position + 1) * 0.5;
    gl_Position = vec4(position, 0.0, 1.0);


}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;
layout(location = 0)  in vec2 v_UV;

layout(binding = 0) uniform sampler2D source_texture;

void main()
{

    color=texture(source_texture,v_UV);

}