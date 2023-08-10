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
layout(location = 0) out vec4 blur_color;
layout(location = 0)  in vec2 v_UV;

layout(binding = 0) uniform sampler2D source_texture;
//kernel11x11
#define kernel11x11
#include post_processing/gaussion_kernel.glsl

void main()
{
    // caution: do not blur the alpha channel!
    float dx = 1.0 / (textureSize(source_texture, 0)).x;
    float dy = 1.0 / (textureSize(source_texture, 0)).y;

    float sdx = 1.0 / (textureSize(source_texture, 0)).x;
    vec3 color = texture(source_texture, v_UV).rgb * weight[0];
    for (int i = 1; i < 6; i++)
    {
        vec2 offset = vec2(sdx * i, 0.0);
        color += texture(source_texture, v_UV + offset).rgb * weight[i];
        color += texture(source_texture, v_UV - offset).rgb * weight[i];
    }
    blur_color=vec4(color,1.0);
//    vec3 hdrColor= texture(source_texture, v_UV).rgb;
//    const float gamma = 2.2;
//    // Reinhard色调映射
//    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
//    // Gamma校正
//    mapped = pow(mapped, vec3(1.0 / gamma));
//    blur_color=vec4(mapped,1.0);

}