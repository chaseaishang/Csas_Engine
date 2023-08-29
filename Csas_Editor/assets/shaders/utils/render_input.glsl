#ifndef RENDER_INPUT_H
#define RENDER_INPUT_H


// uniform blocks >= 10 are reserved for internal use only
layout(std140, binding = 10) uniform RendererInput {
    mat4 lightSpaceMatrix;
    float now_time;
    float delta_time;
} rdr_in;
struct Pixel {
    vec3 position;
    vec3 F0;
    vec3 N;
    vec3 V;
    vec3  albedo;
    float metallic;
    float roughness;
    float ao;
};

#endif